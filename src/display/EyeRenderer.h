#pragma once

#include <lvgl.h>
#include "core/CreatureState.h"

namespace PocketSpirit {

class EyeRenderer {
public:
    EyeRenderer();

    void init(lv_obj_t* parent, lv_color_t primaryColor, lv_color_t eyeColor);
    void setExpression(Expression expr);
    void update(float deltaTimeSec);
    void trackPupil(float normX, float normY);
    void setColors(lv_color_t primaryColor, lv_color_t eyeColor);

    Expression getCurrentExpression() const { return _currentExpr; }

    // Layout constants for 240x240 round display
    static constexpr int LEFT_EYE_X  = 68;
    static constexpr int LEFT_EYE_Y  = 78;
    static constexpr int RIGHT_EYE_X = 128;
    static constexpr int RIGHT_EYE_Y = 78;
    static constexpr int EYE_W       = 44;
    static constexpr int EYE_H       = 44;
    static constexpr int PUPIL_W     = 18;
    static constexpr int PUPIL_H     = 18;
    static constexpr int MAX_PUPIL_OFFSET = 10;

private:
    lv_obj_t* _parent       = nullptr;
    lv_obj_t* _leftEyeBg    = nullptr;
    lv_obj_t* _rightEyeBg   = nullptr;
    lv_obj_t* _leftPupil    = nullptr;
    lv_obj_t* _rightPupil   = nullptr;
    lv_obj_t* _leftEyelid   = nullptr;
    lv_obj_t* _rightEyelid  = nullptr;
    lv_obj_t* _mouth        = nullptr;
    lv_obj_t* _body         = nullptr;

    lv_color_t _primaryColor;
    lv_color_t _eyeColor;

    Expression _currentExpr = Expression::IDLE;

    float _blinkTimer     = 0.0f;
    float _nextBlinkTime  = 3.0f;
    bool  _isBlinking     = false;
    float _blinkProgress  = 0.0f;

    float _pupilNormX = 0.0f;
    float _pupilNormY = 0.0f;

    void createEyeObjects();
    void updateBlink(float dt);
    void updatePupilPositions();
    void applyExpression();
    void drawMouthForExpression();

    void drawIdleMouth();
    void drawHappyMouth();
    void drawSadMouth();
    void drawSurprisedMouth();
    void drawSleepyMouth();
    void drawAngryMouth();
    void drawLoveMouth();
    void drawEatingMouth();
    void drawSleepingMouth();

    static float randomBlinkInterval();
};

} // namespace PocketSpirit
