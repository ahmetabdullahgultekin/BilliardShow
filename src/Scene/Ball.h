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
    static constexpr float RADIUS = 2.8575f * Table::TABLE_SCALE; // 2.8575 cm in meters

    Ball(int number, const glm::vec3 &position);

    ~Ball();

    void SetPosition(const glm::vec3 &pos);

    glm::vec3 GetPosition() const;

    int GetNumber() const;

    void SetModel(Model3D *model);

    void Render(Renderer *renderer, float scale = 0.06f);

    void SetVelocity(const glm::vec3 &vel);

    glm::vec3 GetVelocity() const;

    void Update(float deltaTime);

    void ApplyFriction(float deltaTime, float friction = 0.2f);

    void ResolveTableCollision(const Table &table);

    void ResolveBallCollision(Ball &other);

private:
    int number;
    glm::vec3 position;
    glm::vec3 velocity = glm::vec3(0.0f);
    Model3D *model;
};

#endif //BILLIARDSHOW_BALL_H

