#ifndef BILLIARDSHOW_TABLE_H
#define BILLIARDSHOW_TABLE_H

#include "../Renderer/Renderer.h"
#include "Constants.h"

class Table {
public:
    static constexpr float OUTER_LENGTH = Constants::OUTER_LENGTH;
    static constexpr float OUTER_WIDTH = Constants::OUTER_WIDTH;
    static constexpr float OUTER_HEIGHT = Constants::OUTER_HEIGHT;
    static constexpr float PLAY_LENGTH = Constants::PLAY_LENGTH;
    static constexpr float PLAY_WIDTH = Constants::PLAY_WIDTH;
    static constexpr float POCKET_RADIUS = Constants::POCKET_RADIUS;
    static constexpr float POCKET_DEPTH = Constants::POCKET_DEPTH;
    static constexpr float POCKET_OPENING = Constants::POCKET_OPENING;
    static constexpr float POCKET_OPENING_ANGLE = Constants::POCKET_OPENING_ANGLE;

    Table();
};

#endif //BILLIARDSHOW_TABLE_H
