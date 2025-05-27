/**
 * @file Camera.h
 * @brief Header file for the Camera class.
 * This class manages the camera's position, orientation, and projection in a 3D space.
 * It allows for mouse movement and scroll input to adjust the camera's view.
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
 * @see Camera.cpp for the implementation details.
 */
#ifndef BILLIARDSHOW_CAMERA_H
#define BILLIARDSHOW_CAMERA_H

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Utils/Logger.h"

/** * @class Camera
 * @brief Represents a camera in a 3D space using spherical coordinates.
 * This class allows for orbit-style camera movement around a target point,
 * with methods to process mouse input and retrieve view and projection matrices.
 */
class Camera {

public:
    /** * @fn Camera
     * @brief Constructor for the Camera class.
     * Initializes the camera with default values for yaw, pitch, distance, zoom, aspect ratio, target position, and up vector.
     * Calls updateCameraVectors to calculate the initial camera position.
     * @param aspect The aspect ratio of the viewport (width/height).
     */
    Camera(float aspect);

    /** * @fn ProcessMouseMovement
     * @brief Processes mouse movement input to adjust the camera's yaw and pitch.
     * This method updates the yaw and pitch based on the mouse movement deltas,
     * applying a sensitivity factor to control the speed of rotation.
     * It also clamps the pitch to prevent flipping.
     * @param deltaX The change in mouse X position.
     * @param deltaY The change in mouse Y position.
     */
    void ProcessMouseMovement(float deltaX, float deltaY);

    /** * @fn ProcessMouseScroll
     * @brief Processes mouse scroll input to adjust the camera's distance from the target.
     * This method modifies the distance based on the scroll delta,
     * ensuring it stays within a defined range (1.0 to 10.0).
     * @param deltaY The change in mouse scroll position.
     */
    void ProcessMouseScroll(float deltaY);

    /** * @fn GetViewMatrix
     * @brief Returns the view matrix for the camera.
     * This method calculates the view matrix based on the camera's position, target, and up vector.
     * @return The view matrix as a glm::mat4.
     */
    glm::mat4 GetViewMatrix() const;

    /** * @fn GetProjectionMatrix
     * @brief Returns the projection matrix for the camera.
     * This method calculates the projection matrix based on the camera's aspect ratio and zoom level.
     * @return The projection matrix as a glm::mat4.
     */
    glm::mat4 GetProjectionMatrix() const;

    /** * @fn GetZoom
     * @brief Returns the current zoom level of the camera.
     * @return The zoom level as a float.
     */
    float GetZoom() const { return zoom; }

    /** * @fn GetYaw
     * @brief Returns the current yaw angle of the camera.
     * @return The yaw angle as a float.
     */
    float GetYaw() const { return yaw; }

    /** * @fn GetPitch
     * @brief Returns the current pitch angle of the camera.
     * @return The pitch angle as a float.
     */
    float GetPitch() const { return pitch; }

    /** * @fn GetPosition
     * @brief Returns the current position of the camera.
     * This method provides the camera's position in 3D space as a glm::vec3.
     * @return The position of the camera as a glm::vec3.
     */
    glm::vec3 GetPosition() const { return position; }

private:
    /** * @fn updateCameraVectors
     * @brief Updates the camera position based on spherical coordinates.
     * This method calculates the camera's position in 3D space using spherical coordinates
     * derived from yaw, pitch, and distance. It updates the position vector accordingly.
     */
    void updateCameraVectors();

    /** Camera parameters */
    float yaw, pitch, distance, zoom;
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    float aspectRatio;
};

#endif //BILLIARDSHOW_CAMERA_H
