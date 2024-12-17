//
//  Physics.cpp
//  Game
//
//  Created by Nathan Thurber on 25/5/24.
//

#include "Physics.hpp"
#include "Collisions.hpp"

#include "Utils/Timer.hpp"

#include <thread>

extern bool g_ApplicationRunning;

Physics::Physics()
{
    
}

Physics::~Physics()
{
    
}

void Physics::update()
{
    Timer timer;
        
    while (g_ApplicationRunning)
    {
        double time = timer.Elapsed();
        //timer.Reset();

        m_updateTime = time - m_lastUpdateTime;

        //m_timeStep = glm::min<double>(m_updateTime, 0.0333f); //0.0333f is slowest it will run (30fps)
        //m_timeStep = 0.00001;
        
        //std::cout << m_updateTime << std::endl;
        
        updatePhysics();
        updateBodies(m_timeStep);
        

        
        m_lastUpdateTime = time;
    }
}

void Physics::updatePhysics()
{
    for (auto& pair : ecs->getComponentArray<PhysicsComponent>())
    {
        Entity entity = pair.first;
        
        if (ecs->getComponentArray<ColliderComponent>().count(entity))
        {
            for (auto& secondPair : ecs->getComponentArray<ColliderComponent>())
            {
                Entity secondEntity = secondPair.first;
                if (entity <= secondEntity)
                    continue;
                                
                Collision(entity, secondEntity);
                
                
                //solveCollisions(entity, secondEntity);
            }
        }
        
        for (auto& secondPair : ecs->getComponentArray<PhysicsComponent>())
        {
            Entity secondEntity = secondPair.first;
            if (entity == secondEntity)
                continue;
            
            ecs->getComponent<PhysicsComponent>(entity);
            
            applyNewtonianGravity(entity, secondEntity);
        }
    }
}

void Physics::updateBodies(double dt)
{
    for (auto& pair : ecs->getComponentArray<PhysicsComponent>())
    {
        Entity entity = pair.first;

        PhysicsComponent& physComp = ecs->getComponent<PhysicsComponent>(entity);
        
        physComp.acceleration += physComp.force / physComp.mass;
        physComp.velocity += physComp.acceleration * (float) dt;
        
        ecs->getComponent<TransformComponent>(entity).translation += physComp.velocity * (float) dt;
        
//        physComp.angularAcceleration +=
        physComp.angularVelocity += physComp.angularAcceleration * (float) dt;
        
        ecs->getComponent<TransformComponent>(entity).rotation += physComp.angularVelocity * (float) dt;

        physComp.angularAcceleration = {};
        physComp.acceleration = {};
        physComp.force = {};
    }
}

//void Physics::applyForce(Object& object, glm::vec3 Force, glm::vec3 forcePosition)
//{
//    object.applyForce(Force, forcePosition);
//}


//void Physics::updatePosition(Entity object, double dt)
//{
//
//}

//void Physics::updateRotation(Object &object, double dt)
//{
//    object.angularVelocity = object.angularVelocity + object.angularAcceleration * (float) dt;
//    
//    glm::vec3 a = object.angularVelocity * (float) dt; //angle
//    
//    glm::mat3 rotationMatrix = {
//        glm::cos(a.z)*glm::cos(a.y),    glm::cos(a.z)*glm::sin(a.y)*glm::sin(a.x) - glm::sin(a.z)*glm::cos(a.x),    glm::cos(a.z)*glm::sin(a.y)*glm::cos(a.x) + glm::sin(a.z)*glm::sin(a.x),
//        glm::sin(a.z)*glm::cos(a.y),    glm::sin(a.z)*glm::sin(a.y)*glm::sin(a.x) + glm::cos(a.z)*glm::cos(a.x),    glm::sin(a.z)*glm::sin(a.y)*glm::cos(a.x) - glm::cos(a.z)*glm::sin(a.x),
//        -glm::sin(a.y),                 glm::cos(a.y)*glm::sin(a.x),                                                glm::cos(a.y)*glm::cos(a.x)
//    };
//    
//    for (glm::vec3& Vertex : object.Vertices)
//    {
//        Vertex = rotationMatrix * (Vertex - object.CenterOfMass) + object.CenterOfMass;
//    }
//    
//    object.angularAcceleration = {};
//}

void Physics::applyNewtonianGravity(const Entity object1, const Entity object2)
{
    glm::vec3 direction = (ecs->getComponent<TransformComponent>(object1).translation - ecs->getComponent<TransformComponent>(object2).translation);
    double r = glm::length(direction); //(glm::length(direction) > Object1.boundingRadius + Object2.boundingRadius) ? glm::length(direction) : 100.0f ;
    PhysicsComponent& comp1 = ecs->getComponent<PhysicsComponent>(object1);
    PhysicsComponent& comp2 = ecs->getComponent<PhysicsComponent>(object2);

    double forceMagnitude = G * (comp1.mass * comp2.mass)/(r*r);
    comp1.force += (-glm::normalize(direction) * (float) forceMagnitude);
    comp2.force += (glm::normalize(direction) * (float) forceMagnitude);
}
