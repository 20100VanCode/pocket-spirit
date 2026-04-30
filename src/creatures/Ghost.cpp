#include "Ghost.h"

namespace PocketSpirit {

EvolutionThresholds Ghost::getEvolutionThresholds(EvolutionStage stage) const {
    switch (stage) {
        case EvolutionStage::EGG:
            return { .minAgeSeconds = 45, .minInteractions = 3,
                     .minEnergy = 20.0f, .minHappiness = 20.0f, .minAffection = 15.0f };
        case EvolutionStage::BABY:
            return { .minAgeSeconds = 240, .minInteractions = 15,
                     .minEnergy = 40.0f, .minHappiness = 40.0f, .minAffection = 50.0f };
        default:
            return { .minAgeSeconds = UINT32_MAX, .minInteractions = UINT32_MAX,
                     .minEnergy = 999.0f, .minHappiness = 999.0f, .minAffection = 999.0f };
    }
}

void Ghost::drawBody(lv_obj_t* parent) {
    // Wavy bottom edge (ghost tail) - simulated with overlapping circles
    for (int i = 0; i < 3; i++) {
        lv_obj_t* wave = lv_obj_create(parent);
        lv_obj_set_size(wave, 60, 30);
        lv_obj_set_pos(wave, 60 + i * 40, 135);
        lv_obj_set_style_bg_color(wave, lv_color_make(0xE1, 0xBE, 0xE7), 0);
        lv_obj_set_style_bg_opa(wave, LV_OPA_COVER, 0);
        lv_obj_set_style_border_width(wave, 0, 0);
        lv_obj_set_style_radius(wave, LV_RADIUS_CIRCLE, 0);
        lv_obj_clear_flag(wave, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    }
}

void Ghost::onPet()  {}
void Ghost::onFeed() {}
void Ghost::onPlay() {}
void Ghost::onWake() {}
void Ghost::onSleep() {}

static const AnimationFrame idleAnim[] = {
    { Expression::IDLE,    5000 },
    { Expression::BLINK,   150 },
    { Expression::IDLE,    4000 },
    { Expression::SLEEPY,  800 },
    { Expression::IDLE,    3000 },
};

static const AnimationFrame happyAnim[] = {
    { Expression::HAPPY,   1000 },
    { Expression::LOVE,    1200 },
    { Expression::HAPPY,   800 },
};

static const AnimationFrame sadAnim[] = {
    { Expression::SAD,     2000 },
    { Expression::SLEEPY,  1000 },
    { Expression::SAD,     1500 },
};

static const AnimationFrame eatingAnim[] = {
    { Expression::SURPRISED, 300 },
    { Expression::EATING,    500 },
    { Expression::HAPPY,     400 },
    { Expression::EATING,    500 },
};

static const AnimationFrame evolveAnim[] = {
    { Expression::SURPRISED, 600 },
    { Expression::EVOLVING,  1800 },
    { Expression::LOVE,      1000 },
};

const AnimationFrame* Ghost::getIdleAnimation(uint8_t& count) const {
    count = sizeof(idleAnim) / sizeof(idleAnim[0]);
    return idleAnim;
}

const AnimationFrame* Ghost::getHappyAnimation(uint8_t& count) const {
    count = sizeof(happyAnim) / sizeof(happyAnim[0]);
    return happyAnim;
}

const AnimationFrame* Ghost::getSadAnimation(uint8_t& count) const {
    count = sizeof(sadAnim) / sizeof(sadAnim[0]);
    return sadAnim;
}

const AnimationFrame* Ghost::getEatingAnimation(uint8_t& count) const {
    count = sizeof(eatingAnim) / sizeof(eatingAnim[0]);
    return eatingAnim;
}

const AnimationFrame* Ghost::getEvolutionAnimation(uint8_t& count) const {
    count = sizeof(evolveAnim) / sizeof(evolveAnim[0]);
    return evolveAnim;
}

} // namespace PocketSpirit
