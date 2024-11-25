//
//  Physics.hpp
//  Game
//
//  Created by Nathan Thurber on 25/5/24.
//

#include "Scene/Object.hpp"

#include <iostream>

#include <glm/glm.hpp>
#include <vector>

#include <glm/gtx/string_cast.hpp>

class Physics
{
public:
    Physics();
    ~Physics();
    
    void OnUpdate(); //wind, GR field, 
    
    static void solveCollisions(Object& object1, Object& object2);
    
    static void applyNewtonianGravity(Object& object1, Object& object2);
    
    static void applyForce(Object& object, glm::vec3 Force, glm::vec3 forcePosition);
    
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
    
    static void updatePosition(Object& object, double dt);
    
    static void updateRotation(Object& object, double dt);

    
private:
    constexpr const static float G = 6.67430 * 0.00000000001;
    
private:
    static std::pair<std::vector<glm::vec4>, size_t> GetFaceNormals(std::vector<glm::vec3>& polytope, std::vector<size_t>& faces);
    
    static void AddIfUniqueEdge(std::vector<std::pair<size_t, size_t>>& edges, const std::vector<size_t>& faces, size_t a, size_t b);

};
