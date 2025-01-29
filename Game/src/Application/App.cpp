//
//  App.cpp
//  Game
//
//  Created by Nathan Thurber on 17/5/24.
//

#include "Application/Application.hpp"
#include <iostream>

Application* CreateApplication(int argc, char** argv)
{
    ApplicationSpecification spec;
    spec.Name = "Test";
    spec.Width = 1440;
    spec.Height = 720;

    Application* app = new Application(spec);
    return app;
}

