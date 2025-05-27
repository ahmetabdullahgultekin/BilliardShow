/**
 * @file App.h
 * @brief Header file for the App class.
 * This class initializes the application, sets up the renderer, camera, minimap, and scene,
 * and runs the main application loop.
 * It handles user input, rendering, and scene management.
 * @author Ahmet Abdullah Gultekin
 * @date 2025-05-27
 * @version 1.0
 */
#ifndef BILLIARDSHOW_APP_H
#define BILLIARDSHOW_APP_H

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdio>
#include <chrono>
#include <thread>
#include <atomic>

#include "Renderer/Renderer.h"
#include "Renderer/Camera.h"
#include "UI/Minimap.h"
#include "Loader/ObjectLoader.h"
#include "Scene/Scene.h"
#include "Renderer/Texture.h"
#include "Utils/Logger.h"

// Define the Paths
#define ASSETS_PATH "assets/"
#define OBJ_PATH ASSETS_PATH "objects/"
#define IMAGE_PATH ASSETS_PATH "images/"
#define LOADING_IMAGE "loading16-9.png"
#define LOADING_IMAGE_PATH IMAGE_PATH LOADING_IMAGE
// Define the Window Size
#define WINDOW_WIDTH 1600.0f
#define WINDOW_HEIGHT 900.0f

class Renderer;

class Camera;

class Minimap;

class Scene;

/**
 * @class App
 * @brief Main application class for the BilliardShow simulation.
 * This class initializes the application, sets up the renderer, camera, minimap, and scene,
 * and runs the main application loop.
 */
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
    Scene *scene;
};

#endif //BILLIARDSHOW_APP_H
