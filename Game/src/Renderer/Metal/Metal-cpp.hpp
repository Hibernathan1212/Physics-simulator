//
//  Metal-cpp.hpp
//  Game
//
//  Created by Nathan Thurber on 7/6/24.
//

#pragma once

#include <iostream>

class Metal
{
public:
    Metal();
    ~Metal();
    
    void init();
    void cleanUp();
    
    const void drawFrame();
    
public:
    bool g_FrameBufferResized = false;
    
private:

    
private:
    
    int m_ViewportWidth, m_ViewportHeight;
    
private:
    const int                           MAX_FRAMES_IN_FLIGHT = 3;
    uint32_t                            m_CurrentFrame = 0;
    
    
};
