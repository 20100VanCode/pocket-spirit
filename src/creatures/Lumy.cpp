#include "Lumy.h"

namespace PocketSpirit {

EvolutionThresholds Lumy::getEvolutionThresholds(EvolutionStage stage) const {
    switch (stage) {
        case EvolutionStage::EGG:
            return { .minAgeSeconds = 60, .minInteractions = 5,
                     .minEnergy = 30.0f, .minHappiness = 30.0f, .minAffection = 10.0f };
        case EvolutionStage::BABY:
            return { .minAgeSeconds = 300, .minInteractions = 25,
                     .minEnergy = 50.0f, .minHappiness = 50.0f, .minAffection = 40.0f };
        default:
            return { .minAgeSeconds = UINT32_MAX, .minInteractions = UINT32_MAX,
                     .minEnergy = 999.0f, .minHappiness = 999.0f, .minAffection = 999.0f };
    }
}

void Lumy::drawBody(lv_obj_t* parent) {
    // Glow effect behind the body
    lv_obj_t* glow = lv_obj_create(parent);
    lv_obj_set_size(glow, 180, 140);
    lv_obj_align(glow, LV_ALIGN_CENTER, 0, 10);
    lv_obj_set_style_bg_color(glow, lv_color_make(0xFF, 0xEB, 0x3B), 0);
    lv_obj_set_style_bg_opa(glow, LV_OPA_30, 0);
    lv_obj_set_style_border_width(glow, 0, 0);
    lv_obj_set_style_radius(glow, LV_RADIUS_CIRCLE, 0);
    lv_obj_clear_flag(glow, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
}

void Lumy::onPet()  {}
void Lumy::onFeed() {}
void Lumy::onPlay() {}
void Lumy::onWake() {}
void Lumy::onSleep() {}

static const AnimationFrame idleAnim[] = {
    { Expression::IDLE,    3000 },
    { Expression::BLINK,   150 },
    { Expression::IDLE,    4000 },
    { Expression::HAPPY,   500 },
    { Expression::IDLE,    2000 },
};

static const AnimationFrame happyAnim[] = {
    { Expression::HAPPY,   800 },
    { Expression::IDLE,    200 },
    { Expression::HAPPY,   600 },
    { Expression::LOVE,    800 },
    { Expression::HAPPY,   600 },
};

static const AnimationFrame sadAnim[] = {
    { Expression::SAD,     1500 },
    { Expression::IDLE,    500 },
    { Expression::SAD,     1000 },
};

static const AnimationFrame eatingAnim[] = {
    { Expression::EATING,  400 },
    { Expression::HAPPY,   200 },
    { Expression::EATING,  400 },
    { Expression::HAPPY,   200 },
    { Expression::EATING,  400 },
};

static const AnimationFrame evolveAnim[] = {
    { Expression::SURPRISED, 500 },
    { Expression::EVOLVING,  1500 },
    { Expression::HAPPY,    1000 },
};

const AnimationFrame* Lumy::getIdleAnimation(uint8_t& count) const {
    count = sizeof(idleAnim) / sizeof(idleAnim[0]);
    return idleAnim;
}

const AnimationFrame* Lumy::getHappyAnimation(uint8_t& count) const {
    count = sizeof(happyAnim) / sizeof(happyAnim[0]);
    return happyAnim;
}

const AnimationFrame* Lumy::getSadAnimation(uint8_t& count) const {
    count = sizeof(sadAnim) / sizeof(sadAnim[0]);
    return sadAnim;
}

const AnimationFrame* Lumy::getEatingAnimation(uint8_t& count) const {
    count = sizeof(eatingAnim) / sizeof(eatingAnim[0]);
    return eatingAnim;
}

const AnimationFrame* Lumy::getEvolutionAnimation(uint8_t& count) const {
    count = sizeof(evolveAnim) / sizeof(evolveAnim[0]);
    return evolveAnim;
}

} // namespace PocketSpirit
