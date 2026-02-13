#pragma once
#include <GL/gl.h>
#include "../essentials.hpp"   // vec2

namespace Image
{
    bool Init();

    // Texture loading
    GLuint Load(const char* path);

    // Textured quad
    void Draw(GLuint texture, vec2 pos, float size, float angle = 0.0f);

    // Colored rectangle (like your old DrawQuad)
    void DrawRect(vec2 pos, vec2 size, float r, float g, float b, float angle = 0.0f);
}