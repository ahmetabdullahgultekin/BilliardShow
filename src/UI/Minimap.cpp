#include "Minimap.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

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
            glm::vec3(0.0f, 0.0f, 0.0f), // Look at center
            glm::vec3(0.0f, 0.0f, -1.0f) // Up is -Z for top view
    );

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(&miniProj[0][0]);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(&miniView[0][0]);

    // 3. (Optional) If you add lighting later, disable for minimap
    // glDisable(GL_LIGHTING);

    // 4. Draw the table (and later balls)
    renderer->DrawParallelepiped(glm::vec3(0.0f), glm::vec3(tableWidth, 0.2f, tableDepth));

    // glEnable(GL_LIGHTING); // If you disabled it above

    // 5. Restore full viewport will be handled by the main loop after all drawing
}