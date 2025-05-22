#include "Scene.h"
#include "../App.h"
#include <future>
#include <vector>
#include <chrono>
#include <thread>
#include <atomic>

Scene::Scene() : table(nullptr) {}

Scene::~Scene() {
    delete table;
    for (auto *ball: balls)
        delete ball;
}

void Scene::Init() {
    using namespace std::chrono;
    auto t_start = high_resolution_clock::now();
    // Load table (or just use DrawParallelepiped if you don't have a table model)
    // table = new Model3D();
    // table->Load("assets/Table.obj"); // if you have one
    // table->Install();

    // Place balls
    ballPositions = {
            {0.0f,   0.25f, 0.0f}, // Ball 1
            {0.1f,   0.25f, 0.0f}, // Ball 2
            {-0.1f,  0.25f, 0.0f}, // Ball 3
            {0.2f,   0.25f, 0.05f}, // Ball 4
            {-0.2f,  0.25f, -0.05f}, // Ball 5
            {0.15f,  0.25f, 0.1f}, // Ball 6
            {0.25f,  0.25f, -0.15f}, // Ball 7
            {-0.15f, 0.25f, 0.15f}, // Ball 8
            {0.12f,  0.25f, -0.12f}, // Ball 9
            {-0.12f, 0.25f, 0.12f}, // Ball 10
            {0.18f,  0.25f, 0.18f}, // Ball 11
            {-0.18f, 0.25f, -0.18f}, // Ball 12
            {0.22f,  0.25f, 0.22f}, // Ball 13
            {-0.22f, 0.25f, -0.22f}, // Ball 14
            {0.0f,   0.25f, 0.2f} // Ball 15
    };
    balls.resize(ballPositions.size());

    // --- Responsive loading: show progress while loading balls ---
    extern void DrawLoadingScreen(GLFWwindow* window, Texture* loadingTexture, float spinnerAngle, float progress);
    extern GLFWwindow* g_loadingWindow;
    extern Texture* g_loadingTexture;
    int numBalls = (int)balls.size();
    float spinnerAngle = 0.0f;
    for (int i = 0; i < numBalls; ++i) {
        balls[i] = new Model3D();
        if (!balls[i]->Load(OBJ_PATH "Ball1.obj")) {
            std::cerr << "Could not load " << OBJ_PATH "Ball1.obj" << std::endl;
        }
        std::string texPath = std::string(OBJ_PATH) + "PoolBalluv" + std::to_string(i + 1) + ".jpg";
        balls[i]->SetTexture(texPath);
        // Responsive loading: update the loading screen after each ball
        if (g_loadingWindow && g_loadingTexture) {
            float progress = (float)(i + 1) / numBalls;
            DrawLoadingScreen(g_loadingWindow, g_loadingTexture, spinnerAngle, progress);
            spinnerAngle += 30.0f;
        }
    }
}

void Scene::Render() {
    // Draw table base
    glPushMatrix();
    glColor3f(0.2f, 0.5f, 0.2f); // Table color
    glTranslatef(0.0f, 0.0f, 0.0f);
    glScalef(3.0f, 0.2f, 1.5f);
    renderer->DrawParallelepiped(glm::vec3(0.0f), glm::vec3(3.0f, 0.2f, 1.5f));
    glPopMatrix();

    // Draw balls (after calling Install() after context/glew!)
    for (int i = 0; i < balls.size(); ++i) {
        if (balls[i]) balls[i]->Render(ballPositions[i], 0.06f); // scale!
    }
}

void Scene::SetRenderer(Renderer *_renderer) {
    this->renderer = _renderer;
}

// In Scene.cpp
void Scene::InstallBalls() {
    for (auto *ball: balls)
        if (ball) ball->Install();
}

void Scene::LoadBallsThreaded(std::atomic<float>* progress, std::atomic<bool>* done) {
    // Place balls
    ballPositions = {
            {0.0f,   0.25f, 0.0f}, // Ball 1
            {0.1f,   0.25f, 0.0f}, // Ball 2
            {-0.1f,  0.25f, 0.0f}, // Ball 3
            {0.2f,   0.25f, 0.05f}, // Ball 4
            {-0.2f,  0.25f, -0.05f}, // Ball 5
            {0.15f,  0.25f, 0.1f}, // Ball 6
            {0.25f,  0.25f, -0.15f}, // Ball 7
            {-0.15f, 0.25f, 0.15f}, // Ball 8
            {0.12f,  0.25f, -0.12f}, // Ball 9
            {-0.12f, 0.25f, 0.12f}, // Ball 10
            {0.18f,  0.25f, 0.18f}, // Ball 11
            {-0.18f, 0.25f, -0.18f}, // Ball 12
            {0.22f,  0.25f, 0.22f}, // Ball 13
            {-0.22f, 0.25f, -0.22f}, // Ball 14
            {0.0f,   0.25f, 0.2f} // Ball 15
    };
    balls.resize(ballPositions.size());
    int numBalls = (int)balls.size();
    for (int i = 0; i < numBalls; ++i) {
        balls[i] = new Model3D();
        balls[i]->Load(OBJ_PATH "Ball1.obj");
        std::string texPath = std::string(OBJ_PATH) + "PoolBalluv" + std::to_string(i + 1) + ".jpg";
        balls[i]->SetTexture(texPath);
        if (progress) *progress = float(i + 1) / numBalls;
    }
    if (done) *done = true;
}

