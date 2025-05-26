#pragma once

namespace Constants {
    static constexpr float CentimeterToMeter = 0.01f;

    static constexpr float OUTER_LENGTH = 315.0f * CentimeterToMeter; // 315 cm
    static constexpr float OUTER_WIDTH = 170.0f * CentimeterToMeter; // 170 cm
    static constexpr float OUTER_HEIGHT = 17.0f * CentimeterToMeter; // 17 cm

    static constexpr float PLAY_LENGTH = 284.0f * CentimeterToMeter; // 284 cm
    static constexpr float PLAY_WIDTH = 142.0f * CentimeterToMeter; // 142 cm

    static constexpr float POCKET_RADIUS = 8.0f * CentimeterToMeter; // 8 cm
    static constexpr float POCKET_DEPTH = 10.0f * CentimeterToMeter; // 10 cm
    static constexpr float POCKET_OPENING = 10.0f * CentimeterToMeter; // 10 cm
    static constexpr float POCKET_OPENING_ANGLE = 30.0f; // 30 degrees

    static constexpr float BALL_SCALE = 0.04f; // Scale factor for ball models
    static constexpr float BALL_RADIUS = 4.0f * CentimeterToMeter; // 2.8575 cm in meters, scaled

    static constexpr float BALL_MASS = 0.17f; // 170 grams in kg
    static constexpr float BALL_FRICTION = 0.02f; // Friction coefficient for balls
    static constexpr float BALL_ROLLING_FRICTION = 0.02f; // Rolling friction coefficient for balls
}

