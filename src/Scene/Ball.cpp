#include "Ball.h"
#include "../Renderer/Renderer.h"

Ball::Ball(int number, const glm::vec3 &position)
        : number(number), position(position), model(nullptr), rotation(glm::mat4(1.0f)), angularVelocity(0.0f) {}

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
        // Use the rotation matrix for spinning
        model->Render(position, scale, rotation);
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
    // Gravity constant (meters per second squared)
    constexpr glm::vec3 GRAVITY(0.0f, -9.81f, 0.0f);
    // Apply gravity to velocity
    velocity += GRAVITY * deltaTime;
    // Update position
    position += velocity * deltaTime;
    // --- Realistic rolling/sliding transition ---
    glm::vec3 v_flat = glm::vec3(velocity.x, 0.0f, velocity.z); // ignore y for rolling
    float speed = glm::length(v_flat);
    float spinThreshold = 0.2f; // m/s, tweak as needed
    if (speed > 0.0001f) {
        glm::vec3 axis = glm::normalize(glm::cross(v_flat, glm::vec3(0, 1, 0)));
        float omega = speed / RADIUS; // ideal rolling
        // If speed is above threshold, blend current spin toward rolling spin
        if (speed > spinThreshold) {
            glm::vec3 targetAngular = axis * omega;
            float blend = glm::clamp((speed - spinThreshold) / spinThreshold, 0.0f, 1.0f);
            angularVelocity = glm::mix(angularVelocity, targetAngular, blend * deltaTime * 5.0f); // smooth transition
        } else {
            // At low speed, lock to rolling
            angularVelocity = axis * omega;
        }
        float angle = glm::length(angularVelocity) * deltaTime;
        if (glm::length(angularVelocity) > 0.0001f) {
            glm::vec3 spinAxis = glm::normalize(angularVelocity);
            rotation = glm::rotate(rotation, angle, spinAxis);
        }
    } else {
        angularVelocity = glm::vec3(0.0f);
    }
}

void Ball::ApplyFriction(float deltaTime, float friction) {
    float speed = glm::length(velocity);
    if (speed > 0.0f) {
        float drop = friction * deltaTime;
        float newSpeed = glm::max(0.0f, speed - drop);
        // Reduce velocity
        velocity = (speed > 0.0f) ? velocity * (newSpeed / speed) : glm::vec3(0.0f);
        // --- Friction also causes spin-up (rolling/sliding transition) ---
        // If the ball is sliding (not pure rolling), friction will try to match spin to rolling
        glm::vec3 v_flat = glm::vec3(velocity.x, 0.0f, velocity.z);
        float speedFlat = glm::length(v_flat);
        if (speedFlat > 0.0001f) {
            glm::vec3 axis = glm::normalize(glm::cross(v_flat, glm::vec3(0, 1, 0)));
            float omega = speedFlat / RADIUS;
            glm::vec3 targetAngular = axis * omega;
            // Blend current angular velocity toward rolling value
            float blend = glm::clamp((speedFlat) / 0.2f, 0.0f, 1.0f); // 0.2f is threshold, tweak as needed
            angularVelocity = glm::mix(angularVelocity, targetAngular, blend * deltaTime * 2.0f);
        }
    }
    // Angular friction (spin slows down)
    float angSpeed = glm::length(angularVelocity);
    if (angSpeed > 0.0f) {
        float drop = friction * deltaTime;
        float newAngSpeed = glm::max(0.0f, angSpeed - drop);
        angularVelocity = (angSpeed > 0.0f) ? angularVelocity * (newAngSpeed / angSpeed) : glm::vec3(0.0f);
    }
}

void Ball::ApplyRollingFriction(float deltaTime, float rollingFriction) {
    float speed = glm::length(velocity);
    if (speed > 0.0f) {
        float drop = rollingFriction * deltaTime;
        float newSpeed = glm::max(0.0f, speed - drop);
        velocity = (speed > 0.0f) ? velocity * (newSpeed / speed) : glm::vec3(0.0f);
    }
}

void Ball::ResolveTableCollision(const Table &table) {
    // Use play area dimensions from Table static constants
    float minX = -Table::PLAY_LENGTH / 2.0f + RADIUS;
    float maxX = Table::PLAY_LENGTH / 2.0f - RADIUS;
    float minZ = -Table::PLAY_WIDTH / 2.0f + RADIUS;
    float maxZ = Table::PLAY_WIDTH / 2.0f - RADIUS;
    float tableSurfaceY = Table::OUTER_HEIGHT / 2.0f + RADIUS; // Centered above the table surface

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
    // Clamp y to table surface and stop downward velocity
    if (position.y < tableSurfaceY) {
        position.y = tableSurfaceY;
        if (velocity.y < 0.0f) velocity.y = 0.0f;
    }
    // --- Ensure rolling spin matches velocity after collision ---
    glm::vec3 v_flat = glm::vec3(velocity.x, 0.0f, velocity.z); // ignore y for rolling
    float speed = glm::length(v_flat);
    if (speed > 0.0001f) {
        glm::vec3 axis = glm::normalize(glm::cross(v_flat, glm::vec3(0, 1, 0)));
        float omega = speed / RADIUS; // radians/sec
        angularVelocity = axis * omega;
    } else {
        angularVelocity = glm::vec3(0.0f);
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
        // --- Ensure rolling spin matches velocity after collision ---
        // Use the new velocity direction for spin, not the collision normal
        for (Ball *b: {this, &other}) {
            glm::vec3 v_flat = glm::vec3(b->velocity.x, 0.0f, b->velocity.z);
            float speed = glm::length(v_flat);
            if (speed > 0.0001f) {
                // Rolling axis is opposite to velocity direction (cross(up, -velocity))
                glm::vec3 axis = glm::normalize(glm::cross(glm::vec3(0, 1, 0), -v_flat));
                float omega = speed / RADIUS;
                b->angularVelocity = axis * omega;
            } else {
                b->angularVelocity = glm::vec3(0.0f);
            }
        }
    }
}

void Ball::Install() {
    if (model) {
        if (!model->Install()) {
            Logger::Error("Failed to install model for ball " + std::to_string(number));
        } else {
            Logger::Info("Ball " + std::to_string(number) + " model installed successfully.");
        }
    } else {
        Logger::Error("No model set for ball " + std::to_string(number));
    }
}
