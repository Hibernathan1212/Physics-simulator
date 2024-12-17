//
//  GJK.cpp
//  Game
//
//  Created by Nathan Thurber on 5/12/24.
//

#include "GJKcollision.hpp"

#include "Application/Application.hpp"

extern EntityComponentSystem* ecs;

GJKcollision::GJKcollision(Entity& entity1, Entity& entity2)
: m_entity1(entity1), m_entity2(entity2)
{
    
}

const bool GJKcollision::checkCollision()
{
    bool collided = 0;
    
    const glm::vec3& object1CenterMass = ecs->getComponent<TransformComponent>(m_entity1).translation;
    const glm::vec3& object2CenterMass = ecs->getComponent<TransformComponent>(m_entity2).translation;
    
    const glm::mat4& transform1 = ecs->getComponent<TransformComponent>(m_entity1).getTransform();
    const glm::mat4& transform2 = ecs->getComponent<TransformComponent>(m_entity2).getTransform();

    
    const glm::mat4& invTransform1 = glm::inverse(ecs->getComponent<TransformComponent>(m_entity1).getTransform());
    const glm::mat4& invTransform2 = glm::inverse(ecs->getComponent<TransformComponent>(m_entity2).getTransform());
    
    glm::vec3 direction = object2CenterMass - object1CenterMass;
        
    if (glm::length(direction) > ecs->getComponent<ColliderComponent>(m_entity1).boundingRadius + ecs->getComponent<ColliderComponent>(m_entity2).boundingRadius)
        return collided;
    
//    if (glm::length(direction) == 0)
//        collided = true;
   
    std::vector<glm::vec3> simplex; //newest element is called A, 2nd newest B etc
    
    int iterations = 0;
    
    while(!collided)
    {
        glm::vec3 A = glm::vec3(
        transform1 * glm::vec4(ecs->getComponent<MeshComponent>(m_entity1).mesh->getSupportPoint(invTransform1 * glm::vec4( direction, 0.0f)), 1.0f) -
        transform2 * glm::vec4(ecs->getComponent<MeshComponent>(m_entity2).mesh->getSupportPoint(invTransform2 * glm::vec4(-direction, 0.0f)), 1.0f));
        
        if (glm::dot(A, direction) < 0 || iterations > 100)
        {
            break;
        }
//        simplex.push_back(A);
        
        simplex.insert(simplex.begin(), A);
        
        if (simplex.size() == 0)
        {
            direction = object1CenterMass - object2CenterMass;
        }
        else if (simplex.size() == 1)
        {
            direction = -simplex[0];
        }
        else if (simplex.size() == 2)
        {
            glm::vec3& a = simplex[0];
            glm::vec3& b = simplex[1];
            
            glm::vec3 ab = b - a;
            glm::vec3 ao = -a;
            
            if (glm::dot(ab, ao) > 0)
            {
                direction = glm::cross(glm::cross(ab, ao), ab);
            }
            else
            {
                simplex = { a };
                direction = ao;
            }
        }
        else if (simplex.size() == 3)
        {
            glm::vec3& a = simplex[0];
            glm::vec3& b = simplex[1];
            glm::vec3& c = simplex[2];

            
            glm::vec3 ab = b - a;
            glm::vec3 ac = c - a;
            glm::vec3 ao = -a;
            
            glm::vec3 abc = glm::cross(ab, ac);
            
            if (glm::dot(glm::cross(abc, ac), ao) > 0)
            {
                if (glm::dot(ac, ao) > 0)
                {
                    simplex = { a, c };
                    direction = glm::cross(glm::cross(ac, ao), ac);
                }
                else
                {
                    simplex = { a };
                    direction = ao;
                }
            }
            else if (glm::dot(glm::cross(ab, abc), ao) > 0)
            {
                if (glm::dot(ab, ao) > 0)
                {
                    simplex = { a, b };
                    direction = glm::cross(glm::cross(ab, ao), ab);
                }
                else
                {
                    simplex = { a };
                    direction = ao;
                }
            }
            else
            {
                if (glm::dot(abc, ao))
                {
                    direction = abc;
                }
                else
                {
                    simplex = { a, c, b };
                    direction = -abc;
                }
            }
        }
        else if (simplex.size() == 4)
        {
            glm::vec3& a = simplex[0];
            glm::vec3& b = simplex[1];
            glm::vec3& c = simplex[2];
            glm::vec3& d = simplex[3];

            glm::vec3 ao = -a;
            glm::vec3 ab = b - a;
            glm::vec3 ac = c - a;
            glm::vec3 ad = d - a;
            
            glm::vec3 abc = glm::cross(ab, ac);
            glm::vec3 acd = glm::cross(ac, ad);
            glm::vec3 adb = glm::cross(ad, ab);

            if (glm::dot(abc, ao) > -1e-2)
            {
                simplex = { a, b, c };
                direction = abc;
//                continue;
            }
            else if (glm::dot(acd, ao) > -1e-2)
            {
                simplex = { a, c, d };
                direction = acd;
//                continue;
            }
            else if (glm::dot(adb, ao) > -1e-2)
            {
                simplex = { a, d, b };
                direction = adb;
//                continue;
            }
            else
            {
                collided = true;
            }
        }
        
        iterations++;
    }
        
    
    
    
    if (collided)
    {
        //throw std::runtime_error("error");
        
        glm::vec3 direction = object2CenterMass - object1CenterMass;
        
        //glm::vec3 collisionNormal; //collision normal
        
        std::vector<glm::vec3> polytope(simplex.begin(), simplex.end());
        
        std::vector<size_t> faces = {
            0, 1, 2,
            0, 3, 1,
            0, 2, 3,
            1, 3, 2
        };
        
        // list: vec4(normal, distance), index: min distance
        auto [normals, minFace] = GetFaceNormals(polytope, faces);
        
        //GetFaceNormals(polytope, faces);
        
        glm::vec3 minNormal;
        float minDistance = FLT_MAX;
        
        iterations = 0;
        while (minDistance == FLT_MAX)
        {
            minNormal       = glm::vec3(normals[minFace]);
            minDistance     = normals[minFace].w;
            
            if (iterations++ > 1000)
            {
                break;
            }
            
            glm::vec3 support = glm::vec3(
            transform1 * glm::vec4(ecs->getComponent<MeshComponent>(m_entity1).mesh->getSupportPoint(invTransform1 * glm::vec4( minNormal, 0.0f)), 1.0f) -
            transform2 * glm::vec4(ecs->getComponent<MeshComponent>(m_entity2).mesh->getSupportPoint(invTransform2 * glm::vec4(-minNormal, 0.0f)), 1.0f));

            
            float sDistance = glm::dot(minNormal, support);
            
//            if (sDistance - minDistance < 1e-4)
//            {
//                break;
//            }
//            
//            polytope.push_back(support);
//            
//            for (size_t i = 0; i < normals.size(); i++)
//            {
//                if (glm::dot(support - polytope[faces[i * 3]], glm::vec3(normals[i])) > 0)
//                {
//                    continue;
//                }
//                
//            }
            
            
            if (glm::abs(sDistance - minDistance) > 1e-6)
            {
                minDistance = FLT_MAX;

                std::vector<std::pair<size_t, size_t>> uniqueEdges;
                
                for (size_t i = 0; i < normals.size(); i++)
                {
                    if (glm::dot(glm::vec3(normals[i]), support) > 0)
                    {
                        size_t f = i * 3;
                        
                        AddIfUniqueEdge(uniqueEdges, faces, f,     f + 1);
                        AddIfUniqueEdge(uniqueEdges, faces, f + 1, f + 2);
                        AddIfUniqueEdge(uniqueEdges, faces, f + 2, f    );
                        
                        faces[f + 2] = faces.back(); faces.pop_back();
                        faces[f + 1] = faces.back(); faces.pop_back();
                        faces[f    ] = faces.back(); faces.pop_back();
                        
                        normals[i] = normals.back(); normals.pop_back();
                        
                        i--;
                    }
                }
                
                if (uniqueEdges.size() == 0)
                {
                    break;
                }

                std::vector<size_t> newFaces;
                for (auto [edge1, edge2] : uniqueEdges)
                {
                    newFaces.push_back(edge1);
                    newFaces.push_back(edge2);
                    newFaces.push_back(polytope.size());
                }

                polytope.push_back(support);
                
                auto [newNormals, newMinFace] = GetFaceNormals(polytope, newFaces);
                
                float newMinDistance = FLT_MAX;
                for (size_t i = 0; i < normals.size(); i++)
                {
                    if (normals[i].w < newMinDistance)
                    {
                        newMinDistance = normals[i].w;
                        minFace = i;
                    }
                }

                if (newNormals[newMinFace].w < newMinDistance)
                {
                    minFace = newMinFace + normals.size();
                }
                
                faces  .insert(faces  .end(), newFaces  .begin(), newFaces  .end());
                normals.insert(normals.end(), newNormals.begin(), newNormals.end());
            }
        }
        
        if (minDistance == FLT_MAX)
        {
            return false;
        }
        
        if (glm::dot(minNormal, object1CenterMass - object2CenterMass) < 0)
        {
            minNormal *= -1;
        }
        
        direction = object2CenterMass - object1CenterMass;
        
        const glm::vec3 posA = transform1 * glm::vec4(ecs->getComponent<MeshComponent>(m_entity1).mesh->getAverageSupportPoint(invTransform1 * glm::vec4( direction, 0.0f)), 1.0f);
        
        const glm::vec3 posB = transform2 * glm::vec4(ecs->getComponent<MeshComponent>(m_entity2).mesh->getAverageSupportPoint(invTransform2 * glm::vec4(-direction, 0.0f)), 1.0f);
        
        if (glm::dot(direction, glm::normalize(posA)) > glm::dot(-direction, glm::normalize(posB)))
        {
            m_collisionPosition = posA;
        }
        else
        {
            m_collisionPosition = posB;
        }
        
        m_collisionNormal = minNormal;
        m_penetrationDepth = minDistance + 1e-3;
        
        return true;
    }
    
    return false;
}

