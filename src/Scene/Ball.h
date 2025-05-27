/**
 * @file Ball.h
 * @brief Header file for the Ball class in the BilliardShow game.
 * This class represents a billiard ball with properties like position, velocity, and methods for rendering and physics interactions.
 * @author Ahmet Abdullah Gultekin
 * @date 2025-05-27
 * @version 1.0
 */
#ifndef BILLIARDSHOW_BALL_H
#define BILLIARDSHOW_BALL_H

#include <glm/glm.hpp>
#include "../Loader/ObjectLoader.h"
#include "../Renderer/Renderer.h"
#include "../Utils/Logger.h"
#include "../App.h"
#include "../Scene/Table.h"

// Forward declarations
class Renderer;

class Table;

/**
 * @class Ball
 * @brief Represents a billiard ball with properties and methods for rendering and physics interactions.
 */
class Ball {
public:
    // Constants
    static constexpr float RADIUS = Constants::BALL_RADIUS; // 2.8575 cm in meters

    Ball(int number, const glm::vec3 &position);

    ~Ball();

    /**
     * @brief Sets the position of the ball.
     * @param pos The new position of the ball.
     */
    void SetPosition(const glm::vec3 &pos);

    /**
     * @brief Gets the current position of the ball.
     * @return The position of the ball as a glm::vec3.
     */
    glm::vec3 GetPosition() const;

    /**
     * @brief Gets the number of the ball.
     * @return The number of the ball.
     */
    int GetNumber() const;

    /**
     * @brief Sets the model for the ball.
     * @param model Pointer to the ObjectLoader instance containing the ball model.
     */
    void SetModel(ObjectLoader *model);

    /**
     * @brief Renders the ball using the provided renderer.
     * @param renderer Pointer to the Renderer instance used for rendering.
     * @param scale Scale factor for rendering the ball.
     */
    void Render(Renderer *renderer, float scale);

    /**
     * @brief Sets the velocity of the ball.
     * @param vel The new velocity of the ball as a glm::vec3.
     */
    void SetVelocity(const glm::vec3 &vel);

    /**
     * @brief Gets the current velocity of the ball.
     * @return The velocity of the ball as a glm::vec3.
     */
    glm::vec3 GetVelocity() const;

    /**
     * @brief Sets the angular velocity of the ball.
     * @param avel The new angular velocity of the ball as a glm::vec3.
     */
    void SetAngularVelocity(const glm::vec3 &avel) { angularVelocity = avel; }

    /**
     * @brief Sets the rotation of the ball.
     * @param rot The new rotation matrix of the ball as a glm::mat4.
     */
    glm::vec3 GetAngularVelocity() const { return angularVelocity; }

    /**
     * @brief Gets the rotation matrix of the ball.
     * @return The rotation matrix of the ball as a glm::mat4.
     */
    glm::mat4 GetRotation() const { return rotation; }

    /**
     * @brief Updates the ball's position and rotation based on its velocity and angular velocity.
     * @param deltaTime The time elapsed since the last update in seconds.
     */
    void Update(float deltaTime);

    /**
     * @brief Applies friction to the ball's velocity.
     * @param deltaTime The time elapsed since the last update in seconds.
     * @param friction The friction coefficient to apply.
     */
    void ApplyFriction(float deltaTime, float friction);

    /**
     * @brief Applies rolling friction to the ball's velocity.
     * @param deltaTime The time elapsed since the last update in seconds.
     * @param rollingFriction The rolling friction coefficient to apply.
     */
    void ApplyRollingFriction(float deltaTime, float rollingFriction);

    /**
     * @brief Resolves collision with the table.
     * @param table The Table instance representing the billiard table.
     */
    void ResolveTableCollision(const Table &table);

    /**
     * @brief Resolves collision with another ball.
     * @param other The other Ball instance to resolve collision with.
     */
    void ResolveBallCollision(Ball &other);

    /**
     * @brief Installs the ball model for rendering.
     * This method prepares the ball model for rendering by setting up the necessary OpenGL buffers and attributes.
     */
    void Install();

private:
    /**
     * @brief Updates the rotation matrix based on the angular velocity.
     * @param deltaTime The time elapsed since the last update in seconds.
     */
    int number;
    glm::vec3 position;
    glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f); // Initial velocity
    glm::vec3 angularVelocity = glm::vec3(0.0f); // radians/sec
    glm::mat4 rotation = glm::mat4(0.0f); // orientation
    ObjectLoader *model;
};

#endif //BILLIARDSHOW_BALL_H
