#include "App.h"

static Camera *g_camera = nullptr;
static bool *g_leftMousePressed = nullptr;
static float *g_lastX = nullptr;
static float *g_lastY = nullptr;

void mouse_button_callback(GLFWwindow *, int button, int action, int) {
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        *g_leftMousePressed = (action == GLFW_PRESS);
}

void cursor_position_callback(GLFWwindow *, double xpos, double ypos) {
    if (*g_leftMousePressed) {
        float deltaX = (float) xpos - *g_lastX;
        float deltaY = (float) ypos - *g_lastY;
        g_camera->ProcessMouseMovement(deltaX, deltaY);
    }
    *g_lastX = (float) xpos;
    *g_lastY = (float) ypos;
}

void scroll_callback(GLFWwindow *, double, double yoffset) {
    g_camera->ProcessMouseScroll((float) yoffset);
}

App::App() {
    renderer = new Renderer();
    camera = new Camera(1600.0f / 900.0f);
    minimap = new Minimap(renderer, 3.0f, 1.5f); // Table width & depth
    testBall = new Model3D();
    testBall->Load("assets/Ball1.obj");   // Path to your OBJ file
    scene = new Scene();
}

App::~App() {
    delete renderer;
    delete camera;
    delete minimap;
    delete testBall;
    delete scene;
}

void App::Run() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return;
    }

    GLFWwindow *window = glfwCreateWindow(1600, 900, "BilliardShow", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return;
    }

    // Set up globals for callbacks
    g_camera = camera;
    g_leftMousePressed = &leftMousePressed;
    g_lastX = &lastX;
    g_lastY = &lastY;

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        return;
    }

    //testBall->Install();
    scene->Init();
    scene->InstallBalls();
    scene->SetRenderer(renderer);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // --- Main Scene ---
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        // ---- NEW: Set camera matrices before drawing objects ----
        glm::mat4 proj = camera->GetProjectionMatrix();
        glm::mat4 view = camera->GetViewMatrix();
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(&proj[0][0]);
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(&view[0][0]);

        // ---- Now draw your table ----
        //renderer->DrawParallelepiped(glm::vec3(0.0f), glm::vec3(3.0f, 0.2f, 1.5f));

        // ---- Draw the ball ----
        //testBall->Render(glm::vec3(0.0f, 0.2f, 0.0f)); // Slightly above the table
        scene->Render();

        // ---- Draw the minimap ----
        minimap->Render(width, height);

        // --- Restore full viewport if you draw UI/text later ---
        glViewport(0, 0, width, height);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}