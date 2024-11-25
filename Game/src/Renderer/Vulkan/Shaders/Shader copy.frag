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
    int        nodeOffset;
    int        triOffset;
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
    Object          objects[4];
    Triangle        triangles[100000];
    BVHNode         nodes[200000];
    int             objectCount;
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
    float HitDistance;
    vec3 WorldNormal;

    int ObjectIndex;
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

//HitPayload ClosestHit(Ray ray, float hitDistance, int objectIndex);


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
    
    vec3 color = vec3(0.0f, 0.0f, 0.0f);

    int paths = 12;

    vec3 rayDirection = CalculateRayDirection(gl_FragCoord.x, gl_FragCoord.y);
    
    for (int i = 0; i < paths; i++)
    {
        ray.direction = rayDirection;
        ray.invDir = 1.0f / ray.direction;
        ray.origin = u_Data.cameraData.CameraOrigin;
    
        color += TraceRay(ray, i);
    }
    return vec4(color/paths, 1.0f);
}

vec3 TraceRay(Ray ray, int i)
{
    int bounces = ;

    vec3 light = vec3(0.0f, 0.0f, 0.0f);
    vec3 contribution = vec3(1.0f, 1.0f, 1.0f);

    for (int j = 0; j < bounces; j++)
    {
        HitPayload payload = CalculateRayCollision(ray);
        
        if (payload.HitDistance == 1.0f / 0.0f)
        {
            vec3 skyColor = vec3(0.3f / 500.0f, 0.35f / 500.0f, 0.45f / 500.0f);
            light += skyColor * contribution;
            break;
        }
        
        Material material = u_Data.objects[payload.ObjectIndex].material;
        
        light += material.Emission * contribution;
        contribution *= material.Albedo;
    
        payload.WorldNormal = vec4(u_Data.objects[payload.ObjectIndex].transform * vec4(normalize(payload.WorldNormal), 0)).xyz;
        
        vec3 worldPosition = ray.origin + ray.direction * payload.HitDistance;
        
        ray.origin = worldPosition + payload.WorldNormal * 0.01f;
        //ray.Direction = reflect(ray.Direction, payload.WorldNormal);// + material.Roughness * vec3(rand(payload.WorldPosition*(i+2)/2)));
        //if (material.Roughness == 0.0f)
        //{
        //    ray.direction = reflect(ray.direction, payload.WorldNormal);
        //    continue;
        //}
        
        float rand1 = rand(worldPosition*(i+1));
        float rand2 = rand(worldPosition*2*(i+1));
        float rand3 = rand(worldPosition*3*(i+1));
        
        vec3 diffuseDirection = normalize(0.5f * payload.WorldNormal + vec3(rand1 - rand2, rand2 - rand3, rand3 - rand1));
        vec3 specularDirection = reflect(ray.direction, payload.WorldNormal);// + material.Roughness * vec3(rand(payload.WorldPosition*(i+2)/2)));
        ray.direction = mix(specularDirection, diffuseDirection, material.Roughness);
    }
    return light;
}

HitPayload CalculateRayCollision(Ray ray)
{
    HitPayload result;
    result.HitDistance = 1.0f / 0.0f;
    result.ObjectIndex = -1;
    Ray localRay;

    for (int i = 0; i < u_Data.objectCount; i++)
    {
        localRay.origin = vec4(u_Data.objects[i].invTransform * vec4(ray.origin, 1.0f)).xyz;
        localRay.direction = vec4(u_Data.objects[i].invTransform * vec4(ray.direction, 0.0f)).xyz;
        localRay.invDir = 1.0f / localRay.direction;

        TriangleHitInfo hit = RayTriangleBVH(localRay, result.HitDistance, u_Data.objects[i].nodeOffset, u_Data.objects[i].triOffset);

        // Record closest hit
        if (hit.dst < result.HitDistance)
        {
            result.HitDistance = hit.dst;
            result.WorldNormal = hit.normal;
            
            result.ObjectIndex = i;
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
        BVHNode node = u_Data.nodes[stack[--stackIndex]];
        if (node.triangleCount > 0)
        {
            for (int i = 0; i < node.triangleCount; i++)
            {
                TriangleHitInfo triHitInfo = RayTriangle(ray, u_Data.triangles[triOffset + node.startIndex + i]);

                if (triHitInfo.dst < result.dst)
                {
                    result = triHitInfo;
                }
            }
        }
        else
        {
            int childIndexA = nodeOffset + node.startIndex;
            
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


//HitPayload TraceRay(Ray ray)
//{
//    // (bx^2 + by^2)t^2 + (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
//    // where
//    // a = ray origin
//    // b = ray direction
//    // r = radius
//    // t = hit distance
//
//    int closestSphere = -1;
//    float hitDistance = 1.0f / 0.0;
//    for (int i = 0; i < u_Data.objectData.Objects; i++)
//    {
//        const Sphere sphere = u_Data.objectData.Spheres[i];
//        if (sphere.Radius == 0.0f)
//            continue;
//
//        vec3 origin = ray.Origin - sphere.Position;
//
//        float a = dot(ray.Direction, ray.Direction);
//        float b = 2.0f * dot(origin, ray.Direction);
//        float c = dot(origin, origin) - sphere.Radius * sphere.Radius;
//
//        float discriminant = b * b - 4.0f * a * c;
//
//        if (discriminant < 0.0f)
//            continue;
//
//        float closestT = (-b - sqrt(discriminant)) / (2.0f * a);
//        if (closestT > 0.0f && closestT < hitDistance)
//        {
//            hitDistance = closestT;
//            closestSphere = i;
//        }
//    }
//
//    if (closestSphere < 0)
//        return Miss(ray);
//
//    return ClosestHit(ray, hitDistance, closestSphere);
//}

//HitPayload ClosestHit(Ray ray, float hitDistance, int objectIndex)
//{
//    HitPayload payload;
//    payload.HitDistance = hitDistance;
//    payload.ObjectIndex = objectIndex;
//
//    const Sphere closestSphere = u_Data.objectData.Spheres[objectIndex];
//
//    vec3 origin = ray.Origin - closestSphere.Position;
//
//    payload.WorldPosition = origin + ray.Direction * hitDistance;
//    payload.WorldNormal = normalize(payload.WorldPosition);
//    payload.WorldPosition += closestSphere.Position;
//
//    return payload;
//}
