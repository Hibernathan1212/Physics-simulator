//
//  Collisions.cpp
//  Game
//
//  Created by Nathan Thurber on 5/12/24.
//

#include "Collisions.hpp"
#include <glm/gtx/string_cast.hpp>

Collision::Collision(Entity& entity1, Entity& entity2)
: m_entity1(entity1), m_entity2(entity2), m_object1CenterMass(ecs->getComponent<TransformComponent>(entity1).translation), m_object2CenterMass(ecs->getComponent<TransformComponent>(entity2).translation)
{
    if (detectCollision())
    {
        solveCollision();
    }
}

const bool Collision::detectCollision()
{
    bool collided = false;
    
//#if BVHdepth != -1
//    if (broadPhase(BVHdepth))
//    {
//        collided = narrowPhase();
//    }
//#else
//    collided = broadPhase(BVHdepth);
//
//#endif
    collided = narrowPhase();
    
    return collided;
}

void Collision::solveCollision()
{
    glm::vec3 r1 = m_collisionPosition - m_object1CenterMass;
    
    glm::vec3 r2 = m_collisionPosition - m_object2CenterMass;

    m_collisionNormal = glm::dot(r1, m_collisionNormal) ? m_collisionNormal : -m_collisionNormal;
    
    PhysicsComponent& object1_physics = ecs->getComponent<PhysicsComponent>(m_entity1);
    ColliderComponent& object1_collide = ecs->getComponent<ColliderComponent>(m_entity1);
    
    PhysicsComponent& object2_physics = ecs->getComponent<PhysicsComponent>(m_entity2);
    ColliderComponent& object2_collide = ecs->getComponent<ColliderComponent>(m_entity2);
    
    glm::vec3 relativeVelocity = (object2_physics.velocity + glm::cross(object2_physics.angularVelocity, r2)) - (object1_physics.velocity + glm::cross(object1_physics.angularVelocity, r1));
            
    float e = (ecs->getComponent<MaterialComponent>(m_entity1).material->getRestitution() + ecs->getComponent<MaterialComponent>(m_entity2).material->getRestitution()) / 2.0f; //average restitution
    
    float impulseMagnitude = -(1 + e) * glm::dot(relativeVelocity, m_collisionNormal) / ( 1/object1_physics.mass + 1/object2_physics.mass + glm::dot(glm::cross(glm::inverse(object1_collide.inertiaTensor) * (glm::cross(r1, m_collisionNormal)), r1) + glm::cross(glm::inverse(object2_collide.inertiaTensor) * (glm::cross(r2, m_collisionNormal)), r2), m_collisionNormal) );
    
//    glm::vec3 impulseVector = impulseMagnitude * m_collisionNormal;
    
    object1_physics.velocity -= (impulseMagnitude/object1_physics.mass) * m_collisionNormal;
    object2_physics.velocity += (impulseMagnitude/object2_physics.mass) * m_collisionNormal;

    object1_physics.angularVelocity -= impulseMagnitude * glm::inverse(object1_collide.inertiaTensor) * glm::cross(r1, m_collisionNormal);
    object2_physics.angularVelocity += impulseMagnitude * glm::inverse(object2_collide.inertiaTensor) * glm::cross(r2, m_collisionNormal);
    
    ecs->getComponent<TransformComponent>(m_entity1).translation += m_collisionNormal * 2.0f * m_penetrationDepth * ((object2_physics.mass)/(object1_physics.mass + object2_physics.mass));
    
//    std::cout << glm::to_string(ecs->getComponent<TransformComponent>(m_entity1).translation + m_collisionNormal * 2.0f * m_penetrationDepth * ((object1_physics.mass)/(object1_physics.mass + object2_physics.mass))) << std::endl;
    
    ecs->getComponent<TransformComponent>(m_entity2).translation -= m_collisionNormal * 2.0f * m_penetrationDepth * ((object1_physics.mass)/(object1_physics.mass + object2_physics.mass));
    
    //throw std::runtime_error("Collided");
    
    //std::cout << impulseMagnitude << std::endl;
}


const bool Collision::broadPhase(const uint32_t depth)
{
    BVHcollision BVH(m_entity1, m_entity2);
    
#if BVHdepth == -1
    if (BVH.checkCollision(depth))
    {
        m_collisionPosition = BVH.getCollisionPosition();
        m_collisionNormal = BVH.getCollisionNormal();
        m_penetrationDistance = BVH.getPenetrationDistance();
        return true;
    }
    return false;
    
#else
    //return BVH.checkCollision(depth);
    return false;
#endif
}

const bool Collision::narrowPhase()
{
    GJKcollision GJK(m_entity1, m_entity2);
    
    if (GJK.checkCollision())
    {
        m_collisionPosition = GJK.getCollisionPosition();
        m_collisionNormal = GJK.getCollisionNormal();
        m_penetrationDepth = GJK.getPenetrationDepth();
        return true;
    }
    
    return false;
}
