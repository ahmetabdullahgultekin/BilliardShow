#include "Scene.h"

Scene::Scene() : table(nullptr) {}

Scene::~Scene() {
    delete table;
    for (auto *ball: balls)
        delete ball;
}

void Scene::Render() {
    // Draw table base
    Shader *shader = Shader::GetActiveShader();
    glm::mat4 model = glm::mat4(1.0f);
    shader->setMat4("model", model);
    shader->setVec3("objectColor", glm::vec3(0.2f, 0.5f, 0.2f)); // Table color
    shader->setBool("useTexture", false); // Disable texture for table
    glBindTexture(GL_TEXTURE_2D, 0);

    if (!renderer) {
        Logger::Error("Renderer is not set in Scene::Render");
        return;
    }

    renderer->DrawParallelepiped(glm::vec3(0.0f),
                                 glm::vec3(Table::OUTER_LENGTH, Table::OUTER_HEIGHT, Table::OUTER_WIDTH));

    // Draw balls
    // Configure shader for ball rendering
    shader->setBool("useTexture", true); // Enable texture for balls
    shader->setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f)); // Default ball color
    shader->setMat4("model", glm::mat4(1.0f)); // Reset model matrix

    if (balls.empty()) {
        Logger::Error("No balls to render in Scene::Render");
        return;
    }

    for (int i = 0; i < balls.size(); ++i) {
        if (balls[i]) {
            balls[i]->Render(renderer, Constants::BALL_SCALE); // Render each ball with scale 1.0
        } else {
            Logger::Error("Ball at index " + std::to_string(i) + " is null in Scene::Render");
        }
    }
}

void Scene::SetRenderer(Renderer *_renderer) {
    if (!_renderer) {
        Logger::Error("Attempted to set null renderer in Scene::SetRenderer");
        return;
    }
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
    float startY = Table::OUTER_HEIGHT / 2.0f + Ball::RADIUS; // Centered above the table surface

    ballPositions.clear();
    for (int row = 0; row < 5; ++row) {
        float z = startZ + rowSpacing * row;
        float xOffset = -colSpacing * row / 2.0f;
        for (int col = 0; col <= row; ++col) {
            float x = startX + xOffset + col * colSpacing;
            // Ensure the center is above the table by Ball::RADIUS
            ballPositions.push_back({x, startY, z});
            if (ballPositions.size() >= 15) break;
        }
        if (ballPositions.size() >= 15) break;
    }

    // Create Ball objects and assign models/textures
    balls.resize(ballPositions.size());
/*    Ball* sharedBall = new Ball(0, ballPositions[0]);
    Model3D* sharedBallModel = new Model3D();
    sharedBallModel->Load(OBJ_PATH "Ball1.obj"); // Load a default model
    sharedBall->SetModel(sharedBallModel);
    balls[0] = sharedBall; // The First ball is the cue ball*/
    int numBalls = (int) ballPositions.size();
    for (int i = 0; i < numBalls; ++i) {
        Ball *ball = new Ball(i + 1, ballPositions[i]);
        Model3D *model = new Model3D(); // Copy the shared model
        std::string objPath = OBJ_PATH "Ball" + std::to_string(i % 15 + 1) + ".obj"; // Cycle through 15 ball models
        //std::string texPath = OBJ_PATH "PoolBalluv" + std::to_string(i % 15 + 1) + ".jpg"; // Cycle through textures
        model->Load(objPath);
        //model->SetTexture(texPath);
        //model->Install();
        ball->SetModel(model);
        /*ball->SetPosition(ballPositions[i]);
        ball->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));*/
        balls[i] = ball;
        if (progress) *progress = float(i + 1) / (float) numBalls;
        Logger::Info("Loaded ball model " + std::to_string(i + 1));
    }
    //delete sharedBallModel; // Clean up if not needed
    Logger::Info("All ball models loaded and assigned.");

    if (done) *done = true;
}

void Scene::Update(float deltaTime) {
    // Move and apply friction
    for (auto *ball: balls) {
        if (ball) {
            ball->Update(deltaTime);
            ball->ApplyFriction(deltaTime, Constants::BALL_FRICTION);
            ball->ApplyRollingFriction(deltaTime, Constants::BALL_ROLLING_FRICTION);
        }
    }
    // Ball-ball collisions
    for (size_t i = 0; i < balls.size(); ++i) {
        for (size_t j = i + 1; j < balls.size(); ++j) {
            if (balls[i] && balls[j]) {
                balls[i]->ResolveBallCollision(*balls[j]);
            }
        }
    }
    // Ball-table collisions
    Table tableObj; // Or use your existing table pointer if available
    for (auto *ball: balls) {
        if (ball) {
            ball->ResolveTableCollision(tableObj);
        }
    }
}

void Scene::ResetBallPositions() {
    // Reset all balls to their initial positions and zero velocity
    for (size_t i = 0; i < balls.size() && i < ballPositions.size(); ++i) {
        if (balls[i]) {
            balls[i]->SetPosition(ballPositions[i]);
            balls[i]->SetVelocity(glm::vec3(0.0f, 0.0f, 0.5f)); // Reset to initial velocity
        }
    }
}

