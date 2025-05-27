/**
 * @file Minimap.h
 * @brief Header file for the Minimap class.
 * This class provides functionality to render a minimap for the billiard table,
 * showing the positions of the balls on a scaled-down version of the table.
 * @author Ahmet Abdullah Gultekin
 * @date 2025-05-27
 * @version 1.0
 */
#ifndef BILLIARDSHOW_MINIMAP_H
#define BILLIARDSHOW_MINIMAP_H

#pragma once

#include "../Renderer/Renderer.h"

/** * @class Minimap
 * @brief Represents a minimap for the billiard table.
 * This class handles rendering a minimap that shows the positions of the balls
 * on a scaled-down version of the billiard table.
 */
class Minimap {
public:
    /**
     * @brief Constructor for the Minimap class.
     * Initializes the minimap with a renderer and the dimensions of the billiard table.
     * @param renderer Pointer to the Renderer instance used for rendering.
     * @param tableWidth Width of the billiard table in meters.
     * @param tableDepth Depth of the billiard table in meters.
     */
    Minimap(Renderer *renderer, float tableWidth, float tableDepth);

    /**
     * @brief Destructor for the Minimap class.
     * Cleans up any resources used by the minimap.
     */
    void Render(int windowWidth, int windowHeight);

    /**
     * @brief Sets the positions of the balls for rendering on the minimap.
     * @param positions Pointer to a vector containing the positions of the balls in 3D space.
     * The positions should be in meters.
     */
    void SetBallPositions(const std::vector<glm::vec3> *positions);

private:
    Renderer *renderer;
    float tableWidth, tableDepth;
    const std::vector<glm::vec3> *ballPositions;
};

#endif //BILLIARDSHOW_MINIMAP_H
