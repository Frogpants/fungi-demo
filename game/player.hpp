#pragma once

#include "../core/essentials.hpp"
#include "../core/manager.hpp"
#include "camera.hpp"

struct Player {
    vec2 pos = vec2(0.0);
    vec2 dim = vec2(45.0);
    vec2 vel = vec2(0.0);
    float health = 100.0;

    float cooldown = 0.0;

    float speed = 0.5;

    void controls() {
        if (Input::IsDown("w")) {
            vel.y += speed;
        }
        if (Input::IsDown("s")) {
            vel.y -= speed;
        }
        if (Input::IsDown("d")) {
            vel.x += speed;
        }
        if (Input::IsDown("a")) {
            vel.x -= speed;
        }

        vel = vel * 0.7;
    }
};