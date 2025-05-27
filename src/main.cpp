/**
 * @file main.cpp
 * @brief Main entry point for the BilliardShow application.
 * This file initializes the application, sets up the logger, and runs the main application loop.
 * @author Ahmet Abdullah Gultekin
 * @date 2025-05-27
 * @version 1.0
 * @details
 * The main function initializes the Logger, creates an instance of the App class,
 * and calls its Run method to start the application.
 * It also logs the start and exit of the application.
 * @note
 * This file is part of the BilliardShow project, which simulates a billiard game using OpenGL.
 * It assumes that the OpenGL context is already created and initialized before running the application.
 * * @see App.h for the App class definition and member functions.
 * @see Logger.h for the Logger class used for logging messages.
 * * @see App.cpp for the implementation of the App class methods.
 * * @see Logger.cpp for the implementation of the Logger class methods.
 * * @see Renderer.h for the Renderer class used for rendering 3D objects.
 * * @see Camera.h for the Camera class used for managing the camera view.
 * * @see Minimap.h for the Minimap class used for displaying a minimap of the scene.
 * * @see Scene.h for the Scene class used for managing the game scene and objects.
 * * @see ObjectLoader.h for the ObjectLoader class used for loading 3D models.
 * * @see Texture.h for the Texture class used for managing textures in OpenGL.
 */
#include "App.h"

/**
 * @fn main
 * @brief Main entry point for the BilliardShow application.
 * Initializes the Logger, creates an instance of the App class,
 * and calls its Run method to start the application.
 * Logs the start and exit of the application.
 * @param argc Argument count.
 * @param argv Argument vector (not used).
 * @return Exit status of the application (0 for success).
 */
int main(int argc, char **argv) {
    Logger::Info("BilliardShow started.");
    App app;
    app.Run();
    Logger::Info("BilliardShow exited.");
    return 0;
}