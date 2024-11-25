//
//  Physics.cpp
//  Game
//
//  Created by Nathan Thurber on 25/5/24.
//

#include "Physics.hpp"

Physics::Physics()
{
    
}

Physics::~Physics()
{
    
}

void Physics::OnUpdate()
{
    
}

void Physics::applyForce(Object& object, glm::vec3 Force, glm::vec3 forcePosition)
{
    object.applyForce(Force, forcePosition);
}


void Physics::updatePosition(Object& object, double dt)
{
    object.Acceleration += object.Force / object.Mass;
    
    object.Velocity = object.Velocity + object.Acceleration * (float) dt;
    
    for (glm::vec3& Vertex : object.Vertices)
        Vertex = Vertex + object.Velocity * (float) dt;
    
    object.CenterOfMass = object.CenterOfMass + object.Velocity * (float) dt;
    
    object.Acceleration = {};
    
    object.Force = {};
}

void Physics::updateRotation(Object &object, double dt)
{
    object.angularVelocity = object.angularVelocity + object.angularAcceleration * (float) dt;
    
    glm::vec3 a = object.angularVelocity * (float) dt; //angle
    
    glm::mat3 rotationMatrix = {
        glm::cos(a.z)*glm::cos(a.y),    glm::cos(a.z)*glm::sin(a.y)*glm::sin(a.x) - glm::sin(a.z)*glm::cos(a.x),    glm::cos(a.z)*glm::sin(a.y)*glm::cos(a.x) + glm::sin(a.z)*glm::sin(a.x),
        glm::sin(a.z)*glm::cos(a.y),    glm::sin(a.z)*glm::sin(a.y)*glm::sin(a.x) + glm::cos(a.z)*glm::cos(a.x),    glm::sin(a.z)*glm::sin(a.y)*glm::cos(a.x) - glm::cos(a.z)*glm::sin(a.x),
        -glm::sin(a.y),                 glm::cos(a.y)*glm::sin(a.x),                                                glm::cos(a.y)*glm::cos(a.x)
    };
    
    
    for (glm::vec3& Vertex : object.Vertices)
    {
        Vertex = rotationMatrix * (Vertex - object.CenterOfMass) + object.CenterOfMass;
    }
    
    object.angularAcceleration = {};
}

