#include "Scene.h"

Scene::Scene() : table(nullptr) {}

Scene::~Scene() {
    delete table;
    for (auto *ball: balls)
        delete ball;
}

/*
void Scene::Init() {
    using namespace std::chrono;
    auto t_start = high_resolution_clock::now();

    // Place balls for match start (triangle formation, apex at head spot)
    float ballRadius = 0.028575f; // Standard pool ball radius in meters (57.15mm diameter)
    float rowSpacing = ballRadius * 2.0f + 0.001f; // Small gap between rows
    float colSpacing = ballRadius * 2.0f + 0.001f; // Small gap between balls in a row
    float tableLength = Table::PLAY_LENGTH * 0.01f; // Convert cm to meters
    float headSpotZ = -tableLength / 2.0f + tableLength / 4.0f;
    float startX = 0.0f;
    float startZ = headSpotZ;
    balls.clear();
    int ballNum = 1;
    for (int row = 0; row < 5; ++row) {
        float z = startZ + rowSpacing * row;
        float xOffset = -colSpacing * row / 2.0f;
        for (int col = 0; col <= row; ++col) {
            float x = startX + xOffset + col * colSpacing;
            balls.emplace_back(ballNum, glm::vec3(x, 0.25f, z));
            ++ballNum;
            if (ballNum > 15) break;
        }
        if (ballNum > 15) break;
    }

    // --- Responsive loading: show progress while loading balls ---
    extern void DrawLoadingScreen(GLFWwindow* window, Texture* loadingTexture, float spinnerAngle, float progress);
    extern GLFWwindow* g_loadingWindow;
    extern Texture* g_loadingTexture;
    int numBalls = (int)balls.size();
    float spinnerAngle = 0.0f;
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
*/

void Scene::Render() {
    // Draw table base
    Shader *shader = Shader::GetActiveShader();
    /*glPushMatrix();
    glColor3f(0.2f, 0.5f, 0.2f); // Table color
    glTranslatef(0.0f, 0.0f, 0.0f);*/
    glm::mat4 model = glm::mat4(1.0f);
    shader->setMat4("model", model);
    shader->setVec3("objectColor", glm::vec3(0.2f, 0.5f, 0.2f)); // Table color
    shader->setBool("useTexture", false); // Disable texture for table

    //glScalef(Table::OUTER_LENGTH,Table::OUTER_HEIGHT,Table::OUTER_WIDTH);
    renderer->DrawParallelepiped(glm::vec3(0.0f),
                                 glm::vec3(Table::OUTER_LENGTH, Table::OUTER_HEIGHT, Table::OUTER_WIDTH));
    /*glPopMatrix();*/

    // Draw balls (after calling Install() after context/glew!)
    // Configure shader for ball rendering
    shader->setBool("useTexture", true); // Enable texture for balls
    shader->setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f)); // Default ball color
    shader->setMat4("model", glm::mat4(1.0f)); // Reset model matrix

    if (!renderer) {
        Logger::Error("Renderer is not set in Scene::Render");
        return;
    }
    if (balls.empty()) {
        Logger::Error("No balls to render in Scene::Render");
        return;
    }
    if (ballPositions.empty()) {
        Logger::Error("Ball positions are empty in Scene::Render");
        return;
    }

    for (int i = 0; i < balls.size(); ++i) {
        if (balls[i]) {
            balls[i]->Render(ballPositions[i], 0.04f); // Render each ball using the renderer
        }
    }
}

void Scene::SetRenderer(Renderer *_renderer) {
    this->renderer = _renderer;
}

// In Scene.cpp
void Scene::InstallBalls() {
    // No longer needed, handled in LoadBallsThreaded
    for (auto *ball: balls)
        if (ball) ball->Install();
        else Logger::Error("Ball is null in InstallBalls");
}

void Scene::LoadBallsThreaded(std::atomic<float> *progress, std::atomic<bool> *done) {
    // Place balls for match start (triangle formation, apex at a head spot)
    float rowSpacing = Ball::RADIUS * 2.0f + 0.001f; // Small gap between rows
    float colSpacing = Ball::RADIUS * 2.0f + 0.001f; // Small gap between balls in a row
    float tableLength = Table::OUTER_WIDTH; // in cm
    float headSpotZ = -tableLength / 2.0f + tableLength / 4.0f;
    float startX = 0; // Centered on the table
    float startZ = 0; // Start at the head spot

    ballPositions.clear();
    for (int row = 0; row < 5; ++row) {
        float z = startZ + rowSpacing * row;
        float xOffset = -colSpacing * row / 2.0f;
        for (int col = 0; col <= row; ++col) {
            float x = startX + xOffset + col * colSpacing;
            ballPositions.push_back({x, 0.25f, z});
            if (ballPositions.size() >= 15) break;
        }
        if (ballPositions.size() >= 15) break;
    }

    // Create Ball objects and assign models/textures
    balls.resize(ballPositions.size());
    Model3D *sharedBallModel = new Model3D();
    if (!sharedBallModel->Load(OBJ_PATH "Ball1.obj")) {
        Logger::Error("Failed to load ball model from " OBJ_PATH "Ball1.obj");
        if (done) *done = false;
        return;
    }
    int numBalls = (int) ballPositions.size();
    for (int i = 0; i < numBalls; ++i) {
        balls[i] = new Model3D(*sharedBallModel); // Use a copy constructor or a clone method
        std::string texPath = std::string(OBJ_PATH) + "PoolBalluv" + std::to_string(i + 1) + ".jpg";
        balls[i]->SetTexture(texPath);
        if (progress) *progress = float(i + 1) / (float) numBalls;
        Logger::Info("Loaded ball model " + std::to_string(i + 1));
    }
    delete sharedBallModel; // Clean up if not needed
    Logger::Info("All ball models loaded and assigned.");

    if (done) *done = true;
}

void Scene::Update(float deltaTime) {
/*    // Move and apply friction
    for (auto* ball : balls) {
        ball->Update(deltaTime);
        ball->ApplyFriction(deltaTime);
    }
    // Ball-ball collisions
    for (size_t i = 0; i < balls.size(); ++i) {
        for (size_t j = i + 1; j < balls.size(); ++j) {
            balls[i]->ResolveBallCollision(*balls[j]);
        }
    }
    // Ball-table collisions
    Table tableObj; // Or use your existing table pointer if available
    for (auto* ball : balls) {
        ball->ResolveTableCollision(tableObj);
    }*/
}
