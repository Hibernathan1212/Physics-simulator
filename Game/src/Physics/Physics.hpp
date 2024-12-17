//
//  Physics.hpp
//  Game
//
//  Created by Nathan Thurber on 25/5/24.
//

#pragma once

//#include "Scene/Object.hpp"

#include "Resources/EntityComponentSystem.hpp"

#include <glm/gtx/string_cast.hpp>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>


extern EntityComponentSystem* ecs;


class Physics
{
public:
    Physics();
    ~Physics();
    
    void update();
    
    void updatePhysics(); //wind, GR field,
    
    void updateBodies(double dt);
    
    void solveCollisions(const Entity object1, const Entity object2);
    
    void applyNewtonianGravity(const Entity object1, const Entity object2);
//
//    void applyForce(Object& object, glm::vec3 Force, glm::vec3 forcePosition);
    
    //collisions
    //gravity
    //fluids
    //air
    //thermal energy
    //chemical reactions
    //tensile/shear/compressive/bend/torsion
    //stresses
    //object model deformation
    //objects falling apart
    //GR
    //electromagnetism
    
//    static void updatePosition(Object& object, double dt);
//    
//    static void updateRotation(Object& object, double dt);

    
private:
    constexpr const static float G = 6.67430 * 0.00000000001;
    
private:
    
    double m_updateTime = 0;
    double m_lastUpdateTime = 0;
    double m_timeStep = 0.00001f;
    
    static std::pair<std::vector<glm::vec4>, size_t> GetFaceNormals(std::vector<glm::vec3>& polytope, std::vector<size_t>& faces);
    
    static void AddIfUniqueEdge(std::vector<std::pair<size_t, size_t>>& edges, const std::vector<size_t>& faces, size_t a, size_t b);
};
