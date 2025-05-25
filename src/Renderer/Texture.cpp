#define STB_IMAGE_IMPLEMENTATION

#include "../../third_party/stb_image.h"
#include "Texture.h"
#include "../Utils/Logger.h"
#include <GL/glew.h>

Texture::Texture() {}

Texture::~Texture() {
    if (id) glDeleteTextures(1, &id);
}

bool Texture::LoadFromFile(const std::string &path) {
    int n;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &n, STBI_rgb_alpha);
    if (!data) {
        Logger::Error("Failed to load image: " + path);
        return false;
    }
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    GLenum err = glGetError();
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    if (err != GL_NO_ERROR) {
        Logger::Error("OpenGL error after loading texture: " + std::to_string(err) + ", path: " + path);
        glDeleteTextures(1, &id);
        id = 0;
        return false;
    }
    return id != 0;
}

void Texture::Bind() const {
    glBindTexture(GL_TEXTURE_2D, id);
}

bool Texture::IsValid() const {
/*    if (id == 0) {
        Logger::Error("Texture is not valid: ID is 0.");
        return false;
    } else if (width <= 0 || height <= 0) {
        Logger::Error("Texture is not valid: width or height is non-positive.");
        return false;
    } else if (glIsTexture(id) == GL_FALSE) {
        Logger::Error("Texture is not valid: OpenGL reports it as not a texture.");
        return false;
    } else if (glGetError() != GL_NO_ERROR) {
        Logger::Error("Texture is not valid: OpenGL error occurred.");
        return false;
    } else {
        Logger::Info("Texture is valid: ID = " + std::to_string(id) + ", size = " + std::to_string(width) + "x" + std::to_string(height));
    }*/
    return id != 0;
}

void Texture::Release() {
    if (id != 0) {
        glDeleteTextures(1, &id);
        id = 0;
        width = 0;
        height = 0;
    } else {
        Logger::Warn("Texture::Release called on an already released texture.");
    }
}

