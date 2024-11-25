//
//  ResourceManager.cpp
//  Game
//
//  Created by Nathan Thurber on 14/10/24.
//

#include "ResourceManager.hpp"

ResourceManager::ResourceManager()
{
    
}

MeshResource* ResourceManager::getMesh(const std::string& filePath)
{
    if (m_meshes.find(filePath) == m_meshes.end())
    {
        MeshResource* mesh = new MeshResource();
        if (mesh->loadFromFile(filePath))
        {
            m_meshes[filePath] = mesh;
        }
        else
        {
            delete mesh;
            return nullptr;
        }
    }
    return m_meshes[filePath];
}

Material* ResourceManager::getMaterial(const std::string& materialName)
{
    if (m_materials.find(materialName) == m_materials.end())
    {
        Material* material = new Material();
        m_materials[materialName] = material;
    }
    return m_materials[materialName];
}

BVH* ResourceManager::getBVH(const std::string& filePath)
{
    if (m_BVHs.find(filePath) == m_BVHs.end())
    {
        if (m_meshes.find(filePath) == m_meshes.end())
        {
            MeshResource* mesh = new MeshResource();
            if (mesh->loadFromFile(filePath))
            {
                m_meshes[filePath] = mesh;
            }
            else
            {
                delete mesh;
                return nullptr;
            }
        }
        BVH* bvh = new BVH(*m_meshes[filePath]);
        m_BVHs[filePath] = bvh;
    }
    return m_BVHs[filePath];
}


ResourceManager::~ResourceManager()
{
    for (auto& pair : m_meshes)
        delete pair.second;
    
    for (auto& pair : m_materials)
        delete pair.second;
}
