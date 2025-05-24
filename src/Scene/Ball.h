#ifndef BILLIARDSHOW_BALL_H
#define BILLIARDSHOW_BALL_H

#include <glm/glm.hpp>
#include "../Loader/Model3D.h"

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

private:
    int number;
    glm::vec3 position;
    Model3D *model;
};

#endif //BILLIARDSHOW_BALL_H

