/**
 * @file Texture.cpp
 * @brief Implementation of the Texture class for handling OpenGL textures.
 * This file includes methods for loading textures from files, binding them, and checking their validity.
 * It uses the stb_image library for image loading and OpenGL for texture management.
 * @author Ahmet Abdullah Gultekin
 * @date 2025-05-27
 */
#define STB_IMAGE_IMPLEMENTATION

#include "../../third_party/stb_image.h"
#include "Texture.h"
#include "../Utils/Logger.h"
#include <GL/glew.h>

// Constructor and Destructor
Texture::Texture() {}

Texture::~Texture() {
    if (id) glDeleteTextures(1, &id);
}

/**
 * @fn LoadFromFile
 * @brief Loads a texture from a file.
 * This method loads a texture from a file using the stb_image library.
 * It generates a new OpenGL texture ID and binds it to the current context.
 * It then sets texture parameters and uploads the texture data.
 * It then unbinds the texture and returns the texture ID.
 * If an error occurs during loading, the texture ID is set to 0 and false is returned.
 * @param path The path to the texture file.
 * @return True if the texture was loaded successfully, false otherwise.
 */
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

/**
 * @fn Bind
 * @brief Binds the texture to the current OpenGL context.
 * This method binds the texture to the GL_TEXTURE_2D target.
 * It should be called before rendering to ensure the texture is active.
 */
void Texture::Bind() const {
    glBindTexture(GL_TEXTURE_2D, id);
}

/**
 * @fn IsValid
 * @brief Checks if the texture is valid.
 * This method checks if the texture ID is non-zero and returns true if it is valid.
 * It can be extended to include more validation checks if needed.
 * @return True if the texture is valid, false otherwise.
 */
bool Texture::IsValid() const {
    return id != 0;
}

/**
 * @fn Release
 * @brief Releases the texture resources.
 * This method deletes the OpenGL texture associated with this object.
 * It sets the texture ID to 0 and resets width and height to 0.
 * If the texture is already released, it logs a warning.
 */
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

