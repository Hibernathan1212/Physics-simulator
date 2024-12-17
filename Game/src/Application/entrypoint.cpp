//
//  main.cpp
//  Game
//
//  Created by Nathan Thurber on 17/5/24.
//

//#include <iostream>
#include "Application.hpp"

extern bool g_ApplicationRunning;

int main(int argc, char * argv[]) {
    while (g_ApplicationRunning)
    {
        Application* app = CreateApplication(argc, argv);
        app->Run();
        delete app;
    }
    return 0;
}
