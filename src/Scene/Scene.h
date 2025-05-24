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
#include "../Loader/Model3D.h"
#include "../App.h"
#include "../Scene/Table.h"
#include "../Utils/Logger.h"
#include "Ball.h"

class Scene {
public:
    Scene();

    ~Scene();

    void Init(); // Load/install all balls
    void Render(); // Draw table and balls
    void InstallBalls();

    void LoadBallsThreaded(std::atomic<float> *progress, std::atomic<bool> *done);

    void SetRenderer(Renderer *renderer);

private:
    Model3D *table;
    std::vector<Model3D *> balls; // Vector of ball models
    std::vector<glm::vec3> ballPositions; // Positions of the balls
    Renderer *renderer{nullptr}; // Renderer to use for drawing
};

#endif //BILLIARDSHOW_SCENE_H

