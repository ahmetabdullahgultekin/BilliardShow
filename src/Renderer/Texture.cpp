#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "Texture.h"
#include <GL/glew.h>

Texture::Texture() {}

Texture::~Texture() {
    if (id) glDeleteTextures(1, &id);
}

bool Texture::LoadFromFile(const std::string &path) {
    int n;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &n, STBI_rgb_alpha);
    if (!data) return false;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    return true;
}

void Texture::Bind() const {
    glBindTexture(GL_TEXTURE_2D, id);
}

bool Texture::IsValid() const {
    return id != 0;
}
