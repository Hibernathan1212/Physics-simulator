//
//  Collisions.hpp
//  Game
//
//  Created by Nathan Thurber on 5/12/24.
//

#pragma once

#include "BVHcollision.hpp"
#include "GJKcollision.hpp"

#include "Resources/EntityComponentSystem.hpp"

#include <glm/glm.hpp>

#include <vector>

#define BVHdepth 0 //-1 = only BVH no GJK

extern EntityComponentSystem* ecs;


class Collision
{
public:
    Collision(Entity& entity1, Entity& entity2);
    
private:
    const bool detectCollision();
    
    void solveCollision();
    
    const bool narrowPhase();
    
    const bool broadPhase(const uint32_t depth);
    
private:
    Entity m_entity1;
    Entity m_entity2;
    
    std::vector<glm::vec3> simplex;
    
    glm::vec3 m_collisionPosition;
    glm::vec3 m_collisionNormal;
    float m_penetrationDepth;

    glm::vec3 m_object1CenterMass;
    glm::vec3 m_object2CenterMass;
    
};

