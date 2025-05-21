#ifndef BILLIARDSHOW_APP_H
#define BILLIARDSHOW_APP_H

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdio>

#include "Renderer/Renderer.h"
#include "Renderer/Camera.h"
#include "UI/Minimap.h"
#include "Loader/Model3D.h"
#include "Scene/Scene.h"


class App {
public:
    App();

    ~App();

    void Run();

private:
    Renderer *renderer;
    Camera *camera;
    float lastX = 0.0f, lastY = 0.0f;
    bool leftMousePressed = false;
    Minimap *minimap;
    Model3D *testBall;
    Scene *scene;
};


#endif //BILLIARDSHOW_APP_H
