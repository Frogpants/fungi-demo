#pragma once

#include "../core/essentials.hpp"
#include "player.hpp"

#include <vector>
#include <iostream>


struct Enemy {
    vec2 pos = vec2(0.0);
    float health = 100.0;

    float cooldown = 0.0;

    float speed = 0.5;

    vec2 target;
};
