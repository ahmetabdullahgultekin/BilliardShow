#include "App.h"

static Camera *g_camera = nullptr;
static bool *g_leftMousePressed = nullptr;
static float *g_lastX = nullptr;
static float *g_lastY = nullptr;
GLFWwindow *g_loadingWindow = nullptr;
Texture *g_loadingTexture = nullptr;

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

void DrawLoadingScreen(GLFWwindow *window, Texture *loadingTexture, float spinnerAngle, float progress) {
    int winW, winH;
    glfwGetFramebufferSize(window, &winW, &winH);
    glViewport(0, 0, winW, winH);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, winW, 0, winH, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw image to cover the entire background
    if (loadingTexture && loadingTexture->IsValid()) {
        loadingTexture->Bind();
        glEnable(GL_TEXTURE_2D);
        glColor3f(1, 1, 1);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 1);
        glVertex2i(0, 0);
        glTexCoord2f(1, 1);
        glVertex2i(winW, 0);
        glTexCoord2f(1, 0);
        glVertex2i(winW, winH);
        glTexCoord2f(0, 0);
        glVertex2i(0, winH);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }

    // Draw beautiful spinner (arc tail + center circle)
    glPushMatrix();
    glTranslatef(winW / 2.0f, winH / 2.0f, 0);
    int numArcs = 8;
    float radius = 32.0f;
    float arcLen = 0.7f;
    float arcStep = 2 * 3.14159f / numArcs;
    glLineWidth(4.0f);
    for (int i = 0; i < numArcs; ++i) {
        float alpha = 1.0f - (float) i / numArcs;
        glColor4f(0.9f, 0.8f, 0.2f, alpha);
        glPushMatrix();
        glRotatef(spinnerAngle - i * (360.0f / numArcs), 0, 0, 1);
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j <= 12; ++j) {
            float a = arcLen * (float) j / 12.0f;
            glVertex2f(cosf(a) * radius, sinf(a) * radius);
        }
        glEnd();
        glPopMatrix();
    }
    // Draw the center circle
    glColor4f(0.9f, 0.8f, 0.2f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 0);
    for (int i = 0; i <= 20; ++i) {
        float a = i * 2 * 3.14159f / 20.0f;
        glVertex2f(cosf(a) * 8.0f, sinf(a) * 8.0f);
    }
    glEnd();
    glLineWidth(1.0f);
    glPopMatrix();

    // Optionally: Draw a progress bar at the bottom
    glColor3f(0.9f, 0.8f, 0.2f);
    int barW = (int) (winW * 0.6f);
    int barH = 16;
    int barX = (winW - barW) / 2;
    int barY = winH / 6;
    glBegin(GL_QUADS);
    glVertex2i(barX, barY);
    glVertex2i(barX + (int) (barW * progress), barY);
    glVertex2i(barX + (int) (barW * progress), barY + barH);
    glVertex2i(barX, barY + barH);
    glEnd();

    glfwSwapBuffers(window);
    glfwPollEvents();
}

App::App() {
    renderer = new Renderer();
    camera = new Camera(WINDOW_WIDTH / WINDOW_HEIGHT);
    minimap = new Minimap(renderer, Table::OUTER_WIDTH, Table::OUTER_HEIGHT);
    scene = new Scene();
}

App::~App() {
    delete renderer;
    delete camera;
    delete minimap;
    delete scene;
}

void App::Run() {
    if (!glfwInit()) {
        Logger::Error("Failed to initialize GLFW");
        return;
    }

    GLFWwindow *window = glfwCreateWindow(
            WINDOW_WIDTH, WINDOW_HEIGHT,
            "BilliardShow", nullptr, nullptr);

    if (!window) {
        Logger::Error("Failed to create window");
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
        Logger::Error("Failed to initialize GLEW");
        return;
    }

    // Load loading image
    Texture loadingTexture;
    if (!loadingTexture.LoadFromFile(LOADING_IMAGE_PATH)) {
        Logger::Error(std::string("Failed to load loading image ") + LOADING_IMAGE_PATH);
        return;
    }
    g_loadingWindow = window;
    g_loadingTexture = &loadingTexture;

    // --- Threaded asset loading ---
    std::atomic<float> progress(0.0f);
    std::atomic<bool> done(false);
    std::thread loader([&]() {
        scene->LoadBallsThreaded(&progress, &done);
    });
    float spinnerAngle = 0.0f;
    while (!done) {
        DrawLoadingScreen(window, &loadingTexture, spinnerAngle, progress.load());
        spinnerAngle += 30.0f;
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }
    loader.join();

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

        // ---- Draw the scene ----
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

