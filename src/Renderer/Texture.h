#ifndef BILLIARDSHOW_TEXTURE_H
#define BILLIARDSHOW_TEXTURE_H

#pragma once

#include <string>
#include <GL/glew.h>

class Texture {
public:
    Texture();

    ~Texture();

    bool LoadFromFile(const std::string &path);

    void Bind() const;

    bool IsValid() const;

    int GetWidth() const { return width; }

    int GetHeight() const { return height; }

    void Release();

private:
    GLuint id = 0;
    int width = 0, height = 0;
};

#endif //BILLIARDSHOW_TEXTURE_H
