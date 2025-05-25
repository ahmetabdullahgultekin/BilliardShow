#ifndef BILLIARDSHOW_MODEL3D_H
#define BILLIARDSHOW_MODEL3D_H

#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>

#include "../Renderer/Texture.h"
#include "../Utils/Logger.h"
#include "../Renderer/Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

class Model3D {
public:
    Model3D();

    ~Model3D();

    // Loads vertices/normals/texcoords from the .obj file
    bool Load(const std::string &obj_model_filepath);

    // Sends vertex data to GPU (VAO/VBO)
    bool Install();

    // Renders the model at position (ignore orientation for now)
    void Render(const glm::vec3 &position, float scale = 1.0f) const;

    void SetTexture(const std::string &path);

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
    };

private:
    std::vector<Vertex> vertices;
    unsigned int VAO = 0, VBO = 0;
    Texture texture;
};

#endif //BILLIARDSHOW_MODEL3D_H
