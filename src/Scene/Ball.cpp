/**
 * @file Ball.cpp
 * @brief Implementation of the Ball class for billiards simulation.
 * This class handles ball properties, rendering, physics updates, and collisions.
 * @author Ahmet Abdullah Gultekin
 * @date 2025-05-27
 */
#include "Ball.h"
#include "../Renderer/Renderer.h"

/** * @brief Constructor for Ball class.
 * @param number The ball number (1-16).
 * @param position Initial position of the ball in 3D space.
 */
Ball::Ball(int number, const glm::vec3 &position)
        : number(number), position(position), model(nullptr), rotation(glm::mat4(1.0f)), angularVelocity(0.0f) {}

/**
 * @brief Destructor for Ball class.
 * Frees the model if it was allocated.
 * This is important to prevent memory leaks.
 */
Ball::~Ball() {
    delete model;
}

/**
 * @brief Sets the position of the ball.
 * @param pos New position for the ball.
 */
void Ball::SetPosition(const glm::vec3 &pos) {
    position = pos;
}

/**
 * @brief Gets the current position of the ball.
 * @return The position of the ball in 3D space.
 */
glm::vec3 Ball::GetPosition() const {
    return position;
}

/**
 * @brief Gets the ball number.
 * @return The number of the ball (1-16).
 */
int Ball::GetNumber() const {
    return number;
}

/**
 * @brief Sets the model for the ball.
 * @param m Pointer to the ObjectLoader containing the ball model.
 * This will replace any existing model.
 */
void Ball::SetModel(ObjectLoader *m) {
    if (model) delete model;
    model = m;
}

/**
 * @brief Renders the ball using the provided renderer.
 * @param renderer Pointer to the Renderer instance.
 * @param scale Scale factor for rendering the ball.
 * This will apply the rotation matrix for spinning effect.
 */
void Ball::Render(Renderer *renderer, float scale) {
    if (model && renderer) {
        // Use the rotation matrix for spinning
        model->Render(position, scale, rotation);
    } else {
        Logger::Error("Ball::Render called with null model or renderer");
    }
}

/**
 * @brief Sets the velocity of the ball.
 * @param vel New velocity vector for the ball.
 */
void Ball::SetVelocity(const glm::vec3 &vel) {
    velocity = vel;
}

/**
 * @brief Gets the current velocity of the ball.
 * @return The velocity vector of the ball.
 */
glm::vec3 Ball::GetVelocity() const {
    return velocity;
}

/**
 * @brief Updates the ball's position and rotation based on physics.
 * This applies gravity, updates position, and handles rolling/sliding transitions.
 * @param deltaTime Time step for the update (in seconds).
 */
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
        // If speed is above a threshold,
        // blend current spin toward rolling spin
        if (speed < spinThreshold) {
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

/**
 * @brief Applies friction to the ball's velocity and angular velocity.
 * This simulates rolling/sliding transitions and slows down the ball.
 * @param deltaTime Time step for the update (in seconds).
 * @param friction Friction coefficient to apply.
 */
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
            // Blend current angular velocity toward a rolling value
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

/**
 * @brief Applies rolling friction to the ball's velocity.
 * This simulates the effect of rolling friction on the ball.
 * @param deltaTime Time step for the update (in seconds).
 * @param rollingFriction Rolling friction coefficient to apply.
 */
void Ball::ApplyRollingFriction(float deltaTime, float rollingFriction) {
    float speed = glm::length(velocity);
    if (speed > 0.0f) {
        float drop = rollingFriction * deltaTime;
        float newSpeed = glm::max(0.0f, speed - drop);
        velocity = (speed > 0.0f) ? velocity * (newSpeed / speed) : glm::vec3(0.0f);
    }
}

/**
 * @brief Resolves collisions with the table boundaries.
 * This checks if the ball is outside the play area and adjusts its position and velocity accordingly.
 * @param table Reference to the Table object containing play area dimensions.
 */
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
        glm::vec3 axis = glm::normalize(glm::cross(-v_flat, glm::vec3(0, 1, 0)));
        float omega = speed / RADIUS; // radians/sec
        angularVelocity = axis * omega;
    } else {
        angularVelocity = glm::vec3(0.0f);
    }
}

/**
 * @brief Resolves collisions with another ball.
 * This checks if the balls are overlapping and adjusts their positions and velocities accordingly.
 * @param other Reference to the other Ball object to check for collision.
 */
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
                // Rolling axis is opposite to the velocity direction (cross(up, -velocity))
                glm::vec3 axis = glm::normalize(glm::cross(glm::vec3(0, 1, 0), -v_flat));
                float omega = speed / RADIUS;
                b->angularVelocity = axis * omega;
            } else {
                b->angularVelocity = glm::vec3(0.0f);
            }
        }
    }
}

/**
 * @brief Installs the ball model.
 * This loads the model data into GPU memory and prepares it for rendering.
 * If the model is already installed, it will not reinstall.
 */
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
