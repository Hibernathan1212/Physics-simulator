#version 450
in vec4 gl_FragCoord;

struct Material
{
    vec3 Albedo;
    float Roughness;
    vec3 Emission;
    //float Metallic;
};

struct Triangle
{
    vec3 A;
    vec3 B;
    vec3 C;
    vec3 normA;
    vec3 normB;
    vec3 normC;
};

struct BVHNode
{
    vec3 boundsMin;
    vec3 boundsMax;
    int startIndex;
    int triangleCount;
};

struct Object
{
    int         nodeOffset;
    int         triOffset;
    mat4        transform;
    mat4        invTransform;
    Material    material;
};

struct CameraData
{
    vec3 CameraOrigin;
    float ViewportWidth;
    float ViewportHeight;
    mat4 Projection;
    mat4 View;
    mat4 InverseProjection;
    mat4 InverseView;
};

layout(binding = 0) uniform UniformData
{
    Object          objects[2];
    Triangle        triangles[3000000];
    BVHNode         nodes[6000000];
    
    uint            lightObjIndices;
    uint            objectCount;
    uint            lightCount;
    CameraData      cameraData;
} u_Data;

layout(location = 0) out vec4 outColor;

struct Ray
{
    vec3 origin;
    vec3 direction;
    vec3 invDir;
};

struct HitPayload
{
    float hitDistance;
    vec3 normal;
    int objIndex;
};

struct TriangleHitInfo
{
    float dst;
    vec3 normal;
};

vec3 CalculateRayDirection(float x, float y);

vec4 PerPixel(float x, float y);

vec3 TraceRay(Ray ray, int i);

HitPayload CalculateRayCollision(Ray ray);

vec3 BRDF(vec3 incomingColor, Ray incomingRay, Ray outgoingRay, vec3 norm, Material material);


TriangleHitInfo RayTriangleBVH(Ray ray, float rayLength, int nodeOffset, int triOffset);

TriangleHitInfo RayTriangle(Ray ray, Triangle tri);

float RayBoundingBoxDistance(Ray ray, vec3 boxMin, vec3 boxMax);


HitPayload Miss(Ray ray);

float rand(vec3 c0)
{
    return fract(sin(dot(c0, vec3(12.9898, 357, 23.829))) * 43758.5453);
}

void main()
{
    vec4 color = PerPixel(gl_FragCoord.x, gl_FragCoord.y);
    
    color = clamp(sqrt(color), vec4(0.0f), vec4(1.0f));
    outColor = color;
}

vec3 CalculateRayDirection(float x, float y)
{
    vec2 coord = { (x) / u_Data.cameraData.ViewportWidth, (y) / u_Data.cameraData.ViewportHeight };
    coord = (coord - 1.0f) * 2.0f; // -1 -> 1

    vec4 target = u_Data.cameraData.InverseProjection * vec4(coord.x, coord.y, 1, 1);
    vec3 rayDirection = vec3(u_Data.cameraData.InverseView * vec4(normalize(vec3(target) / target.w), 0)); // World space
    return rayDirection;
}

