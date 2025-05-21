#ifndef BILLIARDSHOW_MODEL3D_H
#define BILLIARDSHOW_MODEL3D_H

#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

class Model3D {
public:
    Model3D();

    ~Model3D();

    // Loads vertices/normals/texcoords from .obj file
    bool Load(const std::string &obj_model_filepath);

    // Sends vertex data to GPU (VAO/VBO)
    bool Install();

    // Renders the model at position (ignore orientation for now)
    void Render(const glm::vec3 &position);

private:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
    };
    std::vector<Vertex> vertices;
    unsigned int VAO = 0, VBO = 0;
};

#endif //BILLIARDSHOW_MODEL3D_H
