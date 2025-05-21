#include "Camera.h"

Camera::Camera(float aspect)
        : yaw(-90.0f), pitch(20.0f), distance(3.0f), zoom(45.0f), aspectRatio(aspect), target(0.0f, 0.0f, 0.0f),
          up(0.0f, 1.0f, 0.0f) {
    updateCameraVectors();
}

void Camera::ProcessMouseMovement(float deltaX, float deltaY) {
    const float sensitivity = 0.3f;
    yaw += deltaX * sensitivity;
    pitch -= deltaY * sensitivity;
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float deltaY) {
    distance -= deltaY * 0.2f;
    if (distance < 1.0f) distance = 1.0f;
    if (distance > 10.0f) distance = 10.0f;
    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    // Spherical coordinates to cartesian conversion
    float radYaw = glm::radians(yaw);
    float radPitch = glm::radians(pitch);
    position.x = target.x + distance * cos(radPitch) * cos(radYaw);
    position.y = target.y + distance * sin(radPitch);
    position.z = target.z + distance * cos(radPitch) * sin(radYaw);
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(position, target, up);
}

glm::mat4 Camera::GetProjectionMatrix() const {
    return glm::perspective(glm::radians(zoom), aspectRatio, 0.1f, 100.0f);
}