/**
 * @file Renderer.cpp
 * @brief Implementation of the Renderer class for rendering 3D objects.
 * This file contains methods to draw a cube (parallelepiped) and a 2D circle.
 * It uses OpenGL for rendering and assumes a shader is set up for rendering.
 * @author Ahmet Abdullah Gultekin
 * @date 2025-05-27
 * @details
 */
#include "Renderer.h"
#include <cmath>

// Helper function to set color per face
static const GLfloat faceColors[6][3] = {
        {1.0f, 0.0f, 0.0f}, // Red
        {0.0f, 1.0f, 0.0f}, // Green
        {0.0f, 0.0f, 1.0f}, // Blue
        {1.0f, 1.0f, 0.0f}, // Yellow
        {1.0f, 0.0f, 1.0f}, // Magenta
        {0.0f, 1.0f, 1.0f}  // Cyan
};

// Vertex data for a unit cube (centered at origin)
static const float cubeVertices[] = {
        // positions         // normals         // texcoords
        // Front face
        -0.5f, -0.5f, 0.5f, 0, 0, 1, 0, 0,
        0.5f, -0.5f, 0.5f, 0, 0, 1, 1, 0,
        0.5f, 0.5f, 0.5f, 0, 0, 1, 1, 1,
        -0.5f, 0.5f, 0.5f, 0, 0, 1, 0, 1,
        // Back face
        -0.5f, -0.5f, -0.5f, 0, 0, -1, 0, 0,
        0.5f, -0.5f, -0.5f, 0, 0, -1, 1, 0,
        0.5f, 0.5f, -0.5f, 0, 0, -1, 1, 1,
        -0.5f, 0.5f, -0.5f, 0, 0, -1, 0, 1,
        // Left face
        -0.5f, -0.5f, -0.5f, -1, 0, 0, 0, 0,
        -0.5f, -0.5f, 0.5f, -1, 0, 0, 1, 0,
        -0.5f, 0.5f, 0.5f, -1, 0, 0, 1, 1,
        -0.5f, 0.5f, -0.5f, -1, 0, 0, 0, 1,
        // Right face
        0.5f, -0.5f, -0.5f, 1, 0, 0, 0, 0,
        0.5f, -0.5f, 0.5f, 1, 0, 0, 1, 0,
        0.5f, 0.5f, 0.5f, 1, 0, 0, 1, 1,
        0.5f, 0.5f, -0.5f, 1, 0, 0, 0, 1,
        // Top face
        -0.5f, 0.5f, -0.5f, 0, 1, 0, 0, 0,
        -0.5f, 0.5f, 0.5f, 0, 1, 0, 1, 0,
        0.5f, 0.5f, 0.5f, 0, 1, 0, 1, 1,
        0.5f, 0.5f, -0.5f, 0, 1, 0, 0, 1,
        // Bottom face
        -0.5f, -0.5f, -0.5f, 0, -1, 0, 0, 0,
        -0.5f, -0.5f, 0.5f, 0, -1, 0, 1, 0,
        0.5f, -0.5f, 0.5f, 0, -1, 0, 1, 1,
        0.5f, -0.5f, -0.5f, 0, -1, 0, 0, 1
};

// Indices for the cube vertices (6 faces, 2 triangles per face)
static const unsigned int cubeIndices[] = {
        0, 1, 2, 2, 3, 0,        // Front (CCW)
        4, 7, 6, 6, 5, 4,        // Back (CCW)
        8, 9, 10, 10, 11, 8,        // Left (CCW, fixed)
        12, 15, 14, 14, 13, 12,        // Right (CCW, fixed)
        16, 17, 18, 18, 19, 16,        // Top (CCW)
        20, 23, 22, 22, 21, 20         // Bottom (CCW)
};

/**
 * @struct CubeGL
 * @brief OpenGL resources for rendering a cube.
 * This structure holds the Vertex Array Object (VAO), Vertex Buffer Object (VBO),
 * Element Buffer Object (EBO) and a flag to check if initialized.
 * It is used to manage the OpenGL state for rendering a cube efficiently.
 */
