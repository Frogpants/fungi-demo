#pragma once

#include <GLFW/glfw3.h>

#include "input/input.hpp"
#include "mouse/mouse.hpp"
#include "essentials.hpp"

inline float gamemode = 0.0f;

inline vec2 screen = vec2(640.0);

// namespace KeyMaps {
//     inline controls = 
// }

namespace Manager {

    inline GLFWwindow* window = nullptr;

    inline void Init(GLFWwindow* win) {
        window = win;

        Input::Init(window);
        Mouse::Init(window);
    }

    inline void Update() {
        Input::Update();
        Mouse::Update();
    }
}
