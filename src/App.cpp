/**
 * @file App.cpp
 * @brief Implementation of the App class for the BilliardShow application.
 * This file contains the main application logic, including initialization,
 * event handling, and rendering.
 * @author Ahmet Abdullah Gultekin
 * @date 2025-05-27
 * @version 1.0
 */
#include "App.h"
#include "Renderer/Shader.h"

/* * @brief Global variables for mouse input handling.
 * These are used to track the camera state and mouse position.
 */
static Camera *g_camera = nullptr;
static bool *g_leftMousePressed = nullptr;
static float *g_lastX = nullptr;
static float *g_lastY = nullptr;

/** * @brief Initializes global variables for mouse input handling.
 * This function sets up the camera and mouse state variables.
 * @param camera Pointer to the Camera instance.
 * @param leftMousePressed Pointer to a boolean indicating if the left mouse button is pressed.
 * @param lastX Pointer to a float for the last X position of the mouse.
 * @param lastY Pointer to a float for the last Y position of the mouse.
 */
void mouse_button_callback(GLFWwindow *, int button, int action, int) {
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        *g_leftMousePressed = (action == GLFW_PRESS);
}

/** * @brief Callback function for mouse cursor position changes.
 * This function updates the camera orientation based on mouse movement
 * when the left mouse button is pressed.
 * @param xpos The new X position of the mouse cursor.
 * @param ypos The new Y position of the mouse cursor.
 */
void cursor_position_callback(GLFWwindow *, double xpos, double ypos) {
    if (*g_leftMousePressed) {
        float deltaX = (float) xpos - *g_lastX;
        float deltaY = (float) ypos - *g_lastY;
        g_camera->ProcessMouseMovement(deltaX, deltaY);
    }
    *g_lastX = (float) xpos;
    *g_lastY = (float) ypos;
}

/** * @brief Callback function for mouse scroll input.
 * This function adjusts the camera's zoom level based on the scroll input.
 * @param yoffset The amount of scroll input (positive or negative).
 */
void scroll_callback(GLFWwindow *, double, double yoffset) {
    g_camera->ProcessMouseScroll((float) yoffset);
}

/** * @brief Draws a loading screen with a spinner and progress bar.
 * This function renders a loading screen with a spinner animation
 * and an optional progress bar at the bottom.
 * @param window Pointer to the GLFW window.
 * @param loadingTexture Pointer to the texture for the loading background.
 * @param spinnerAngle The current angle of the spinner for animation.
 * @param progress The current progress value (0.0 to 1.0) for the progress bar.
 */
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

/** * @class App
 * @brief Main application class for the BilliardShow.
 * This class initializes the application, sets up the OpenGL context,
 * and runs the main loop for rendering and event handling.
 */
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

/**
 * @brief Runs the main application loop.
 * Initializes GLFW, creates a window, sets up callbacks,
 * and enters the main rendering loop.
 */