const glm::vec3 GJKcollision::support(const Entity& a, const glm::mat4& transform1, const glm::mat4& invTransform1, const glm::vec3& direction)
{
    return glm::vec3(
    transform1 * glm::vec4(ecs->getComponent<MeshComponent>(m_entity1).mesh->getSupportPoint(invTransform1 * glm::vec4( direction, 0.0f)), 1.0f));
}

std::pair<std::vector<glm::vec4>, size_t> GJKcollision::GetFaceNormals(std::vector<glm::vec3>& polytope, std::vector<size_t>& faces)
{
    std::vector<glm::vec4> normals;
    size_t minTriangle = 0;
    float  minDistance = FLT_MAX;

    for (int i = 0; i < faces.size(); i += 3)
    {
        glm::vec3 a = polytope[faces[i    ]];
        glm::vec3 b = polytope[faces[i + 1]];
        glm::vec3 c = polytope[faces[i + 2]];

        glm::vec3 normal = glm::normalize(glm::cross(b - a, c - a));
        float distance = glm::dot(normal, a);

        if (distance < 0)
        {
            normal   *= -1;
            distance *= -1;
        }

        normals.emplace_back(normal, distance);

        if (distance < minDistance)
        {
            minTriangle = i / 3;
            minDistance = distance;
        }
    }

    return { normals, minTriangle };
}

void GJKcollision::AddIfUniqueEdge(std::vector<std::pair<size_t, size_t>>& edges, const std::vector<size_t>& faces, size_t a, size_t b)
{
    auto reverse = std::find(edges.begin(), edges.end(), std::make_pair(faces[b], faces[a]));
 
    if (reverse != edges.end())
    {
        edges.erase(reverse);
    }
    else
    {
        edges.emplace_back(faces[a], faces[b]);
    }
}
