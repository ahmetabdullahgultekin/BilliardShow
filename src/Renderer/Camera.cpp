/**
 * @file Camera.cpp
 * @brief Implementation of Camera class for managing camera position and orientation in OpenGL.
 * This class handles mouse movement and scroll input to adjust the camera's yaw, pitch, distance, and zoom level.
 * @author Ahmet Abdullah Gultekin
 * @date 2025-05-27
 * @version 1.0
 * @details
 * The Camera class uses spherical coordinates to calculate the camera's position based on yaw, pitch, and distance.
 * It provides methods to process mouse movement and scroll input, updating the camera's orientation and position accordingly.
 * It also provides methods to get the view and projection matrices for rendering.
 * @note
 * This implementation uses the GLM library for vector and matrix operations.
 * It assumes that the OpenGL context is already created and initialized before using this class.
 * @see Camera.h for the class definition and member variables.
 */
#include "Camera.h"

/** * @fn Camera
 * @brief Constructor for the Camera class.
 * Initializes the camera with default values for yaw, pitch, distance, zoom, aspect ratio, target position, and up vector.
 * Calls updateCameraVectors to calculate the initial camera position.
 * @param aspect The aspect ratio of the viewport (width/height).
 */
Camera::Camera(float aspect)
        : yaw(-90.0f), pitch(20.0f), distance(3.0f), zoom(45.0f), aspectRatio(aspect), target(0.0f, 0.0f, 0.0f),
          up(0.0f, 1.0f, 0.0f) {
    updateCameraVectors();
}

/** * @fn ProcessMouseMovement
 * @brief Processes mouse movement input to adjust the camera's yaw and pitch.
 * This method updates the yaw and pitch based on the mouse movement deltas,
 * applying a sensitivity factor to control the speed of rotation.
 * It also clamps the pitch to prevent flipping.
 * @param deltaX The change in mouse X position.
 * @param deltaY The change in mouse Y position.
 */
void Camera::ProcessMouseMovement(float deltaX, float deltaY) {
    const float sensitivity = 0.3f;
    yaw += deltaX * sensitivity;
    pitch -= deltaY * sensitivity;
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    updateCameraVectors();
}

/** * @fn ProcessMouseScroll
 * @brief Processes mouse scroll input to adjust the camera's distance from the target.
 * This method modifies the distance based on the scroll delta,
 * ensuring it stays within a defined range (1.0 to 10.0).
 * @param deltaY The change in mouse scroll position.
 */
void Camera::ProcessMouseScroll(float deltaY) {
    distance -= deltaY * 0.2f;
    if (distance < 1.0f) distance = 1.0f;
    if (distance > 10.0f) distance = 10.0f;
    updateCameraVectors();
}

/**
 * @fn updateCameraVectors
 * @brief Updates the camera position based on spherical coordinates.
 * This method calculates the camera's position in 3D space using spherical coordinates
 * derived from yaw, pitch, and distance. It updates the position vector accordingly.
 */
void Camera::updateCameraVectors() {
    // Spherical coordinates to cartesian conversion
    float radYaw = glm::radians(yaw);
    float radPitch = glm::radians(pitch);
    position.x = target.x + distance * cos(radPitch) * cos(radYaw);
    position.y = target.y + distance * sin(radPitch);
    position.z = target.z + distance * cos(radPitch) * sin(radYaw);
}

/**
 * @fn GetViewMatrix
 * @brief Returns the view matrix for the camera.
 * This method uses glm::lookAt to create a view matrix based on the camera's position, target, and up vector.
 * @return The view matrix as a glm::mat4.
 */
glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(position, target, up);
}

/**
 * @fn GetProjectionMatrix
 * @brief Returns the projection matrix for the camera.
 * This method creates a perspective projection matrix using glm::perspective,
 * based on the camera's zoom level and aspect ratio.
 * @return The projection matrix as a glm::mat4.
 */
glm::mat4 Camera::GetProjectionMatrix() const {
    return glm::perspective(glm::radians(zoom), aspectRatio, 0.1f, 100.0f);
}

