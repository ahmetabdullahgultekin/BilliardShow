#include "Ball.h"
#include "../Renderer/Renderer.h"

Ball::Ball(int number, const glm::vec3 &position)
        : number(number), position(position), model(nullptr) {}

Ball::~Ball() {
    if (model) delete model;
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
        model->Render(position, scale);
    }
}

