//
//  BVH.hpp
//  Game
//
//  Created by Nathan Thurber on 5/12/24.
//

#pragma once

#include "Resources/EntityComponentSystem.hpp"

//#include "Renderer/ShaderTypes.hpp"


extern EntityComponentSystem* ecs;

//extern UniformData* u_Data;

class BVHcollision
{
public:
    BVHcollision(Entity& entity1, Entity& entity2);
    
    //const bool checkCollision(const uint32_t depth);
    
private:
    
    Entity m_entity1;
    Entity m_entity2;
};
