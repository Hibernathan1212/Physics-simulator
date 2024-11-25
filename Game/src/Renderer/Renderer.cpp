//
//  Renderer.cpp
//  Game
//
//  Created by Nathan Thurber on 17/5/24.
//

#include "Renderer.hpp"
#include <iostream>

#include <thread>

extern bool g_ApplicationRunning;

Renderer::Renderer(ApplicationSpecification& specification)
: m_GraphicsAPI(Graphics_API), m_specification(specification), m_Renderer(m_specification)
{
    init();
}

Renderer::~Renderer()
{
    cleanUp();
}

void Renderer::init()
{
    switch(m_GraphicsAPI)
    {
        case 0:
        {
            std::cout << "[Renderer] Graphics API = Vulkan" << std::endl;
            break;
        }
        case 1:
        {
            std::cout << "[Renderer] Graphics API = Metal" << std::endl;
            break;
        }
        case 2:
        {
            std::cout << "[Renderer] Graphics API = OpenGL" << std::endl;
            break;
        }
        default:
        {
            std::cout << "[Renderer] Graphics API = OpenGL" << std::endl;
            break;
        }
    }
}

void Renderer::render(bool& Running, EntityComponentSystem& ecs, Camera& g_Camera)
{
    m_Renderer.render(Running, g_Camera, ecs);
}

void Renderer::cleanUp()
{
    //m_Renderer.cleanUp();
}
