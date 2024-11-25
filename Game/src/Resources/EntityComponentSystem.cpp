//
//  EntityComponentSystem.cpp
//  Game
//
//  Created by Nathan Thurber on 14/10/24.
//

#include "EntityComponentSystem.hpp"

Entity EntityComponentSystem::createEntity()
{
    Entity entity = nextEntity++;
    return entity;
}

template <>
std::unordered_map<Entity, MeshComponent>& EntityComponentSystem::getComponentArray<MeshComponent>()
{
    return m_meshComponents;
}

template <>
std::unordered_map<Entity, MaterialComponent>& EntityComponentSystem::getComponentArray<MaterialComponent>()
{
    return m_materialComponents;
}

template <>
std::unordered_map<Entity, BVHComponent>& EntityComponentSystem::getComponentArray<BVHComponent>()
{
    return m_BVHComponents;
}

template <>
std::unordered_map<Entity, TransformComponent>& EntityComponentSystem::getComponentArray<TransformComponent>()
{
    return m_transformComponents;
}

template <>
std::unordered_map<Entity, PhysicsComponent>& EntityComponentSystem::getComponentArray<PhysicsComponent>()
{
    return m_physicsComponents;
}
