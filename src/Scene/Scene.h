#ifndef BILLIARDSHOW_SCENE_H
#define BILLIARDSHOW_SCENE_H

#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <iostream>

#include "../Renderer/Renderer.h"
#include "../Loader/Model3D.h"

class Scene {
public:
    Scene();

    ~Scene();

    void Init(); // Load/install all balls
    void Render(); // Draw table and balls
    void InstallBalls();

    void SetRenderer(Renderer *renderer);

private:
    Model3D *table;
    std::vector<Model3D *> balls;
    std::vector<glm::vec3> ballPositions; // One for each ball
    Renderer *renderer;
};

#endif //BILLIARDSHOW_SCENE_H
