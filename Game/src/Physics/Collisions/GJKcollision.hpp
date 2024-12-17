//
//  GJK.hpp
//  Game
//
//  Created by Nathan Thurber on 5/12/24.
//

#pragma once

#include "Resources/EntityComponentSystem.hpp"

#include <glm/glm.hpp>

class GJKcollision
{
public:
    GJKcollision(Entity& entity1, Entity& entity2);
    
    const bool checkCollision();
    
    const inline glm::vec3 getCollisionPosition() { return m_collisionPosition; };
    
    const inline glm::vec3 getCollisionNormal() { return m_collisionNormal; };
    
    const inline float getPenetrationDepth() { return m_penetrationDepth; };
    
private:
    
    std::pair<std::vector<glm::vec4>, size_t> GetFaceNormals(std::vector<glm::vec3>& polytope, std::vector<size_t>& faces);
    
    void AddIfUniqueEdge(std::vector<std::pair<size_t, size_t>>& edges, const std::vector<size_t>& faces, size_t a, size_t b);
    
    const glm::vec3 support(const Entity& a, const glm::mat4& transform1, const glm::mat4& invTransform1, const glm::vec3& direction);

private:
    
    glm::vec3 m_collisionPosition;
    glm::vec3 m_collisionNormal;
    float m_penetrationDepth;
    
    Entity m_entity1;
    Entity m_entity2;
};

struct Face
{
    glm::vec3 vertices[3];
    glm::vec3 normal;
    float distance; // Distance from the origin along the normal

    Face(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
    {
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
        normal = glm::normalize(glm::cross(b - a, c - a));
        distance = glm::dot(normal, a);
        
        if (distance < 0)
        {
            normal = -normal; // Ensure the normal points outward
            distance = -distance;
        }
    }
};
