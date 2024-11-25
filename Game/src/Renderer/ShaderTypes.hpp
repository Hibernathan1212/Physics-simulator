//
//  ShaderTypes.hpp
//  Game
//
//  Created by Nathan Thurber on 20/5/24.
//

#pragma once

struct u_CameraData
{
    alignas(16) glm::vec3 CameraOrigin = glm::vec3(0.0f, 0.0f, 0.0f);
    alignas(4) float ViewportWidth = 1600;
    alignas(4) float ViewportHeight = 900;
    alignas(16) glm::mat4 Projection{ 1.0f };
    alignas(16) glm::mat4 View{ 1.0f };
    alignas(16) glm::mat4 InverseProjection{ 1.0f };
    alignas(16) glm::mat4 InverseView{ 1.0f };
};

struct u_Material
{
    alignas(16) glm::vec3 Albedo{1.0f};
    alignas(4)  float Roughness = 1.0f;
    alignas(16) glm::vec3 Emission = glm::vec3(0.0f, 0.0f, 0.0f) * 0.0f; //Emission color * Emission power
    //float4 specularColour;
    //float smoothness;
    //float specularProbability;
};

//struct u_Sphere
//{
//    glm::vec3 Position{0.0f};
//    float Radius = 0.0f;
//
//    u_Material material;
//};
//
//struct u_Triangle
//{
//    alignas(16) glm::vec3 A{0.0f};
//    alignas(16) glm::vec3 B{0.0f};
//    alignas(16) glm::vec3 C{0.0f};
//
//    u_Material material;
//};


struct u_Triangle
{
    alignas(16) glm::vec3 posA, posB, posC;
    alignas(16) glm::vec3 normA, normB, normC;
};

struct u_BVHNode
{
    alignas(16) glm::vec3 boundsMin;
    alignas(16) glm::vec3 boundsMax;
    alignas(4) int32_t startIndex;     // index refers to triangles if is leaf node (triangleCount > 0) otherwise it is the index of the first child node
    alignas(4) int32_t triangleCount;
};

//struct u_Object
//{
//    alignas(16) glm::vec4 Vertices[10000];
//    alignas(16) glm::ivec4 Indices[10000];
//    u_Material material;
//    uint32_t numberOfIndices;
//};

struct u_Object
{
    alignas(4) int32_t     nodeOffset;
    alignas(4) int32_t     triOffset;
    alignas(16) glm::mat4   transform{1.0f};
    alignas(16) glm::mat4   invTransform{1.0f};
    u_Material  material;
};

struct UniformData
{
    u_Object     objects[2];
    u_Triangle   triangles[3000000];
    u_BVHNode    nodes[6000000];
    
    alignas(4) uint32_t    lightObjIndices;
    //alignas(4) int32_t     triangleCount;
    alignas(4) uint32_t    objectCount;
    alignas(4) uint32_t    lightCount;
    
    u_CameraData cameraData;
};

