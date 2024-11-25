//
//  Input.hpp
//  Game
//
//  Created by Nathan Thurber on 30/5/24.
//

#pragma once

#include "KeyCodes.hpp"

#include <glm/glm.hpp>
#include <stdint.h>

class Input {
public:
    static bool IsKeyDown(KeyCode keycode);
    static bool IsMouseButtonDown(MouseButton button);

    static glm::vec2 GetMousePosition();

    static void SetCursorMode(CursorMode mode);
};

