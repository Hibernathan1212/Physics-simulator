//
//  Object.hpp
//  Game
//
//  Created by Nathan Thurber on 26/5/24.
//

#pragma once

#include <glm/glm.hpp>
#include <vector>

#include <iostream>
#include <glm/gtx/string_cast.hpp>

//struct Material
//{
//    alignas(16) glm::vec3 Albedo{1.0f};
//    alignas(4)  float Roughness = 1.0f;
//    alignas(16) glm::vec3 Emission = glm::vec3(0.0f, 0.0f, 0.0f) * 0.0f; //Emission color * Emission power
//    alignas(4)  float Restitution = 1.0f; //from 0 to 1, 1 being no energy loss in collision and 0 being all energy lost
//};

//struct Sphere
//{
//    glm::vec3 Position{0.0f};
//    glm::vec3 Velocity{0.0f};
//    glm::vec3 Acceleration{0.0f};
//    float Mass = 1.0f;
//
//    float Radius = 0.0f;
//
//    Material material;
//};

struct Triangle
{
    glm::vec3 A{0.0f};
    glm::vec3 B{0.0f};
    glm::vec3 C{0.0f};

    glm::vec3 Velocity{0.0f};
    //glm::vec3 PreviousPosition{0.0f};
    glm::vec3 Acceleration{0.0f};
    float Mass = 1.0f;
};

struct Object
{
    glm::vec3 Velocity{0.0f};
    glm::vec3 Acceleration{0.0f};
    glm::vec3 angularVelocity{0.0f};
    glm::vec3 angularAcceleration{0.0f};
    
    glm::vec3 Force{0.0f};
    
    //void updatePosition(float dt)
    //{
    //    Acceleration += force/Mass;
    //
    //    Velocity = Velocity + Acceleration * dt;
    //    for (glm::vec3& Vertex : Vertices)
    //        Vertex = Vertex + Velocity * dt;
    //
    //    CenterOfMass = CenterOfMass + Velocity * dt;
    //
    //    Acceleration = {};
    //    force = {};
    //}
    
    void updateRotations(float dt)
    {
        angularVelocity = angularVelocity + angularAcceleration * dt;
        
        glm::vec3 a = angularVelocity * dt; //angle
        
        glm::mat3 rotationMatrix = {
            glm::cos(a.z)*glm::cos(a.y),    glm::cos(a.z)*glm::sin(a.y)*glm::sin(-a.x) - glm::sin(a.z)*glm::cos(-a.x),    glm::cos(a.z)*glm::sin(a.y)*glm::cos(-a.x) + glm::sin(a.z)*glm::sin(-a.x),
            glm::sin(a.z)*glm::cos(a.y),    glm::sin(a.z)*glm::sin(a.y)*glm::sin(-a.x) + glm::cos(a.z)*glm::cos(-a.x),    glm::sin(a.z)*glm::sin(a.y)*glm::cos(-a.x) - glm::cos(a.z)*glm::sin(-a.x),
            -glm::sin(a.y),                 glm::cos(a.y)*glm::sin(-a.x),                                                glm::cos(a.y)*glm::cos(-a.x)
        };
        
        
        for (glm::vec3& Vertex : Vertices)
            Vertex = rotationMatrix * (Vertex - CenterOfMass) + CenterOfMass;
        
        angularAcceleration = {};
    }
    
    void accelerate(glm::vec3 acc)
    {
        Acceleration += acc;
    }
    
    void applyForce(glm::vec3 force)
    {
        Force += force;
    }
    
    void applyForce(glm::vec3 force, glm::vec3 position)
    {
        
    }
    
    glm::vec3 getSupportPoint(glm::vec3 direction)
    {
        glm::vec3 supportPoint;
        float maxDot = -MAXFLOAT;
        for (glm::vec3 Vertex : Vertices)
        {
            float dot = glm::dot(direction, Vertex - CenterOfMass);
            if (dot > maxDot)
            {
                maxDot = dot;
                supportPoint = Vertex;
            }
        }
        return supportPoint;
    }
    
    glm::vec3 getAverageSupportPoint(glm::vec3 direction)
    {
        glm::vec3 avgPoint{0.0f};
        std::vector<glm::vec3> avg;
        float maxDot = -MAXFLOAT;
        //std::cout << glm::to_string(CenterOfMass) << std::endl;
        //std::cout << glm::to_string(direction) << std::endl;
        for (glm::vec3& Vertex : Vertices)
        {
            //std::cout << glm::to_string(Vertex) << std::endl;
            //std::cout << glm::to_string(Vertex - CenterOfMass) << std::endl;
            float dot = glm::dot(direction, Vertex - CenterOfMass);
            if (dot > maxDot)
                maxDot = dot;
        }
        
        for (glm::vec3& Vertex : Vertices)
        {
            float dot = glm::dot(direction, Vertex - CenterOfMass);
            if (dot == maxDot)
            {
                avg.push_back(Vertex);
            }
        }
        
        for (glm::vec3& Vertex : avg)
        {
            avgPoint.x = avgPoint.x + Vertex.x;
            avgPoint.y = avgPoint.y + Vertex.y;
            avgPoint.z = avgPoint.z + Vertex.z;
        }
        
        avgPoint.x = avgPoint.x / avg.size();
        avgPoint.y = avgPoint.y / avg.size();
        avgPoint.z = avgPoint.z / avg.size();
        
        return avgPoint;
    }
    
    float getMaxDot(glm::vec3 direction)
    {
        float maxDot = -MAXFLOAT;
        for (glm::vec3 Vertex : Vertices)
        {
            float dot = glm::dot(direction, glm::normalize(Vertex - CenterOfMass));
            if (dot > maxDot)
                maxDot = dot;
        }
        return maxDot;
    }
    
    float Mass = 1.0f;
    glm::vec3 CenterOfMass{0.0f};
    float boundingRadius; //distance to furthest vertex for optimization with collisions
    glm::mat3 inertiaTensor; /*
                            [inertia in x axis, 0.0f, 0.0f,
                            0.0f, inertia in y axis, 0.0f,
                            0.0f, 0.0f, inertia in z axis]
                            */
    
    std::vector<glm::vec3> Vertices;
    std::vector<glm::ivec3> Indices;
    //Material material;
};
