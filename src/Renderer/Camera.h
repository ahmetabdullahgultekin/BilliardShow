#ifndef BILLIARDSHOW_CAMERA_H
#define BILLIARDSHOW_CAMERA_H

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Utils/Logger.h"

class Camera {
public:
    Camera(float aspect);

    void ProcessMouseMovement(float deltaX, float deltaY);

    void ProcessMouseScroll(float deltaY);

    glm::mat4 GetViewMatrix() const;

    glm::mat4 GetProjectionMatrix() const;

    float GetZoom() const { return zoom; }

    float GetYaw() const { return yaw; }

    float GetPitch() const { return pitch; }

    glm::vec3 GetPosition() const { return position; }

private:
    void updateCameraVectors();

    // Spherical coordinates for orbit camera
    float yaw, pitch, distance, zoom;
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    float aspectRatio;
};

#endif //BILLIARDSHOW_CAMERA_H