void App::Run() {
    // Initialize GLFW and create a window
    if (!glfwInit()) {
        Logger::Error("Failed to initialize GLFW");
        return;
    }

    // Set GLFW window hints for OpenGL version and profile
    GLFWwindow *window = glfwCreateWindow(
            WINDOW_WIDTH, WINDOW_HEIGHT,
            "BilliardShow", nullptr, nullptr);

    // Set the OpenGL context version
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

    // Set the initial mouse position
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Set the initial mouse position to the center of the window
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

    // --- Threaded asset loading ---
    std::atomic<float> progress(0.0f);
    std::atomic<bool> done(false);
    float spinnerAngle = 0.0f;

    // Create a shared context for the background thread
    GLFWwindow *bgWindow = glfwCreateWindow(1, 1, "", nullptr, window); // Share with mainWindow
    glfwHideWindow(bgWindow);
    std::thread bgThread([&]() {
        glfwMakeContextCurrent(bgWindow);
        // Now safe to make OpenGL calls in this thread
        scene->LoadBallsThreaded(&progress, &done);
        done.store(true); // Signal that loading is done
        glfwMakeContextCurrent(nullptr); // Optional: release context
    });

    // --- Main thread loading screen ---
    glfwMakeContextCurrent(window);
    while (!done.load()) {
        spinnerAngle += 1.0f; // Increment spinner angle
        if (spinnerAngle >= 360.0f) spinnerAngle = 0.0f; // Reset angle
        DrawLoadingScreen(window, &loadingTexture, spinnerAngle, progress.load());
    }
    bgThread.join(); // Wait for the background thread to finish
    glfwDestroyWindow(bgWindow); // Clean up the background window
    glfwMakeContextCurrent(window); // Switch back to the main window context
    // Clear texture after loading
    loadingTexture.Release();
    // --- End of threaded loading ---

    // Now, in the main thread, create and install balls (OpenGL calls)
    scene->InstallBalls(); // This should do all OpenGL-dependent work
    scene->SetRenderer(renderer);

    // Create and use the main shader
    Shader mainShader("shaders/basic.vert", "shaders/basic.frag");
    // Main loop
    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        auto deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // --- Main Scene ---
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        // Modern OpenGL: set camera matrices as uniforms, not with glMatrixMode
        glm::mat4 proj = camera->GetProjectionMatrix();
        glm::mat4 view = camera->GetViewMatrix();

        // Use the main shader
        mainShader.use();
        // Set the viewport to the full window size
        glViewport(0, 0, width, height);
        // Set the camera matrices as uniforms
        mainShader.setInt("texture1", 0);
        mainShader.setMat4("projection", proj);
        mainShader.setMat4("view", view);

        // --- Lighting toggles and parameters ---
        static bool enableAmbient = true;
        static bool enableDirectional = true;
        static bool enablePoint = true;
        static bool enableSpot = true;
        static bool wasKey1 = false, wasKey2 = false, wasKey3 = false, wasKey4 = false;

        // Handle key press for toggling lights
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            if (!wasKey1) {
                enableAmbient = !enableAmbient;
                wasKey1 = true;
            }
        } else { wasKey1 = false; }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
            if (!wasKey2) {
                enableDirectional = !enableDirectional;
                wasKey2 = true;
            }
        } else { wasKey2 = false; }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
            if (!wasKey3) {
                enablePoint = !enablePoint;
                wasKey3 = true;
            }
        } else { wasKey3 = false; }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
            if (!wasKey4) {
                enableSpot = !enableSpot;
                wasKey4 = true;
            }
        } else { wasKey4 = false; }

        // Set light uniforms
        mainShader.setBool("enableAmbient", enableAmbient);
        mainShader.setVec3("ambientColor", glm::vec3(0.15f, 0.15f, 0.15f));
        mainShader.setBool("enableDirectional", enableDirectional);
        mainShader.setVec3("dirLightDir", glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)));
        mainShader.setVec3("dirLightColor", glm::vec3(0.7f, 0.7f, 0.7f));
        mainShader.setBool("enablePoint", enablePoint);
        mainShader.setVec3("pointLightPos", glm::vec3(0.0f, 3.0f, 0.0f));
        mainShader.setVec3("pointLightColor", glm::vec3(1.0f, 0.9f, 0.7f));
        mainShader.setFloat("pointLightConstant", 1.0f);
        mainShader.setFloat("pointLightLinear", 0.09f);
        mainShader.setFloat("pointLightQuadratic", 0.032f);
        mainShader.setBool("enableSpot", enableSpot);
        mainShader.setVec3("spotLightPos", glm::vec3(0.0f, 3.0f, 3.0f));
        mainShader.setVec3("spotLightDir", glm::normalize(glm::vec3(0.0f, -1.0f, -1.0f)));
        mainShader.setVec3("spotLightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        mainShader.setFloat("spotLightCutoff", glm::cos(glm::radians(12.5f)));
        mainShader.setFloat("spotLightOuterCutoff", glm::cos(glm::radians(17.5f)));
        mainShader.setFloat("spotLightConstant", 1.0f);
        mainShader.setFloat("spotLightLinear", 0.09f);
        mainShader.setFloat("spotLightQuadratic", 0.032f);

        glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D rendering
        glEnable(GL_CULL_FACE); // Enable backface culling for better performance
        glCullFace(GL_BACK); // Cull back faces
        glFrontFace(GL_CCW); // Set the front face to counter-clockwise
        glEnable(GL_BLEND); // Enable blending for transparency
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set blending function

        // ---- Update physics ----
        scene->Update(deltaTime);
        minimap->SetBallPositions(&scene->GetBallPositions());

        // Place this at the top of your main loop, outside any if/else:
        static bool wasRPressed = false;

        // Handle key press for resetting positions
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            if (!wasRPressed) {
                scene->ResetBallPositions();
                wasRPressed = true;
            }
        } else {
            wasRPressed = false;
        }

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
