#ifndef BILLIARDSHOW_RENDERER_H
#define BILLIARDSHOW_RENDERER_H

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Shader.h"

#include <vector>

class Renderer {
public:
    // Dimensions in centimeters
    static constexpr float centimeterToMeterFactor = 0.01f; // Convert cm to meters

    Renderer();

    ~Renderer();

    void DrawParallelepiped(const glm::vec3 &position, const glm::vec3 &size);

    void SetCamera(const Camera *cam);

    void InitCubeGL();
};

#endif //BILLIARDSHOW_RENDERER_H
