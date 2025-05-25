#include "Ball.h"
#include "../Renderer/Renderer.h"

Ball::Ball(int number, const glm::vec3 &position)
        : number(number), position(position), model(nullptr) {}

Ball::~Ball() {
    delete model;
}

void Ball::SetPosition(const glm::vec3 &pos) {
    position = pos;
}

glm::vec3 Ball::GetPosition() const {
    return position;
}

int Ball::GetNumber() const {
    return number;
}

void Ball::SetModel(Model3D *m) {
    if (model) delete model;
    model = m;
}

void Ball::Render(Renderer *renderer, float scale) {
    if (model && renderer) {
        model->Render(position, scale);
    } else {
        Logger::Error("Ball::Render called with null model or renderer");
    }
}

void Ball::SetVelocity(const glm::vec3 &vel) {
    velocity = vel;
}

glm::vec3 Ball::GetVelocity() const {
    return velocity;
}

void Ball::Update(float deltaTime) {
    position += velocity * deltaTime;
}

void Ball::ApplyFriction(float deltaTime, float friction) {
    float speed = glm::length(velocity);
    if (speed > 0.0f) {
        float drop = friction * deltaTime;
        speed = glm::max(0.0f, speed - drop);
        velocity = (speed > 0.0f) ? glm::normalize(velocity) * speed : glm::vec3(0.0f);
    }
}

void Ball::ResolveTableCollision(const Table &table) {
    // Use play area dimensions from Table static constants
    float minX = -Table::PLAY_WIDTH / 2.0f + RADIUS;
    float maxX = Table::PLAY_WIDTH / 2.0f - RADIUS;
    float minZ = -Table::PLAY_LENGTH / 2.0f + RADIUS;
    float maxZ = Table::PLAY_LENGTH / 2.0f - RADIUS;
    if (position.x < minX) {
        position.x = minX;
        velocity.x *= -1.0f;
    }
    if (position.x > maxX) {
        position.x = maxX;
        velocity.x *= -1.0f;
    }
    if (position.z < minZ) {
        position.z = minZ;
        velocity.z *= -1.0f;
    }
    if (position.z > maxZ) {
        position.z = maxZ;
        velocity.z *= -1.0f;
    }
}

void Ball::ResolveBallCollision(Ball &other) {
    glm::vec3 delta = other.position - position;
    float dist = glm::length(delta);
    if (dist < 2 * RADIUS && dist > 0.0f) {
        glm::vec3 normal = delta / dist;
        float overlap = 2 * RADIUS - dist;
        // Separate balls
        position -= normal * (overlap / 2.0f);
        other.position += normal * (overlap / 2.0f);
        // Elastic collision (swap velocity components along normal)
        float v1 = glm::dot(velocity, normal);
        float v2 = glm::dot(other.velocity, normal);
        float m1 = v2;
        float m2 = v1;
        velocity += (m1 - v1) * normal;
        other.velocity += (m2 - v2) * normal;
    }
}
