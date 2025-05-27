/**
 * @file Minimap.cpp
 * @brief Implementation of the Minimap class for rendering a top-down view of the billiard table.
 * This class handles the rendering of a minimap that shows the positions of the balls on the table.
 * It uses an orthographic projection to create a top-down view of the scene.
 * @author Ahmet Abdullah Gultekin
 * @date 2025-05-27
 * @version 1.0
 */
#include "Minimap.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include "../Scene/Scene.h"

// tableWidth, tableDepth: for camera setup
Minimap::Minimap(Renderer *renderer, float tableWidth, float tableDepth)
        : renderer(renderer), tableWidth(tableWidth), tableDepth(tableDepth) {}

/**
 * @fn Render
 * @brief Renders the minimap in the top-right corner of the window.
 * This method sets up a small viewport for the minimap,
 * configures an orthographic projection for a top-down view,
 * and draws the billiard table and balls as simple shapes.
 * @param windowWidth
 * @param windowHeight
 */
void Minimap::Render(int windowWidth, int windowHeight) {
    // 1. Set a small viewport (top-right)
    int miniSize = windowHeight / 4;
    glViewport(windowWidth - miniSize - 10, windowHeight - miniSize - 10, miniSize, miniSize);

    // 2. Top-down orthographic projection
    float margin = 0.1f; // leave a bit of space around the table
    glm::mat4 miniProj = glm::ortho(
            -tableWidth / 2 - margin, tableWidth / 2 + margin,
            -tableDepth / 2 - margin, tableDepth / 2 + margin,
            -10.0f, 10.0f
    );
    glm::mat4 miniView = glm::lookAt(
            glm::vec3(0.0f, 5.0f, 0.0f), // From above
            glm::vec3(0.0f, 0.0f, 0.0f), // Look at a center
            glm::vec3(0.0f, 0.0f, -1.0f) // Up is -Z for top view
    );

    // Set the projection and view matrices in the shader
    if (!renderer) {
        Logger::Error("Renderer is not set in Minimap::Render");
        return;
    }
    Shader *shader = Shader::GetActiveShader();
    if (!shader) {
        Logger::Error("No active shader set for minimap rendering!");
        return;
    }
    shader->setMat4("projection", miniProj);
    shader->setMat4("view", miniView);
    shader->setBool("useTexture", false); // Disable texture for minimap
    shader->setVec3("objectColor", glm::vec3(0.2f, 0.5f, 0.2f)); // Table color
    // Set the camera position in the shader
    shader->setVec3("cameraPos", glm::vec3(0.0f, 5.0f, 0.0f)); // Camera position for minimap
    // Set the light position in the shader
    shader->setVec3("lightPos", glm::vec3(0.0f, 10.0f, 10.0f)); // Example light position
    shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f)); // White light
    shader->setVec3("objectColor", glm::vec3(0.2f, 0.5f, 0.2f)); // Example object color
    glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D rendering
    glEnable(GL_CULL_FACE); // Enable backface culling for better performance
    glCullFace(GL_BACK); // Cull back faces
    glFrontFace(GL_CCW); // Set the front face to counter-clockwise
    glEnable(GL_BLEND); // Enable blending for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set blending function
    // 3. Disable depth testing and lighting for minimap
    glDisable(GL_DEPTH_TEST); // Disable depth testing for minimap
    glDisable(GL_LIGHTING); // Disable lighting for minimap

    // 4. Draw the table (and later balls)
    renderer->DrawParallelepiped(glm::vec3(0.0f), glm::vec3(tableWidth, 0.2f, tableDepth));

    // 5. Draw balls as simple circles
    if (!ballPositions->empty()) {
        float ballRadius = 0.057f; // Approximate pool ball radius in meters
        for (const auto &pos: *ballPositions) {
            shader->use();
            shader->setBool("useTexture", false); // Ensure color, not texture
            shader->setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f)); // White ball
            shader->setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, 0.11f, pos.z)) *
                                     glm::scale(glm::mat4(1.0f), glm::vec3(ballRadius)));
            renderer->DrawCircle2D(glm::vec3(0.0f), 1.0f); // Draw unit circle at origin, transformed by model
        }
    } else {
        Logger::Error("No ball positions set for minimap rendering");
    }
}

/**
 * @fn SetBallPositions
 * @brief Sets the positions of the balls for rendering in the minimap.
 * This method allows the minimap to access the current positions of the balls
 * to render them correctly on the minimap.
 * @param positions Pointer to a vector containing ball positions.
 */
void Minimap::SetBallPositions(const std::vector<glm::vec3> *positions) {
    ballPositions = positions;
}