void Physics::solveCollisions(Object& Object1, Object& Object2)
{
    glm::vec3 direction = Object2.CenterOfMass - Object1.CenterOfMass;
    
    bool Collided = 0;
    
    if (glm::length(direction) > Object1.boundingRadius + Object2.boundingRadius)
        return;
    
    if (glm::length(direction) == 0)
        Collided = 1;
   
    std::vector<glm::vec3> simplex; //newest element is called A, 2nd newest B etc
    
    int iterations = 0;
    while(!Collided && iterations < (Object1.Vertices.size() + Object2.Vertices.size()) * 2)
    {
        if (simplex.size() == 0)
        {
            direction = Object1.CenterOfMass - Object2.CenterOfMass;
        }
        else if (simplex.size() == 1)
        {
            direction *= -1;
        }
        else if (simplex.size() == 2)
        {
            glm::vec3 AB = simplex[1] - simplex[0];
            direction = glm::cross(AB, glm::cross(-simplex[1], AB));
            //direction = glm::cross(simplex[0] - simplex[1], glm::cross(-simplex[1], simplex[0] - simplex[1]));
        }
        else if (simplex.size() == 3)
        {
            glm::vec3 AO = glm::vec3{0.0f} - simplex[0];
            glm::vec3 AB = simplex[1] - simplex[0];
            glm::vec3 AC = simplex[2] - simplex[0];
            
            direction = glm::cross(AB, AC);
            
            if (glm::dot(direction, AO) < 0)
                direction *= -1;
        }
        else if (simplex.size() == 4)
        {
            glm::vec3 DA = simplex[3] - simplex[0];
            glm::vec3 DB = simplex[3] - simplex[1];
            glm::vec3 DC = simplex[3] - simplex[2];

            glm::vec3 DO = glm::vec3{0.0f} - simplex[3];
            
            glm::vec3 ADBperp = -glm::cross(DA, DB);
            glm::vec3 BDCperp = -glm::cross(DB, DC);
            glm::vec3 CDAperp = -glm::cross(DC, DA);

            if (glm::dot(ADBperp, DO) > 0)
            {
                simplex.erase(simplex.begin() + 2);
                direction = ADBperp;
            }
            else if (glm::dot(BDCperp, DO) > 0)
            {
                simplex.erase(simplex.begin());
                direction = BDCperp;

            }
            else if (glm::dot(CDAperp, DO) > 0)
            {
                simplex.erase(simplex.begin() + 1);
                direction = CDAperp;

            }
            else
            {
                Collided = true;
            }
        }

        glm::vec3 A = Object1.getSupportPoint(direction) - Object2.getSupportPoint(-direction);
            
        if (glm::dot(A, direction) < 1e-6)
            break;
        
        simplex.push_back(A);
        iterations++;
    }
    
    if (Collided)
    {
        glm::vec3 direction = Object2.CenterOfMass - Object1.CenterOfMass;
        
        glm::vec3 collisionNormal; //collision normal
        
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
        
        float minDistance = MAXFLOAT;
        
        while (minDistance == MAXFLOAT)
        {
            collisionNormal = glm::vec3(normals[minFace].x, normals[minFace].y, normals[minFace].z);
            minDistance     = normals[minFace].w;
            
            glm::vec3 support = Object1.getAverageSupportPoint(collisionNormal) - Object2.getAverageSupportPoint(-collisionNormal);
            
            float sDistance = glm::dot(collisionNormal, support);
            
            if (abs(sDistance - minDistance) > 0.001f)
            {
                minDistance = MAXFLOAT;
                
                std::vector<std::pair<size_t, size_t>> uniqueEdges;
                
                for (size_t i = 0; i < normals.size(); i++)
                {
                    if (glm::dot(glm::vec3(normals[i]), support) >= 0.0f)
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
                
                std::vector<size_t> newFaces;
                for (auto [edgeIndex1, edgeIndex2] : uniqueEdges)
                {
                    newFaces.push_back(edgeIndex1);
                    newFaces.push_back(edgeIndex2);
                    newFaces.push_back(polytope.size());
                }
                
                polytope.push_back(support);
                
                auto [newNormals, newMinFace] = GetFaceNormals(polytope, newFaces);
                
                float oldMinDistance = MAXFLOAT;
                for (int i = 0; i < normals.size(); i++)
                {
                    if (normals[i].w < oldMinDistance)
                    {
                        oldMinDistance = normals[i].w;
                        minFace = i;
                    }
                }
                
                if (newNormals[newMinFace].w < oldMinDistance)
                {
                    minFace = newMinFace + normals.size();
                }
                
                faces  .insert(faces  .end(), newFaces  .begin(), newFaces  .end());
                normals.insert(normals.end(), newNormals.begin(), newNormals.end());
            }
        }
        
        //collisionNormal *= 1;
        //minDistance += 0.0001f;


        
        if (glm::dot(collisionNormal, Object1.CenterOfMass - Object2.CenterOfMass) < 0)
        {
            collisionNormal *= -1;
        }
                
        glm::vec3 collisionPosition;
        
        if (glm::dot(collisionNormal, glm::normalize(Object1.getAverageSupportPoint(-collisionNormal))) > glm::dot(-collisionNormal, glm::normalize(Object2.getAverageSupportPoint(collisionNormal))))
        {
            collisionPosition = Object1.getAverageSupportPoint(-collisionNormal);
        }
        else
        {
            collisionPosition = Object2.getAverageSupportPoint(collisionNormal);
        }
                
        glm::vec3 r1 = collisionPosition - Object1.CenterOfMass;
        
        glm::vec3 r2 = collisionPosition - Object2.CenterOfMass;
        
        //glm::vec3 relativeVelocity = (Object2.Velocity + Object2.angularVelocity * glm::length(r2) * glm::dot(r2, (Object2.angularVelocity))) - (Object1.Velocity + Object1.angularVelocity * glm::length(r1) * glm::dot(r1, (Object1.angularVelocity))); //derivative of point on sphere , velocity of point on sphere
        
        glm::vec3 relativeVelocity = (Object2.Velocity - glm::cross(Object2.angularVelocity, r2)) - (Object1.Velocity + glm::cross(Object1.angularVelocity, r1));
        
        //std::cout << glm::to_string(relativeVelocity) << std::endl;
        
        //relativeVelocity = (Object2.Velocity) - (Object1.Velocity);
        
        
//        float e = (Object1.material.Restitution + Object2.material.Restitution) / 2.0f; //average restitution
//        
//        float impulseMagnitude = -(1 + e) * glm::dot(relativeVelocity, collisionNormal) / ( 1/Object1.Mass + 1/Object2.Mass + glm::dot(glm::cross(glm::inverse(Object1.inertiaTensor) * (glm::cross(r1, collisionNormal)), r1) + glm::cross(glm::inverse(Object2.inertiaTensor) * (glm::cross(r2, collisionNormal)), r2), collisionNormal) );
//        
//        glm::vec3 impulseVector = impulseMagnitude * collisionNormal;
//        
//        Object1.Velocity = Object1.Velocity - impulseVector/Object1.Mass;
//        Object2.Velocity = Object2.Velocity + impulseVector/Object2.Mass;
//        
//        Object1.angularVelocity = Object1.angularVelocity - impulseMagnitude * glm::inverse(Object1.inertiaTensor) * glm::cross(r1, collisionNormal);
//        
//        Object2.angularVelocity = Object2.angularVelocity - impulseMagnitude * glm::inverse(Object2.inertiaTensor) * glm::cross(r2, collisionNormal);
//        
//        for (glm::vec3& Vertex : Object1.Vertices)
//            Vertex += collisionNormal * minDistance * 1.5f * ((Object2.Mass)/(Object1.Mass + Object2.Mass));
//        Object1.CenterOfMass += collisionNormal * minDistance * 1.5f * ((Object2.Mass)/(Object1.Mass + Object2.Mass));
//        
//        for (glm::vec3& Vertex : Object2.Vertices)
//            Vertex -= collisionNormal * minDistance * 1.5f * ((Object1.Mass)/(Object1.Mass + Object2.Mass));
//        Object2.CenterOfMass -= collisionNormal * minDistance * 1.5f * ((Object1.Mass)/(Object1.Mass + Object2.Mass));
        
        //std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
        //std::cout << impulseMagnitude << std::endl;
        //std::cout << -(1 + e) * glm::dot(relativeVelocity, collisionNormal) << std::endl;
        //std::cout << ( 1/Object1.Mass + 1/Object2.Mass + glm::dot(glm::cross(glm::inverse(Object1.inertiaTensor) * (glm::cross(r1, collisionNormal)), r1) + glm::cross(glm::inverse(Object2.inertiaTensor) * (glm::cross(r2, collisionNormal)), r2), collisionNormal) ) << std::endl;
        //std::cout << glm::to_string(relativeVelocity) << std::endl;
        //std::cout << glm::to_string(collisionNormal) << std::endl;
        //std::cout << glm::to_string(collisionPosition) << std::endl;
        //std::cout << glm::to_string(r1) << std::endl;
        //std::cout << glm::to_string(r2) << std::endl;
        //std::cout << glm::to_string(Object1.CenterOfMass) << std::endl;
        //std::cout << glm::to_string(Object2.CenterOfMass) << std::endl;
        //std::cout << glm::to_string(Object1.angularVelocity) << std::endl;
        //std::cout << glm::to_string(Object2.angularVelocity) << std::endl;
        //std::cout << glm::to_string(Object1.Velocity) << std::endl;
        //std::cout << glm::to_string(Object2.Velocity) << std::endl;
        //std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
        
        //if (Collided)
            //throw std::runtime_error("Collided");
    }
}

void Physics::applyNewtonianGravity(Object& Object1, Object& Object2)
{
    glm::vec3 direction = (Object1.CenterOfMass - Object2.CenterOfMass);
    double r = glm::length(direction); //(glm::length(direction) > Object1.boundingRadius + Object2.boundingRadius) ? glm::length(direction) : 100.0f ;
    double force_magnitude = G * (Object1.Mass * Object2.Mass)/(r*r);
    Object1.applyForce(-glm::normalize(direction) * (float) force_magnitude);
    Object2.applyForce(glm::normalize(direction) * (float) force_magnitude);
}



std::pair<std::vector<glm::vec4>, size_t> Physics::GetFaceNormals(std::vector<glm::vec3>& polytope, std::vector<size_t>& faces)
{
    std::vector<glm::vec4> normals;
    size_t minTriangle = 0;
    float  minDistance = MAXFLOAT;

    for (int i = 0; i < faces.size(); i += 3)
    {
        glm::vec3 a = polytope[faces[i    ]];
        glm::vec3 b = polytope[faces[i + 1]];
        glm::vec3 c = polytope[faces[i + 2]];

        glm::vec3 normal = glm::normalize(glm::cross(b - a, c - a));
        float distance = glm::dot(normal, a);

        if (distance < 0) {
            normal   *= -1;
            distance *= -1;
        }

        normals.emplace_back(normal, distance);

        if (distance < minDistance) {
            minTriangle = i / 3;
            minDistance = distance;
        }
    }

    return { normals, minTriangle };
}

void Physics::AddIfUniqueEdge(std::vector<std::pair<size_t, size_t>>& edges, const std::vector<size_t>& faces, size_t a, size_t b)
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
