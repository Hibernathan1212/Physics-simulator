//
//  Application.cpp
//  Game
//
//  Created by Nathan Thurber on 17/5/24.
//

#include "Application.hpp"

#include <iostream>

#include <thread>


//#define GLFW_INCLUDE_VULKAN

inline bool g_ApplicationRunning = 1;
inline bool g_WindowOpen = 1;

inline ResourceManager*         rm;
inline EntityComponentSystem*   ecs;

Application::Application(ApplicationSpecification& specification)
: m_Specification(specification), g_Renderer(specification), g_Camera(30.0f, 0.1f, 1000.0f), g_Physics()
{
    Init();
}

Application::~Application()
{
    Shutdown();
}

void Application::Init()
{
    rm = new ResourceManager();
    ecs = new EntityComponentSystem();
    
    Material* green = rm->getMaterial("green");
    green->setColor( {0.0f, 0.25f, 0.0f} );
    green->setEmission( {0.0f, 0.0f, 0.0f} );
    
    Material* light = rm->getMaterial("light");
    light->setColor( {0.8f, 0.8f, 0.1f} );
    light->setEmission( glm::vec3(0.8f, 0.8f, 0.1f) * 2.0f );
    
    Material* red = rm->getMaterial("red");
    red->setColor( {0.7f, 0.02f, 0.004f} );
    red->setEmission( glm::vec3(1.0f, 0.05f, 0.01f) * 0.0f );
    
    Material* orange = rm->getMaterial("orange");
    orange->setColor( {0.8f, 0.4f, 0.05f} );
    orange->setEmission( glm::vec3(1.0f, 0.6f, 0.05f) * 0.0f );
    
    Material* cream = rm->getMaterial("cream");
    cream->setColor( {0.85f, 0.7f, 0.7f} );
    cream->setEmission( glm::vec3(0.95f, 0.8f, 0.8f) * 0.0f );
    
    
    Entity dragon = ecs->createEntity();
    ecs->addComponent(dragon, MeshComponent{rm->getMesh("/Users/nathan/Downloads/Dragon_800K.obj")});
//    ecs->addComponent(dragon, MeshComponent{rm->getMesh("/Users/nathan/Downloads/untitled.obj")});
    ecs->addComponent(dragon, BVHComponent{rm->getBVH("/Users/nathan/Downloads/Dragon_800K.obj")});
//    ecs->addComponent(dragon, BVHComponent{rm->getBVH("/Users/nathan/Downloads/untitled.obj")});
    ecs->addComponent(dragon, TransformComponent{ .translation=glm::vec3(0.0f, 0.0f, 0.0f), .rotation=glm::vec3(0.0f, 0.0f, glm::pi<float>()), .scale=1.0f});
    ecs->addComponent(dragon, MaterialComponent{green});
    ecs->addComponent(dragon, PhysicsComponent{ .mass=10.0f });
    ecs->addComponent(dragon, ColliderComponent{ .boundingRadius=100.0f,
            .inertiaTensor = glm::mat3{1.0f/12.0f * 10.0f * (2*2 + 2*2), 0.0f, 0.0f,
                0.0f, 1.0f/12.0f * 10.0f * (2*2 + 2*2), 0.0f,
                0.0f, 0.0f, 1.0f/12.0f * 10.0f * (2*2 + 2*2) } } ); //not actually correct});
    
    //Entity statue = ecs->createEntity();
    //ecs->addComponent(statue, BVHComponent{rm->getBVH("/Users/nathan/Downloads/xyzrgb_statuette.ply")});
    //ecs->addComponent(statue, TransformComponent{glm::vec3(0.0f), glm::vec3(glm::pi<float>(), glm::pi<float>(), 0.0f), 0.02f} );
    //ecs->addComponent(statue, MaterialComponent{red});
    
    
    
    Entity Light = ecs->createEntity();
    ecs->addComponent(Light, MeshComponent{rm->getMesh("/Users/nathan/Downloads/untitled.obj")});
    ecs->addComponent(Light, BVHComponent{rm->getBVH("/Users/nathan/Downloads/untitled.obj")});
    ecs->addComponent(Light, TransformComponent{ glm::vec3(-6.0f, -15.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 8.0f} );
    ecs->addComponent(Light, MaterialComponent{light});
    ecs->addComponent(Light, PhysicsComponent{ .mass=10.0f });
    ecs->addComponent(Light, ColliderComponent{ .boundingRadius=100.0f });
    
//    Entity Light = ecs->createEntity();
//    ecs->addComponent(Light, MeshComponent{rm->getMesh("/Users/nathan/Downloads/untitled.obj")});
//    ecs->addComponent(Light, BVHComponent{rm->getBVH("/Users/nathan/Downloads/untitled.obj")});
//    ecs->addComponent(Light, TransformComponent{ .translation=glm::vec3(0.0f, -4.8f, 0.0f), .scale=1.0f /*, .rotation=glm::vec3(1.0f, 1.0f, 0.0f)*/ } );
//    ecs->addComponent(Light, MaterialComponent{light});
//    ecs->addComponent(Light, PhysicsComponent{ .mass=1000000000000.0f, .velocity=glm::vec3(0.0f, 2.0f, 0.0f), .angularVelocity=glm::vec3(0.0f, 0.0f, 0.0f) });
//    ecs->addComponent(Light, ColliderComponent{ .boundingRadius=100.0f,
//            .inertiaTensor = glm::mat3{1.0f/12.0f * 10.0f * (2*2 + 2*2), 0.0f, 0.0f,
//                0.0f, 1.0f/12.0f * 10.0f * (2*2 + 2*2), 0.0f,
//                0.0f, 0.0f, 1.0f/12.0f * 10.0f * (2*2 + 2*2) } } ); //not actually correct
    
    //Entity sponza = ecs->createEntity();
    //ecs->addComponent(sponza, BVHComponent{rm->getBVH("/Users/nathan/Downloads/sponza.obj")});
    //ecs->addComponent(sponza, TransformComponent{ glm::vec3(20.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.5f} );
    //ecs->addComponent(sponza, MaterialComponent{cream});
    
    //Entity hairball = ecs->createEntity();
    //ecs->addComponent(hairball, BVHComponent{rm->getBVH("/Users/nathan/Downloads/hairball.obj")});
    //ecs->addComponent(hairball, TransformComponent{ glm::vec3(-6.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.25f} );
    //ecs->addComponent(hairball, MaterialComponent{red});
    
    //Entity knight = ecs->createEntity();
    //ecs->addComponent(knight, BVHComponent{rm->getBVH("/Users/nathan/Downloads/Knight.fbx")});
    //ecs->addComponent(knight, TransformComponent{ glm::vec3(-4.0f, -0.0f, 6.0f), glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f), 2.0f} );
    //ecs->addComponent(knight, MaterialComponent{orange});
    
    //ecs->addComponent(test, BVHComponent{rm->getBVH("/Users/nathan/Downloads/untitled.obj")});
}

void Application::Run()
{
    m_Running = true;
    
    //std::thread Scene(g_Scene.init(g_ApplicationRunning));
    
    //std::thread Scene( [&Scene, this](){ g_Scene.init(g_ApplicationRunning);} );
                     
    //std::thread Camera( [&Camera, this](){ g_Camera.init(g_ApplicationRunning);} );

    
    //g_Renderer->render();
        
    //std::thread Renderer( [&Renderer, this](){g_Renderer.render(g_ApplicationRunning, g_Scene, g_Camera);} );

    std::thread physicsThread( [&physicsThread, this]() { g_Physics.update(); });
        
    m_LastFrameTime = duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

    while (g_ApplicationRunning)
    {
        float time = GetTime();
        m_FrameTime = time - m_LastFrameTime;
        g_TimeStep = glm::min<float>(m_FrameTime, 0.0333f); //0.0333f is slowest it will run (30fps)
        m_LastFrameTime = time;
        
        g_Camera.OnUpdate(g_TimeStep);
        
        g_Renderer.render(g_Camera);
    }
    
    physicsThread.join();
    
    //Scene.join();
    //Camera.join();
    //Renderer.join();
    
}

void Application::Shutdown()
{
    g_ApplicationRunning = false;
    g_Renderer.cleanUp();
    
    delete ecs;
    delete rm;
}

void Application::OnUpdate(float ts)
{
    g_Camera.OnUpdate(ts);
}

void Application::Close()
{
    m_Running = false;
    g_Renderer.cleanUp();
}

float Application::GetTime()
{
    return (float)glfwGetTime();
}
