//
//  MeshResource.hpp
//  Game
//
//  Created by Nathan Thurber on 14/10/24.
//


//#include "BVH.hpp"

#include <iostream>

#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#pragma once

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class MeshResource
{
public:
    MeshResource();
    ~MeshResource();
    
    //BVH* bvh = nullptr;
    
    bool loadFromFile(const std::string& filePath);  // Load mesh from file

    const std::vector<Vertex>& getVertices() const { return vertices; }
    const std::vector<uint32_t>& getIndices() const { return indices; }
    
    const glm::vec3 getSupportPoint(glm::vec3 direction);
    const glm::vec3 getAverageSupportPoint(glm::vec3 direction);

private:
    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh);
    
    void processUniqueVertices();
    
    std::vector<Vertex> vertices;   // Vertex data
    std::vector<unsigned int> indices;  // Index data
    
//    std::vector<glm::vec3> uniqueVertices;
};
