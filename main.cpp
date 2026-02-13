#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include "game/player.hpp"
#include "game/enemy.hpp"
#include "game/camera.hpp"
#include "game/bullet.hpp"

#include "core/manager.hpp"
#include "core/essentials.hpp"
#include "core/collision.hpp"

Player player;
Camera camera;
std::vector<Enemy> enemies;
std::vector<Bullet> bullets;

int tick = 0;


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

void DrawCircle(vec2 pos, float radius, float r, float g, float b)
{
    const int segments = 32; // more = smoother

    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);

        // Center
        glVertex2f(pos.x, pos.y);

        // Outer ring
        for (int i = 0; i <= segments; ++i)
        {
            float angle = 2.0f * 3.1415926f * i / segments;
            float x = pos.x + std::cos(angle) * radius;
            float y = pos.y + std::sin(angle) * radius;
            glVertex2f(x, y);
        }

    glEnd();
}


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


vec2 GetMouseWorld()
{
    float near = 0.1f;

    float ndcX = (2.0f * Mouse::X()) / 1280.0f - 1.0f;
    float ndcY = 1.0f - (2.0f * Mouse::Y()) / 720.0f;

    float viewX = ndcX * screen.x;
    float viewY = ndcY * screen.y;

    float scale = camera.zoom / near;

    viewX *= scale;
    viewY *= scale;

    vec2 mouseWorld;
    mouseWorld.x = viewX + camera.pos.x;
    mouseWorld.y = viewY + camera.pos.y;

    return mouseWorld;
}


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
    // for (int i = 0; i < 10; i++) {
    //     Enemy e;
    //     e.pos = vec2(rand() % 600 - 300, rand() % 600 - 300);
    //     enemies.push_back(e);
    // }

    Manager::Init(window);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glLoadIdentity();

        tick += 1;

        // Apply camera
        camera.target = player.pos;
        camera.follow();
        glTranslatef(-camera.pos.x, -camera.pos.y, 0);

        player.controls();

        if (Mouse::IsDown(0)) {
            if (tick % 100 == 0) {

                Bullet b;
                b.pos = player.pos;

                vec2 mouseCentered;
                mouseCentered.x = Mouse::X() - screen.x * 0.5f;
                mouseCentered.y = -screen.y * 0.5f + Mouse::Y();

                b.dir = pointAt(player.pos, mouseCentered);

                bullets.push_back(b);
                DrawCircle(b.pos, 10.0, 1, 1, 1);
            }
        }

        vec2 force = vec2(0.0);

        for (auto& b : bullets) {
            b.move();
            if (abs(b.pos.x) >= screen.x) {
                if (abs(b.pos.y) >= screen.y) {
                    bullets.erase(bullets.begin() + findIndex(bullets, b));
                }
            }
            DrawCircle(b.pos, 2.0, 1, 1, 1);
        }

        // Enemy Updating
        for (auto& e : enemies) {
            e.target = player.pos;

            e.follow();
            vec2 separation = vec2(0.0f);

            for (auto& t : enemies) {
                if (&e == &t) continue;

                vec2 diff = e.pos - t.pos;
                float dist = length(diff) * 0.1;

                separation = separation + normalize(diff) / dist;
            }

            e.pos = e.pos + separation * e.speed * 4.0;

            if (BoxCollide(player.pos, player.dim, e.pos, e.dim)) {
                vec2 diff = player.pos - e.pos;
                float dist = length(diff) * 0.01;

                force = force + normalize(diff) / dist;
            }

            for (auto& b : bullets) {
                if (BallCollide(e.pos, e.dim, b.pos, b.dim)) {
                    enemies.erase(enemies.begin() + findIndex(enemies, e));
                    bullets.erase(bullets.begin() + findIndex(bullets, b));
                }
            }

            DrawQuad(e.pos, 12, 1, 0, 0);
        }

        player.pos = player.pos + force * 2.0;
        DrawQuad(player.pos, 15, 0, 1, 0);

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
