/**
 * @file Renderer.h
 * @brief Renderer class for OpenGL rendering in the billiard simulation.
 * This class handles the rendering of 3D objects, including the billiard table and balls,
 * as well as 2D elements like the minimap.
 * @author Ahmet Abdullah Gultekin
 * @date 2025-05-27
 * @version 1.0
 */
#ifndef BILLIARDSHOW_RENDERER_H
#define BILLIARDSHOW_RENDERER_H

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Shader.h"

#include <vector>

/** * @class Renderer
 * @brief Handles OpenGL rendering operations for the billiard simulation.
 * This class provides methods to draw 3D objects like the billiard table and balls,
 * as well as 2D elements like circles for the minimap.
 */
class Renderer {
public:
    // Dimensions in centimeters
    static constexpr float centimeterToMeterFactor = 0.01f; // Convert cm to meters

    Renderer();

    ~Renderer();

    static void DrawParallelepiped(const glm::vec3 &position, const glm::vec3 &size);

    void SetCamera(const Camera *cam);

    // Draw a filled 2D circle (for minimap balls)
    void DrawCircle2D(const glm::vec3 &center, float radius, int segments = 24);
};

#endif //BILLIARDSHOW_RENDERER_H
