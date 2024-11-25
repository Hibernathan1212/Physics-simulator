//
//  Renderer.hpp
//  Game
//
//  Created by Nathan Thurber on 17/5/24.
//
#include <stdio.h>

//#include "Scene/Scene.hpp"
#include "Scene/Camera.hpp"
#include "Resources/EntityComponentSystem.hpp"

#include "Application/ApplicationSpecification.hpp"

//#include "Metal.hpp"

/*
 Graphics_API:
 0 = Vulkan
 1 = Metal
 2 = OpenGL
 
*/

#ifdef __APPLE__
#ifdef __MACH__
//mac
#define GLFW_INCLUDE_VULKAN
#include "Renderer/Vulkan/Vulkan.hpp"
#define Graphics_API 0
//
#else
//other apple products
#include "Renderer/Metal/Metal.hpp"
#define Graphics_API 1
//
#endif
#else
#include "Renderer/Vulkan/Vulkan.hpp"
#define Graphics_API 0

#endif

extern bool g_WindowOpen;

class Renderer
{
public:
    Renderer(ApplicationSpecification& specification);
    ~Renderer();
    void beginRenderThread();
    void render(bool& Running, EntityComponentSystem& ecs, Camera& g_Camera);
    void init();
    void cleanUp();

private:
    
private:
    ApplicationSpecification m_specification;
    
    int m_GraphicsAPI;
    
#ifdef __APPLE__
#ifdef __MACH__
    Vulkan m_Renderer;
#else
    Metal m_Renderer;
#endif
#else
    Vulkan m_Renderer;
#endif
};

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
