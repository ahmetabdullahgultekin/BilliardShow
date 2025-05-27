#ifndef BILLIARDSHOW_SCENE_H
#define BILLIARDSHOW_SCENE_H

#pragma once

#include <glm/glm.hpp>

#include <future>
#include <chrono>
#include <thread>
#include <atomic>
#include <vector>
#include <iostream>

#include "../Renderer/Renderer.h"
#include "../Loader/ObjectLoader.h"
#include "../App.h"
#include "../Scene/Table.h"
#include "../Utils/Logger.h"
#include "Ball.h"

class Ball;

/**
 * @file Scene.h
 * @brief Scene class for managing the billiard table and balls.
 * This class handles the rendering of the billiard table and balls,
 * as well as loading ball models in a separate thread.
 * @author Ahmet Abdullah Gultekin
 * @date 2025-05-27
 * @version 1.0
 */
class Scene {
public:

    /**
     * @brief Default constructor for Scene.
     * Initializes the scene with an empty table and ball positions.
     */
    Scene();

    ~Scene();

    /**
     * @brief Renders the scene.
     * This method draws the billiard table and all balls in the scene.
     * It uses the Renderer to draw the objects.
     */
    void Render(); // Draw table and balls

    /**
     * @brief Sets the table for the scene.
     * This method assigns a Table instance to the scene.
     * It checks if the provided table is valid (not null) before setting it.
     * @param _table Pointer to the Table instance to set.
     */
    void InstallBalls();

    /**
     * @brief Loads balls in a separate thread.
     * This method initializes the ball positions and creates Ball objects with their models.
     * It uses atomic variables to track progress and completion status.
     * @param progress Pointer to an atomic float for tracking loading progress.
     * @param done Pointer to an atomic bool for signaling completion.
     */
    void LoadBallsThreaded(std::atomic<float> *progress, std::atomic<bool> *done);

    /**
     * @brief Sets the renderer for the scene.
     * This method assigns a Renderer instance to the scene.
     * It checks if the provided renderer is valid (not null) before setting it.
     * @param renderer Pointer to the Renderer instance to set.
     */
    void SetRenderer(Renderer *renderer);

    /**
     * @brief Sets the table for the scene.
     * This method assigns a Table instance to the scene.
     * It checks if the provided table is valid (not null) before setting it.
     * @param _table Pointer to the Table instance to set.
     */
    void Update(float deltaTime);

    /** @brief Resets the ball positions to their initial state.
     * This method clears the current ball positions and reinitializes them
     * to the starting positions for a new game or reset.
     */
    void ResetBallPositions();

    // Getter for ball positions
    const std::vector<glm::vec3> &GetBallPositions() const { return ballPositions; }

    std::vector<Ball *> balls;
private:
    // Table object
    ObjectLoader *table;
    // Vector of ball models
    std::vector<glm::vec3> ballPositions; // Positions of the balls
    Renderer *renderer{nullptr}; // Renderer to use for drawing
};

#endif //BILLIARDSHOW_SCENE_H


