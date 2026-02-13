#include "image.hpp"
#include <iostream>
#include <filesystem>  // C++17
#include <GL/gl.h>
#include <GL/glu.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Image {

bool Init()
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return true;
}

GLuint Load(const char* filename)
{
    std::string fullPath(filename); // use the path exactly as passed

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &channels, 4);
    if (!data) {
        std::cout << "Failed to load texture: " << fullPath << std::endl;
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return texture;
}

void Draw(GLuint texture, vec2 pos, float size, float angle)
{
    glPushMatrix();
    glTranslatef(pos.x, pos.y, 0);
    glRotatef(angle, 0, 0, 1);

    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(-size, -size);
        glTexCoord2f(1, 0); glVertex2f( size, -size);
        glTexCoord2f(1, 1); glVertex2f( size,  size);
        glTexCoord2f(0, 1); glVertex2f(-size,  size);
    glEnd();

    glPopMatrix();
}

void DrawRect(vec2 pos, vec2 size, float r, float g, float b, float angle)
{
    glPushMatrix();
    glTranslatef(pos.x, pos.y, 0);
    glRotatef(angle, 0, 0, 1);

    glBindTexture(GL_TEXTURE_2D, 0);  // disable texture
    glColor3f(r, g, b);

    glBegin(GL_QUADS);
        glVertex2f(-size.x, -size.y);
        glVertex2f( size.x, -size.y);
        glVertex2f( size.x,  size.y);
        glVertex2f(-size.x,  size.y);
    glEnd();

    glPopMatrix();
}

} // namespace Image
