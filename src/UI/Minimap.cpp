#include "Minimap.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include "../Utils/Logger.h"

// tableWidth, tableDepth: for camera setup
Minimap::Minimap(Renderer *renderer, float tableWidth, float tableDepth)
        : renderer(renderer), tableWidth(tableWidth), tableDepth(tableDepth) {}

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
    glFrontFace(GL_CCW); // Set front face to counter-clockwise
    glEnable(GL_BLEND); // Enable blending for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set blending function
    // 3. Disable depth testing and lighting for minimap
    glDisable(GL_DEPTH_TEST); // Disable depth testing for minimap
    glDisable(GL_LIGHTING); // Disable lighting for minimap

    // 3. (Optional) If you add lighting later, disable for minimap
    // glDisable(GL_LIGHTING);

    // 4. Draw the table (and later balls)
    renderer->DrawParallelepiped(glm::vec3(0.0f), glm::vec3(tableWidth, 0.2f, tableDepth));
}
