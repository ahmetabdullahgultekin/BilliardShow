#include "Scene.h"
#include <future>
#include <vector>

Scene::Scene() : table(nullptr) {}

Scene::~Scene() {
    delete table;
    for (auto *ball: balls)
        delete ball;
}

void Scene::Init() {
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

    // Use futures to load balls in parallel
    std::vector<std::future<Model3D *>> futures;
    for (int i = 0; i < ballPositions.size(); ++i) {
        futures.push_back(std::async(std::launch::async, [i]() {
            std::string objFile = "assets/Ball" + std::to_string(i + 1) + ".obj";
            Model3D *model = new Model3D();
            if (!model->Load(objFile)) {
                std::cerr << "Could not load " << objFile << std::endl;
                delete model;
                return static_cast<Model3D *>(nullptr);
            }
            return model;
        }));
    }
    for (int i = 0; i < balls.size(); ++i) {
        balls[i] = futures[i].get();
    }
}

void Scene::Render() {
    // Draw table base
    // if (table) table->Render(glm::vec3(0.0f), 1.0f);
    // Or:
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

void Scene::SetRenderer(Renderer *renderer) {
    this->renderer = renderer;
}

// In Scene.cpp
void Scene::InstallBalls() {
    for (auto *ball: balls)
        if (ball) ball->Install();
}

