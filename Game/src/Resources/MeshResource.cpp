//
//  MeshResource.cpp
//  Game
//
//  Created by Nathan Thurber on 14/10/24.
//

#include "MeshResource.hpp"

#include <glm/gtx/string_cast.hpp>

MeshResource::MeshResource()
{
    
}

MeshResource::~MeshResource()
{
    
}

bool MeshResource::loadFromFile(const std::string& filePath)
{
    Assimp::Importer importer;
    
    const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "[Mesh resource] Assimp error: " << importer.GetErrorString() << std::endl;
        return false;
    }

    processNode(scene->mRootNode, scene);
    
//    processUniqueVertices();
    return true;
}

void MeshResource::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

void MeshResource::processMesh(aiMesh* mesh)
{
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
        
        if (mesh->HasNormals())
        {
            vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
        }

        if (mesh->mTextureCoords[0])
        {
            vertex.texCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
        }
        else
        {
            vertex.texCoords = { 0.0f, 0.0f };  // Default to (0, 0) if no texture coords are available
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
}

//void MeshResource::processUniqueVertices()
//{
//    for (int i = 0; i < vertices.size(); i++)
//    {
//        bool duplicate = false;
//        for (int j = 0; j < vertices.size(); j++)
//        {
//            if (vertices[i].position == vertices[j].position)
//            {
//                duplicate = true;
//                break;
//            }
//        }
//        
//        if (!duplicate)
//        {
//            uniqueVertices.push_back(vertices[i].position);
//        }
//    }
//}

const glm::vec3 MeshResource::getSupportPoint(glm::vec3 direction)
{
    glm::vec3 supportPoint;
    float maxDot = -FLT_MAX;
    
    supportPoint = vertices[0].position;
    maxDot = glm::dot(direction, supportPoint);
    
    for (const Vertex& vertex : vertices)
    {
        float dot = glm::dot(direction, vertex.position);
        if (dot > maxDot)
        {
            maxDot = dot;
            supportPoint = vertex.position;
        }
    }
    return supportPoint;
}

const glm::vec3 MeshResource::getAverageSupportPoint(glm::vec3 direction)
{
    glm::vec3 avgPoint{0.0f};
    std::vector<glm::vec3> supportPoints;
    float maxDot = -FLT_MAX;
    
    for (const Vertex& vertex : vertices)
    {
        float dot = glm::dot(direction, vertex.position);
        if (dot > maxDot)
            maxDot = dot;
    }
    
    for (const Vertex& vertex : vertices)
    {
        float dot = glm::dot(direction, vertex.position);
        if (dot > maxDot - 1e-2)
        {
            supportPoints.push_back(vertex.position);
        }
    }
    
    for (const glm::vec3& vertex : supportPoints)
    {
        avgPoint += vertex;
    }
    
    avgPoint /= supportPoints.size();
    
    return avgPoint;
}