vec4 PerPixel(float x, float y)
{
    Ray ray;
    
    ray.direction = CalculateRayDirection(gl_FragCoord.x, gl_FragCoord.y);
    ray.invDir = 1.0f / ray.direction;
    ray.origin = u_Data.cameraData.CameraOrigin;

    int bounces = 1;
    int paths = 4;

    vec3 light = vec3(0.0f, 0.0f, 0.0f);
    vec3 contribution = vec3(1.0f, 1.0f, 1.0f);

    Material material;
    
    for (int bounce = 0; bounce < bounces; bounce++)
    {
        HitPayload payload = CalculateRayCollision(ray);
        
        if (payload.hitDistance == 1.0f / 0.0f)
        {
            vec3 skyColor = vec3(0.3f / 500.0f, 0.35f / 500.0f, 0.45f / 500.0f);
            light += skyColor * contribution;
            break;
        }
        
        payload.normal = vec4(u_Data.objects[payload.objIndex].transform * vec4(normalize(payload.normal), 0)).xyz;
        ray.origin = (ray.origin + ray.direction * payload.hitDistance) + payload.normal * 0.01f;
        
        material = u_Data.objects[payload.objIndex].material;
        
        //light += material.Emission * contribution;
        contribution *= material.Albedo;
        
        Ray secondaryRay;
        secondaryRay.origin = ray.origin;
        HitPayload secondaryPayload;
        
        for (int lightPath = 0; lightPath < u_Data.lightCount; lightPath++)
        {
            vec3 dif = secondaryRay.origin - vec4(u_Data.objects[u_Data.lightObjIndices].transform *  vec4(u_Data.triangles[u_Data.objects[u_Data.lightObjIndices].triOffset].A, 1.0f)).xyz;
            secondaryRay.direction = normalize(-dif);

            if (dot(secondaryRay.direction, payload.normal) <= 0.001f)
                continue;
        
            secondaryRay.invDir = 1.0f / secondaryRay.direction;
            
            secondaryPayload = CalculateRayCollision(secondaryRay);
            if (secondaryPayload.objIndex == u_Data.lightObjIndices)
                light += u_Data.objects[u_Data.lightObjIndices].material.Emission * contribution * pow(dot(payload.normal, secondaryRay.direction), (1.0f / u_Data.objects[u_Data.lightObjIndices].material.Roughness));  //BRDF(contribution, ray, secondaryRay, vec4(u_Data.objects[secondaryPayload.objIndex].transform * vec4(normalize(secondaryPayload.normal), 0)).xyz, u_Data.objects[u_Data.lightObjIndices].material);
        }
        
        for (int path = 0; path < paths; path++)
        {
            secondaryRay.origin = ray.origin;
            secondaryRay.direction = ray.direction;
            secondaryRay.invDir = ray.invDir;

            vec3 secondaryContribution = contribution;
            
            Material secondaryMaterial = material;
            
            secondaryPayload = payload;
            
            for (int secondaryBounce = bounce; secondaryBounce < 2; secondaryBounce++)
            {
                float rand1 = rand(secondaryRay.origin*(path+bounce+1));
                float rand2 = rand(secondaryRay.origin*2*(path+bounce+1));
                float rand3 = rand(secondaryRay.origin*3*(path+bounce+1));
                
                vec3 diffuseDirection = normalize(0.5f * secondaryPayload.normal + vec3(rand1 - rand2, rand2 - rand3, rand3 - rand1));
                vec3 specularDirection = reflect(secondaryRay.direction, secondaryPayload.normal);
                secondaryRay.direction = mix(specularDirection, diffuseDirection, secondaryMaterial.Roughness);
                secondaryRay.invDir = 1.0f / secondaryRay.direction;
                
                secondaryPayload = CalculateRayCollision(secondaryRay);

                if (secondaryPayload.hitDistance == 1.0f / 0.0f)
                {
                    vec3 skyColor = vec3(0.3f / 500.0f, 0.35f / 500.0f, 0.45f / 500.0f);
                    light += skyColor * contribution;
                    break;
                }
                
                secondaryMaterial = u_Data.objects[secondaryPayload.objIndex].material;
                
                light += secondaryMaterial.Emission * secondaryContribution;
                secondaryContribution *= secondaryMaterial.Albedo;
                
                secondaryPayload.normal = vec4(u_Data.objects[secondaryPayload.objIndex].transform * vec4(normalize(secondaryPayload.normal), 0)).xyz;
                secondaryRay.origin = (secondaryRay.origin + secondaryRay.direction * secondaryPayload.hitDistance) + secondaryPayload.normal * 0.01f;
            }
        }
        
        float rand1 = rand(ray.origin*(bounce+1));
        float rand2 = rand(ray.origin*2*(bounce+1));
        float rand3 = rand(ray.origin*3*(bounce+1));
        
        vec3 diffuseDirection = normalize(0.5f * payload.normal + vec3(rand1 - rand2, rand2 - rand3, rand3 - rand1));
        vec3 specularDirection = reflect(ray.direction, payload.normal);// + material.Roughness * vec3(rand(payload.WorldPosition*(i+2)/2)));
        ray.direction = mix(specularDirection, diffuseDirection, material.Roughness);
    }
    
    //return light;
    
    return vec4(light, 1.0f);
}

vec3 BRDF(vec3 incomingColor, Ray incomingRay, Ray outgoingRay, vec3 norm, Material material)
{
    return incomingColor * material.Albedo * dot(norm, -incomingRay.direction);// * mix( , , material.roughness)
}


//vec3 getNewDirection(vec3 normal, float roughness)
//{
//    vec3 diffuseDirection = normalize(0.5f * payload.normal + vec3(rand1 - rand2, rand2 - rand3, rand3 - rand1));
//    vec3 specularDirection = reflect(ray.direction, payload.normal);// + material.Roughness * vec3(rand(payload.WorldPosition*(i+2)/2)));
//
//    return mix(specularDirection, diffuseDirection, material.Roughness);
//}

//vec3 TraceRay(Ray ray, int i)
//{
//    int paths = 8;
//    
//    for (int i = 0; i < paths; i++)
//    {
//        
//    }
//    
//}

