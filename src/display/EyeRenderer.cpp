#include "EyeRenderer.h"
#include <esp_random.h>

namespace PocketSpirit {

EyeRenderer::EyeRenderer() {}

void EyeRenderer::init(lv_obj_t* parent, lv_color_t primaryColor, lv_color_t eyeColor) {
    _parent       = parent;
    _primaryColor = primaryColor;
    _eyeColor     = eyeColor;

    createEyeObjects();
    applyExpression();
}

void EyeRenderer::createEyeObjects() {
    // Body background
    _body = lv_obj_create(_parent);
    lv_obj_set_size(_body, 160, 120);
    lv_obj_align(_body, LV_ALIGN_CENTER, 0, 15);
    lv_obj_set_style_bg_color(_body, _primaryColor, 0);
    lv_obj_set_style_bg_opa(_body, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(_body, 0, 0);
    lv_obj_set_style_radius(_body, LV_RADIUS_CIRCLE, 0);
    lv_obj_clear_flag(_body, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);

    // Left eye background (white)
    _leftEyeBg = lv_obj_create(_parent);
    lv_obj_set_size(_leftEyeBg, EYE_W, EYE_H);
    lv_obj_set_pos(_leftEyeBg, LEFT_EYE_X, LEFT_EYE_Y);
    lv_obj_set_style_bg_color(_leftEyeBg, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(_leftEyeBg, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(_leftEyeBg, 0, 0);
    lv_obj_set_style_radius(_leftEyeBg, LV_RADIUS_CIRCLE, 0);
    lv_obj_clear_flag(_leftEyeBg, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);

    // Right eye background
    _rightEyeBg = lv_obj_create(_parent);
    lv_obj_set_size(_rightEyeBg, EYE_W, EYE_H);
    lv_obj_set_pos(_rightEyeBg, RIGHT_EYE_X, RIGHT_EYE_Y);
    lv_obj_set_style_bg_color(_rightEyeBg, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(_rightEyeBg, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(_rightEyeBg, 0, 0);
    lv_obj_set_style_radius(_rightEyeBg, LV_RADIUS_CIRCLE, 0);
    lv_obj_clear_flag(_rightEyeBg, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);

    // Left pupil
    _leftPupil = lv_obj_create(_leftEyeBg);
    lv_obj_set_size(_leftPupil, PUPIL_W, PUPIL_H);
    lv_obj_align(_leftPupil, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(_leftPupil, _eyeColor, 0);
    lv_obj_set_style_bg_opa(_leftPupil, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(_leftPupil, 0, 0);
    lv_obj_set_style_radius(_leftPupil, LV_RADIUS_CIRCLE, 0);
    lv_obj_clear_flag(_leftPupil, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);

    // Right pupil
    _rightPupil = lv_obj_create(_rightEyeBg);
    lv_obj_set_size(_rightPupil, PUPIL_W, PUPIL_H);
    lv_obj_align(_rightPupil, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(_rightPupil, _eyeColor, 0);
    lv_obj_set_style_bg_opa(_rightPupil, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(_rightPupil, 0, 0);
    lv_obj_set_style_radius(_rightPupil, LV_RADIUS_CIRCLE, 0);
    lv_obj_clear_flag(_rightPupil, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);

    // Left eyelid (for blink/close)
    _leftEyelid = lv_obj_create(_leftEyeBg);
    lv_obj_set_size(_leftEyelid, EYE_W + 4, EYE_H + 4);
    lv_obj_align(_leftEyelid, LV_ALIGN_TOP_MID, 0, -(EYE_H + 4));
    lv_obj_set_style_bg_color(_leftEyelid, _primaryColor, 0);
    lv_obj_set_style_bg_opa(_leftEyelid, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(_leftEyelid, 0, 0);
    lv_obj_set_style_radius(_leftEyelid, 0, 0);
    lv_obj_clear_flag(_leftEyelid, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);

    // Right eyelid
    _rightEyelid = lv_obj_create(_rightEyeBg);
    lv_obj_set_size(_rightEyelid, EYE_W + 4, EYE_H + 4);
    lv_obj_align(_rightEyelid, LV_ALIGN_TOP_MID, 0, -(EYE_H + 4));
    lv_obj_set_style_bg_color(_rightEyelid, _primaryColor, 0);
    lv_obj_set_style_bg_opa(_rightEyelid, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(_rightEyelid, 0, 0);
    lv_obj_set_style_radius(_rightEyelid, 0, 0);
    lv_obj_clear_flag(_rightEyelid, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);

    // Mouth
    _mouth = lv_obj_create(_parent);
    lv_obj_set_size(_mouth, 30, 15);
    lv_obj_set_pos(_mouth, 105, 125);
    lv_obj_set_style_bg_color(_mouth, _eyeColor, 0);
    lv_obj_set_style_bg_opa(_mouth, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(_mouth, 0, 0);
    lv_obj_set_style_radius(_mouth, LV_RADIUS_CIRCLE, 0);
    lv_obj_clear_flag(_mouth, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
}

void EyeRenderer::setExpression(Expression expr) {
    if (_currentExpr == expr) return;
    _currentExpr = expr;
    applyExpression();
}

void EyeRenderer::applyExpression() {
    if (!_leftEyeBg) return;

    // Reset eyes to default round shape
    lv_obj_set_size(_leftEyeBg, EYE_W, EYE_H);
    lv_obj_set_size(_rightEyeBg, EYE_W, EYE_H);
    lv_obj_set_style_radius(_leftEyeBg, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_radius(_rightEyeBg, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_size(_leftPupil, PUPIL_W, PUPIL_H);
    lv_obj_set_size(_rightPupil, PUPIL_W, PUPIL_H);
    lv_obj_set_style_radius(_leftPupil, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_radius(_rightPupil, LV_RADIUS_CIRCLE, 0);

    switch (_currentExpr) {
        case Expression::HAPPY:
            // Squinted happy eyes (half-height)
            lv_obj_set_size(_leftEyeBg, EYE_W, EYE_H / 2);
            lv_obj_set_size(_rightEyeBg, EYE_W, EYE_H / 2);
            lv_obj_set_style_radius(_leftEyeBg, LV_RADIUS_CIRCLE, 0);
            lv_obj_set_style_radius(_rightEyeBg, LV_RADIUS_CIRCLE, 0);
            lv_obj_add_flag(_leftPupil, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(_rightPupil, LV_OBJ_FLAG_HIDDEN);
            break;

        case Expression::SAD:
            // Droopy eyes - slightly taller on bottom
            lv_obj_set_style_radius(_leftEyeBg, 10, 0);
            lv_obj_set_style_radius(_rightEyeBg, 10, 0);
            lv_obj_align(_leftPupil, LV_ALIGN_CENTER, 0, 3);
            lv_obj_align(_rightPupil, LV_ALIGN_CENTER, 0, 3);
            break;

        case Expression::SLEEPY:
            // Half-closed eyes
            lv_obj_set_size(_leftEyelid, EYE_W + 4, (EYE_H + 4) / 2);
            lv_obj_set_size(_rightEyelid, EYE_W + 4, (EYE_H + 4) / 2);
            lv_obj_align(_leftEyelid, LV_ALIGN_TOP_MID, 0, -2);
            lv_obj_align(_rightEyelid, LV_ALIGN_TOP_MID, 0, -2);
            break;

        case Expression::SURPRISED:
            // Wide eyes, small pupils
            lv_obj_set_size(_leftEyeBg, EYE_W + 8, EYE_H + 8);
            lv_obj_set_size(_rightEyeBg, EYE_W + 8, EYE_H + 8);
            lv_obj_set_size(_leftPupil, PUPIL_W - 6, PUPIL_H - 6);
            lv_obj_set_size(_rightPupil, PUPIL_W - 6, PUPIL_H - 6);
            break;

        case Expression::ANGRY:
            // Angled eyelids (top half covered)
            lv_obj_set_size(_leftEyelid, EYE_W + 4, (EYE_H + 4) / 2 + 4);
            lv_obj_set_size(_rightEyelid, EYE_W + 4, (EYE_H + 4) / 2 + 4);
            lv_obj_align(_leftEyelid, LV_ALIGN_TOP_MID, -4, -2);
            lv_obj_align(_rightEyelid, LV_ALIGN_TOP_MID, 4, -2);
            break;

        case Expression::LOVE:
            // Larger pupils (dilated)
            lv_obj_set_size(_leftPupil, PUPIL_W + 6, PUPIL_H + 6);
            lv_obj_set_size(_rightPupil, PUPIL_W + 6, PUPIL_H + 6);
            break;

        case Expression::SLEEPING:
            // Fully closed eyes
            lv_obj_set_size(_leftEyelid, EYE_W + 4, EYE_H + 4);
            lv_obj_set_size(_rightEyelid, EYE_W + 4, EYE_H + 4);
            lv_obj_align(_leftEyelid, LV_ALIGN_CENTER, 0, 0);
            lv_obj_align(_rightEyelid, LV_ALIGN_CENTER, 0, 0);
            lv_obj_add_flag(_leftPupil, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(_rightPupil, LV_OBJ_FLAG_HIDDEN);
            break;

        case Expression::EATING:
            // Small happy eyes
            lv_obj_set_size(_leftEyeBg, EYE_W - 4, EYE_H / 2);
            lv_obj_set_size(_rightEyeBg, EYE_W - 4, EYE_H / 2);
            lv_obj_add_flag(_leftPupil, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(_rightPupil, LV_OBJ_FLAG_HIDDEN);
            break;

        case Expression::EVOLVING:
            // Glowing large eyes
            lv_obj_set_size(_leftPupil, PUPIL_W + 10, PUPIL_H + 10);
            lv_obj_set_size(_rightPupil, PUPIL_W + 10, PUPIL_H + 10);
            lv_obj_set_style_bg_color(_leftPupil, lv_color_make(0xFF, 0xFF, 0x00), 0);
            lv_obj_set_style_bg_color(_rightPupil, lv_color_make(0xFF, 0xFF, 0x00), 0);
            break;

        case Expression::IDLE:
        default:
            lv_obj_clear_flag(_leftPupil, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(_rightPupil, LV_OBJ_FLAG_HIDDEN);
            // Reset eyelids
            lv_obj_set_size(_leftEyelid, EYE_W + 4, EYE_H + 4);
            lv_obj_set_size(_rightEyelid, EYE_W + 4, EYE_H + 4);
            lv_obj_align(_leftEyelid, LV_ALIGN_TOP_MID, 0, -(EYE_H + 4));
            lv_obj_align(_rightEyelid, LV_ALIGN_TOP_MID, 0, -(EYE_H + 4));
            // Reset pupil color
            lv_obj_set_style_bg_color(_leftPupil, _eyeColor, 0);
            lv_obj_set_style_bg_color(_rightPupil, _eyeColor, 0);
            break;
    }

    drawMouthForExpression();
    updatePupilPositions();
}

void EyeRenderer::update(float deltaTimeSec) {
    if (_currentExpr != Expression::SLEEPING) {
        updateBlink(deltaTimeSec);
    }
}

void EyeRenderer::updateBlink(float dt) {
    _blinkTimer += dt;

    if (_isBlinking) {
        _blinkProgress += dt * 10.0f; // blink speed

        if (_blinkProgress >= 1.0f) {
            _isBlinking = false;
            _blinkProgress = 0.0f;
            _blinkTimer = 0.0f;
            _nextBlinkTime = randomBlinkInterval();

            // Reset eyelids
            lv_obj_align(_leftEyelid, LV_ALIGN_TOP_MID, 0, -(EYE_H + 4));
            lv_obj_align(_rightEyelid, LV_ALIGN_TOP_MID, 0, -(EYE_H + 4));
        } else {
            // Animate eyelid down then up
            float t = _blinkProgress;
            float lidOffset;
            if (t < 0.5f) {
                lidOffset = -(EYE_H + 4) * (1.0f - t * 2.0f);
            } else {
                lidOffset = -(EYE_H + 4) * ((t - 0.5f) * 2.0f);
            }
            lv_obj_align(_leftEyelid, LV_ALIGN_TOP_MID, 0, (int32_t)lidOffset);
            lv_obj_align(_rightEyelid, LV_ALIGN_TOP_MID, 0, (int32_t)lidOffset);
        }
    } else if (_blinkTimer >= _nextBlinkTime) {
        _isBlinking = true;
        _blinkProgress = 0.0f;
    }
}

void EyeRenderer::trackPupil(float normX, float normY) {
    _pupilNormX = normX > 1.0f ? 1.0f : (normX < -1.0f ? -1.0f : normX);
    _pupilNormY = normY > 1.0f ? 1.0f : (normY < -1.0f ? -1.0f : normY);
    updatePupilPositions();
}

void EyeRenderer::updatePupilPositions() {
    if (!_leftPupil || !_rightPupil) return;

    int32_t offX = (int32_t)(_pupilNormX * MAX_PUPIL_OFFSET);
    int32_t offY = (int32_t)(_pupilNormY * MAX_PUPIL_OFFSET);

    lv_obj_align(_leftPupil, LV_ALIGN_CENTER, offX, offY);
    lv_obj_align(_rightPupil, LV_ALIGN_CENTER, offX, offY);
}

void EyeRenderer::setColors(lv_color_t primaryColor, lv_color_t eyeColor) {
    _primaryColor = primaryColor;
    _eyeColor     = eyeColor;

    if (_body)          lv_obj_set_style_bg_color(_body, _primaryColor, 0);
    if (_leftEyelid)    lv_obj_set_style_bg_color(_leftEyelid, _primaryColor, 0);
    if (_rightEyelid)   lv_obj_set_style_bg_color(_rightEyelid, _primaryColor, 0);
    if (_leftPupil)     lv_obj_set_style_bg_color(_leftPupil, _eyeColor, 0);
    if (_rightPupil)    lv_obj_set_style_bg_color(_rightPupil, _eyeColor, 0);
    if (_mouth)         lv_obj_set_style_bg_color(_mouth, _eyeColor, 0);
}

void EyeRenderer::drawMouthForExpression() {
    if (!_mouth) return;

    lv_obj_clear_flag(_mouth, LV_OBJ_FLAG_HIDDEN);

    switch (_currentExpr) {
        case Expression::HAPPY:
        case Expression::LOVE:
            drawHappyMouth();
            break;
        case Expression::SAD:
            drawSadMouth();
            break;
        case Expression::SURPRISED:
            drawSurprisedMouth();
            break;
        case Expression::SLEEPY:
            drawSleepyMouth();
            break;
        case Expression::ANGRY:
            drawAngryMouth();
            break;
        case Expression::EATING:
            drawEatingMouth();
            break;
        case Expression::SLEEPING:
            lv_obj_add_flag(_mouth, LV_OBJ_FLAG_HIDDEN);
            break;
        case Expression::EVOLVING:
            drawSurprisedMouth();
            break;
        default:
            drawIdleMouth();
            break;
    }
}

void EyeRenderer::drawIdleMouth() {
    lv_obj_set_size(_mouth, 20, 4);
    lv_obj_set_pos(_mouth, 110, 128);
    lv_obj_set_style_radius(_mouth, 2, 0);
}

void EyeRenderer::drawHappyMouth() {
    lv_obj_set_size(_mouth, 28, 12);
    lv_obj_set_pos(_mouth, 106, 124);
    lv_obj_set_style_radius(_mouth, LV_RADIUS_CIRCLE, 0);
}

void EyeRenderer::drawSadMouth() {
    lv_obj_set_size(_mouth, 20, 6);
    lv_obj_set_pos(_mouth, 110, 130);
    lv_obj_set_style_radius(_mouth, 2, 0);
}

void EyeRenderer::drawSurprisedMouth() {
    lv_obj_set_size(_mouth, 14, 14);
    lv_obj_set_pos(_mouth, 113, 124);
    lv_obj_set_style_radius(_mouth, LV_RADIUS_CIRCLE, 0);
}

void EyeRenderer::drawSleepyMouth() {
    lv_obj_set_size(_mouth, 16, 4);
    lv_obj_set_pos(_mouth, 112, 128);
    lv_obj_set_style_radius(_mouth, 2, 0);
}

void EyeRenderer::drawAngryMouth() {
    lv_obj_set_size(_mouth, 24, 6);
    lv_obj_set_pos(_mouth, 108, 128);
    lv_obj_set_style_radius(_mouth, 0, 0);
}

void EyeRenderer::drawLoveMouth() {
    drawHappyMouth();
}

void EyeRenderer::drawEatingMouth() {
    lv_obj_set_size(_mouth, 16, 16);
    lv_obj_set_pos(_mouth, 112, 122);
    lv_obj_set_style_radius(_mouth, LV_RADIUS_CIRCLE, 0);
}

void EyeRenderer::drawSleepingMouth() {
    lv_obj_add_flag(_mouth, LV_OBJ_FLAG_HIDDEN);
}

float EyeRenderer::randomBlinkInterval() {
    // Random interval between 2 and 6 seconds
    uint32_t r = esp_random();
    return 2.0f + (float)(r % 400) / 100.0f;
}

} // namespace PocketSpirit
