//
//  MeshResource.hpp
//  Game
//
//  Created by Nathan Thurber on 14/10/24.
//

#pragma once

#include <iostream>

#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
    
    bool loadFromFile(const std::string& filePath);  // Load mesh from file

    const std::vector<Vertex>& getVertices() const { return vertices; }
    const std::vector<uint32_t>& getIndices() const { return indices; }
private:
    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh);
    
    std::vector<Vertex> vertices;   // Vertex data
    std::vector<unsigned int> indices;  // Index data
};