HitPayload CalculateRayCollision(Ray ray)
{
    HitPayload result;
    result.hitDistance = 1.0f / 0.0f;
    result.objIndex = -1;
    Ray localRay;

    for (int i = 0; i < u_Data.objectCount; i++)
    {
        localRay.origin = vec4(u_Data.objects[i].invTransform * vec4(ray.origin, 1.0f)).xyz;
        localRay.direction = vec4(u_Data.objects[i].invTransform * vec4(ray.direction, 0.0f)).xyz;
        localRay.invDir = 1.0f / localRay.direction;

        TriangleHitInfo hit = RayTriangleBVH(localRay, result.hitDistance, u_Data.objects[i].nodeOffset, u_Data.objects[i].triOffset);

        // Record closest hit
        if (hit.dst < result.hitDistance)
        {
            result.hitDistance = hit.dst;
            result.normal = hit.normal;
            
            result.objIndex = i;
        }
    }

    return result;
}

TriangleHitInfo RayTriangleBVH(Ray ray, float rayLength, int nodeOffset, int triOffset)
{
    TriangleHitInfo result;
    result.dst = rayLength;
    
    int stack[32];
    int stackIndex = 0;
    stack[stackIndex] = nodeOffset;
    stackIndex++;
    
    while (stackIndex > 0)
    {
        stackIndex--;
        int triangleCount = u_Data.nodes[stack[stackIndex]].triangleCount;
        int startIndex = u_Data.nodes[stack[stackIndex]].startIndex;
        
        if (triangleCount > 0)
        {
            for (int i = 0; i < triangleCount; i++)
            {
                TriangleHitInfo triHitInfo = RayTriangle(ray, u_Data.triangles[triOffset + startIndex + i]);

                if (triHitInfo.dst < result.dst)
                {
                    result = triHitInfo;
                }
            }
        }
        else
        {
            int childIndexA = nodeOffset + startIndex;
            
            float dstA = RayBoundingBoxDistance(ray, u_Data.nodes[childIndexA].boundsMin, u_Data.nodes[childIndexA].boundsMax);
            float dstB = RayBoundingBoxDistance(ray, u_Data.nodes[childIndexA + 1].boundsMin, u_Data.nodes[childIndexA + 1].boundsMax);
            
            float dstNear;
            float dstFar;
            int childIndexNear;
            int childIndexFar;
            
            if (dstA <= dstB)
            {
                dstNear = dstA;
                dstFar  = dstB;
                childIndexNear = childIndexA;
                childIndexFar = childIndexA + 1;
            }
            else
            {
                dstNear = dstB;
                dstFar  = dstA;
                childIndexNear = childIndexA + 1;
                childIndexFar = childIndexA;
            }

            if (dstFar < result.dst) stack[stackIndex++] = childIndexFar;
            if (dstNear < result.dst) stack[stackIndex++] = childIndexNear;
        }
    }

    return result;
}

TriangleHitInfo RayTriangle(Ray ray, Triangle tri)
{
    TriangleHitInfo hitInfo;
    hitInfo.dst = 1.0f / 0.0f;
    
    vec3 edgeAB = tri.B - tri.A;
    vec3 edgeAC = tri.C - tri.A;
    vec3 normalVector = cross(edgeAB, edgeAC);
        
    float det = -dot(ray.direction, normalVector);
    if (det == 0) return hitInfo;

    float invDet = 1 / det;
    
    vec3 ao = ray.origin - tri.A;
    
    float dst = dot(ao, normalVector) * invDet;
    //if (dst < 0) return hitInfo;
    
    vec3 dao = cross(ao, ray.direction);
    
    float u = dot(edgeAC, dao) * invDet;
    if (u < 0) return hitInfo;
    
    float v = -dot(edgeAB, dao) * invDet;
    if (v < 0) return hitInfo;
    
    float w = 1 - u - v;
    if (w < 0) return hitInfo;
        
    hitInfo.dst = dst;
    //hitInfo.position = ray.origin + ray.direction * dst;
    //hitInfo.normal = (normalVector * det);
    hitInfo.normal = (tri.normA * w + tri.normB * u + tri.normC * v);
    
    return hitInfo;
}

float RayBoundingBoxDistance(Ray ray, vec3 boxMin, vec3 boxMax)
{
    vec3 tMin = (boxMin - ray.origin) * ray.invDir;
    vec3 tMax = (boxMax - ray.origin) * ray.invDir;
    
    vec3 t1 = min(tMin, tMax);
    vec3 t2 = max(tMin, tMax);
    
    float dstNear = max(max(t1.x, t1.y), t1.z);
    float dstFar = min(min(t2.x, t2.y), t2.z);
    
    return dstFar >= dstNear && dstFar > 0 ? dstNear : 1.0f / 0.0f;
}
