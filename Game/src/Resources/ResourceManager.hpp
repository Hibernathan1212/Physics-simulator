//
//  ResourceManager.hpp
//  Game
//
//  Created by Nathan Thurber on 14/10/24.
//

#pragma once

#include "MeshResource.hpp"
#include "Material.hpp"
#include "BVH.hpp"

#include <stdio.h>
#include <map>

class ResourceManager
{
public:
    ResourceManager();
    ~ResourceManager();
        
    MeshResource*       getMesh(const std::string& filePath);
    Material*           getMaterial(const std::string& materialPath);
    BVH*                getBVH(const std::string& meshPath);
    
private:
    
    std::unordered_map<std::string, MeshResource*>  m_meshes;
    std::unordered_map<std::string, Material*>      m_materials;
    std::unordered_map<std::string, BVH*>           m_BVHs;
};
