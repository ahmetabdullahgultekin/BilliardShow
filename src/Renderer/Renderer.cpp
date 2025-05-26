#include "Renderer.h"

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

static const unsigned int cubeIndices[] = {
        0, 1, 2, 2, 3, 0,        // Front (CCW)
        4, 7, 6, 6, 5, 4,        // Back (CCW)
        8, 9, 10, 10, 11, 8,        // Left (CCW, fixed)
        12, 15, 14, 14, 13, 12,        // Right (CCW, fixed)
        16, 17, 18, 18, 19, 16,        // Top (CCW)
        20, 23, 22, 22, 21, 20         // Bottom (CCW)
};

struct CubeGL {
    GLuint vao = 0, vbo = 0, ebo = 0;
    bool initialized = false;
} cubeGL;

Renderer::Renderer() {
    // If you want to set OpenGL state (enable depth, etc.), do here
    glEnable(GL_DEPTH_TEST);
}

Renderer::~Renderer() {}

const Camera *camera = nullptr;

void Renderer::SetCamera(const Camera *cam) { camera = cam; }

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
    shader->setBool("useTexture", false); // Disable texture for this example
    shader->setVec3("objectColor", glm::vec3(0.2f, 0.5f, 0.2f)); // Set object color
    shader->setMat4("model", glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), size));
    glBindVertexArray(cubeGL.vao);
    glDrawElements(GL_TRIANGLES, sizeof(cubeIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, nullptr);
    // Cleanup
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

