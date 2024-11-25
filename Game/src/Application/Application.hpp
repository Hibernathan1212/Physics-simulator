//
//  Application.hpp
//  Game
//
//  Created by Nathan Thurber on 17/5/24.
//

#include "Renderer/Renderer.hpp"
#include "Resources/ResourceManager.hpp"
#include "Resources/EntityComponentSystem.hpp"
#include "Resources/BVH.hpp"
//#include "Scene/Scene.hpp"

#include "Application/ApplicationSpecification.hpp"

class Application
{
public:
    Application(ApplicationSpecification& specification);
    ~Application();
    void Run();
    void Close();
    
public:
    ResourceManager         rm;
    EntityComponentSystem   ecs;
    Camera                  g_Camera;
    //Scene                   g_Scene;
    Renderer                g_Renderer;
    
public:
    float g_TimeStep;
    
private:
    void Init();
    void OnUpdate(float ts);
    void Shutdown();
    
    float GetTime();
    
private:
    ApplicationSpecification m_Specification;
    
    bool m_Running = false;
    
private:
    float m_FrameTime = 0.0f;
    float m_LastFrameTime = 0.0f;
};

Application* CreateApplication(int argc, char** argv);
