//
//  EntityComponentSystem.hpp
//  Game
//
//  Created by Nathan Thurber on 14/10/24.
//

#pragma once

#include "Components.hpp"

#include "MeshResource.hpp"
#include "Material.hpp"

#include <glm/glm.hpp>

#include <unordered_map>
#include <vector>

using Entity = unsigned int;

class EntityComponentSystem
{
public:
    Entity createEntity();

    template <typename T>
    void addComponent(Entity entity, T component)
    {
        getComponentArray<T>()[entity] = component;
    }

    template <typename T>
    void removeComponent(Entity entity, T component)
    {
        getComponentArray<T>().erase(entity);
    }

    template <typename T>
    T& getComponent(Entity entity)
    {
        return getComponentArray<T>()[entity];
    }
    
    template <typename T>
    std::unordered_map<Entity, T>& getComponentArray();
    
private:
    Entity nextEntity = 1;

    std::unordered_map<Entity, MeshComponent> m_meshComponents;
    std::unordered_map<Entity, MaterialComponent> m_materialComponents;
    std::unordered_map<Entity, BVHComponent> m_BVHComponents;
    std::unordered_map<Entity, TransformComponent> m_transformComponents;
    std::unordered_map<Entity, PhysicsComponent> m_physicsComponents;

};
