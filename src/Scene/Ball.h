#ifndef BILLIARDSHOW_BALL_H
#define BILLIARDSHOW_BALL_H

#include <glm/glm.hpp>
#include "../Loader/Model3D.h"
#include "../Renderer/Renderer.h"
#include "../Utils/Logger.h"
#include "../App.h"
#include "../Scene/Table.h"

class Renderer;

class Table;

class Ball {
public:
    static constexpr float RADIUS = Constants::BALL_RADIUS; // 2.8575 cm in meters

    Ball(int number, const glm::vec3 &position);

    ~Ball();

    void SetPosition(const glm::vec3 &pos);

    glm::vec3 GetPosition() const;

    int GetNumber() const;

    void SetModel(Model3D *model);

    void Render(Renderer *renderer, float scale);

    void SetVelocity(const glm::vec3 &vel);

    glm::vec3 GetVelocity() const;

    void SetAngularVelocity(const glm::vec3 &avel) { angularVelocity = avel; }

    glm::vec3 GetAngularVelocity() const { return angularVelocity; }

    glm::mat4 GetRotation() const { return rotation; }

    void Update(float deltaTime);

    void ApplyFriction(float deltaTime, float friction);

    void ApplyRollingFriction(float deltaTime, float rollingFriction);

    void ResolveTableCollision(const Table &table);

    void ResolveBallCollision(Ball &other);

    void Install();

private:
    int number;
    glm::vec3 position;
    glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f); // Initial velocity
    glm::vec3 angularVelocity = glm::vec3(0.0f); // radians/sec
    glm::mat4 rotation = glm::mat4(0.0f); // orientation
    Model3D *model;
};

#endif //BILLIARDSHOW_BALL_H