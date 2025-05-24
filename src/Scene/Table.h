#ifndef BILLIARDSHOW_TABLE_H
#define BILLIARDSHOW_TABLE_H

class Table {
public:
    // Dimensions in centimeters
    static constexpr float TABLE_SCALE = 0.01f; // Convert cm to meters

    static constexpr float OUTER_LENGTH = 315.0f * TABLE_SCALE; // 315 cm
    static constexpr float OUTER_WIDTH = 170.0f * TABLE_SCALE; // 170 cm
    static constexpr float OUTER_HEIGHT = 17.0f * TABLE_SCALE; // 17 cm

    static constexpr float PLAY_LENGTH = 284.0f * TABLE_SCALE; // 284 cm
    static constexpr float PLAY_WIDTH = 142.0f * TABLE_SCALE; // 142 cm

    static constexpr float POCKET_RADIUS = 8.0f * TABLE_SCALE; // 8 cm
    static constexpr float POCKET_DEPTH = 10.0f * TABLE_SCALE; // 10 cm
    static constexpr float POCKET_OPENING = 10.0f * TABLE_SCALE; // 10 cm
    static constexpr float POCKET_OPENING_ANGLE = 30.0f; // 30 degrees

    Table();
};

#endif //BILLIARDSHOW_TABLE_H
