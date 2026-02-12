#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "game/player.hpp"
#include "game/enemy.hpp"
#include "game/camera.hpp"

#include "core/manager.hpp"
#include "core/essentials.hpp"

Player player;
Camera camera;
std::vector<Enemy> enemies;

// --------------------------------------------
// Simple Quad Renderer
// --------------------------------------------
void DrawQuad(vec2 pos, float size, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(pos.x - size, pos.y - size);
        glVertex2f(pos.x + size, pos.y - size);
        glVertex2f(pos.x + size, pos.y + size);
        glVertex2f(pos.x - size, pos.y + size);
    glEnd();
}

// --------------------------------------------
// UI Health Bar
// --------------------------------------------
void DrawHealthBar(float health)
{
    float width = 200.0f;
    float height = 20.0f;

    float hpPercent = health / 100.0f;

    // Background
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
        glVertex2f(-screen.x + 20, screen.y - 40);
        glVertex2f(-screen.x + 20 + width, screen.y - 40);
        glVertex2f(-screen.x + 20 + width, screen.y - 40 + height);
        glVertex2f(-screen.x + 20, screen.y - 40 + height);
    glEnd();

    // Health
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(-screen.x + 20, screen.y - 40);
        glVertex2f(-screen.x + 20 + width * hpPercent, screen.y - 40);
        glVertex2f(-screen.x + 20 + width * hpPercent, screen.y - 40 + height);
        glVertex2f(-screen.x + 20, screen.y - 40 + height);
    glEnd();
}

// --------------------------------------------

int main()
{
    srand((unsigned int)time(nullptr));

    if (!glfwInit())
        return -1;

    GLFWwindow* window = glfwCreateWindow(1280, 720, "2.5D Roguelike Demo", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Setup 2D projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(70.0, screen.x/screen.y, 0.1, 1000.0);
    //glFrustum(-screen.x, screen.x, -screen.y, screen.y, 0.1, 1000.0);
    glOrtho(-screen.x, screen.x, -screen.y, screen.y, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    // Spawn enemies
    for (int i = 0; i < 10; i++) {
        Enemy e;
        e.pos = vec2(rand() % 600 - 300, rand() % 600 - 300);
        enemies.push_back(e);
    }

    Manager::Init(window);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glLoadIdentity();

        // Apply camera
        camera.target = player.pos;
        camera.follow();
        glTranslatef(-camera.pos.x, -camera.pos.y, 0);

        // Player Updating
        player.move();
        DrawQuad(player.pos, 15, 0, 1, 0);

        // Enemy Updating
        for (auto& e : enemies) {
            e.target = player.pos;
            vec2 dir = e.target - e.pos;
            dir = normalize(dir);
            e.pos += dir;

            DrawQuad(e.pos, 12, 1, 0, 0);
        }

        // UI
        glLoadIdentity();
        DrawHealthBar(player.health);

        if(Input::IsDown("escape")) {
            // change to pause at some point
            glfwTerminate();
            return 0;
        }

        Manager::Update();

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