struct CubeGL {
    GLuint vao = 0, vbo = 0, ebo = 0;
    bool initialized = false;
} cubeGL;

/**
 * @class Renderer
 * @brief Class for rendering 3D objects using OpenGL.
 * This class provides methods to draw a cube (parallelepiped) and a 2D circle.
 * It uses shaders for rendering and assumes a shader is set up before drawing.
 */
Renderer::Renderer() {
    // If you want to set OpenGL state (enable depth, etc.), do here
    glEnable(GL_DEPTH_TEST);
}

Renderer::~Renderer() = default;

/**
 * @brief Sets the camera for the renderer.
 * This method allows the renderer to use a specific camera for rendering.
 * @param cam Pointer to the Camera object to be used.
 */
const Camera *camera = nullptr;

/**
 * @fn SetCamera
 * @brief Sets the camera for the renderer.
 * This method allows the renderer to use a specific camera for rendering.
 * @param cam Pointer to the Camera object to be used.
 */
void Renderer::SetCamera(const Camera *cam) { camera = cam; }

/**
 * @fn DrawParallelepiped
 * @brief Draws a cube (parallelepiped) at the specified position with the given size.
 * This method initializes the OpenGL resources if not already done,
 * configures the shader, and draws the cube using the specified position and size.
 * @param position The position of the cube in world coordinates.
 * @param size The size of the cube in world coordinates.
 * @return void
 */
void Renderer::DrawParallelepiped(const glm::vec3 &position, const glm::vec3 &size) {
    // Use Shader for rendering
    if (!cubeGL.initialized) {
        // Initialize VAO, VBO, EBO for cube
        glGenVertexArrays(1, &cubeGL.vao);
        glGenBuffers(1, &cubeGL.vbo);
        glGenBuffers(1, &cubeGL.ebo);

        glBindVertexArray(cubeGL.vao);

        glBindBuffer(GL_ARRAY_BUFFER, cubeGL.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeGL.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // Texcoord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        cubeGL.initialized = true;
    }
    // Configure the shader
    Shader *shader = Shader::GetActiveShader();
    if (!shader) {
        Logger::Error("No active shader set for rendering!");
        return;
    }
    shader->use();
    shader->setBool("useTexture", false);
    shader->setVec3("objectColor", glm::vec3(0.2f, 0.5f, 0.2f)); // Set object color
    shader->setMat4("model", glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), size));
    glBindVertexArray(cubeGL.vao);
    glDrawElements(GL_TRIANGLES, sizeof(cubeIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, nullptr);
    // Cleanup
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/**
 * @fn DrawCircle2D
 * @brief Draws a 2D circle at the specified center with the given radius.
 * This method initializes the OpenGL resources if not already done,
 * configures the shader, and draws the circle using the specified center and radius.
 * @param center The center of the circle in world coordinates.
 * @param radius The radius of the circle in world coordinates.
 * @param segments The number of segments to approximate the circle.
 * @return void
 */
void Renderer::DrawCircle2D(const glm::vec3 &center, float radius, int segments) {
    static GLuint vao = 0, vbo = 0;
    static int lastSegments = 0;
    static int vertexCount = 0;
    if (vao == 0 || lastSegments != segments) {
        if (vao != 0) {
            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
        }
        std::vector<glm::vec3> vertices;
        vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f)); // Center
        for (int i = 0; i <= segments; ++i) {
            float angle = 2.0f * 3.1415926f * float(i) / float(segments);
            float x = cosf(angle);
            float z = sinf(angle);
            vertices.push_back(glm::vec3(x, 0.0f, z));
        }
        vertexCount = static_cast<int>(vertices.size());
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *) 0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
        lastSegments = segments;
    }

    Shader *shader = Shader::GetActiveShader();
    if (!shader) return;
    shader->use();
    shader->setMat4("model", glm::translate(glm::mat4(1.0f), center) *
                             glm::scale(glm::mat4(1.0f), glm::vec3(radius, 1.0f, radius)));
    shader->setBool("useTexture", false); // Balls in minimap should not use texture
    shader->setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f)); // Set color to white

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
    glBindVertexArray(0);
}
