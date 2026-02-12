#pragma once

#include "../core/essentials.hpp"
#include "../core/manager.hpp"
#include "camera.hpp"

struct Player {
    vec2 pos = vec2(0.0);
    float health = 100.0;

    float cooldown = 0.0;

    float speed = 1.0;

    void move() {
        if (Input::IsDown("w")) {
            pos.y += speed;
        }
        if (Input::IsDown("s")) {
            pos.y -= speed;
        }
        if (Input::IsDown("d")) {
            pos.x += speed;
        }
        if (Input::IsDown("a")) {
            pos.x -= speed;
        }
    }
};