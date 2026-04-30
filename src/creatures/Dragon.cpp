#include "Dragon.h"

namespace PocketSpirit {

EvolutionThresholds Dragon::getEvolutionThresholds(EvolutionStage stage) const {
    switch (stage) {
        case EvolutionStage::EGG:
            return { .minAgeSeconds = 90, .minInteractions = 8,
                     .minEnergy = 40.0f, .minHappiness = 25.0f, .minAffection = 15.0f };
        case EvolutionStage::BABY:
            return { .minAgeSeconds = 420, .minInteractions = 30,
                     .minEnergy = 60.0f, .minHappiness = 55.0f, .minAffection = 35.0f };
        default:
            return { .minAgeSeconds = UINT32_MAX, .minInteractions = UINT32_MAX,
                     .minEnergy = 999.0f, .minHappiness = 999.0f, .minAffection = 999.0f };
    }
}

void Dragon::drawBody(lv_obj_t* parent) {
    // Small horns
    lv_obj_t* hornL = lv_obj_create(parent);
    lv_obj_set_size(hornL, 8, 20);
    lv_obj_set_pos(hornL, 72, 58);
    lv_obj_set_style_bg_color(hornL, lv_color_make(0x2E, 0x7D, 0x32), 0);
    lv_obj_set_style_bg_opa(hornL, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(hornL, 0, 0);
    lv_obj_set_style_radius(hornL, 4, 0);
    lv_obj_clear_flag(hornL, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* hornR = lv_obj_create(parent);
    lv_obj_set_size(hornR, 8, 20);
    lv_obj_set_pos(hornR, 160, 58);
    lv_obj_set_style_bg_color(hornR, lv_color_make(0x2E, 0x7D, 0x32), 0);
    lv_obj_set_style_bg_opa(hornR, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(hornR, 0, 0);
    lv_obj_set_style_radius(hornR, 4, 0);
    lv_obj_clear_flag(hornR, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
}

void Dragon::onPet()  {}
void Dragon::onFeed() {}
void Dragon::onPlay() {}
void Dragon::onWake() {}
void Dragon::onSleep() {}

static const AnimationFrame idleAnim[] = {
    { Expression::IDLE,    4000 },
    { Expression::BLINK,   150 },
    { Expression::IDLE,    3000 },
    { Expression::ANGRY,   300 },
    { Expression::IDLE,    2000 },
};

static const AnimationFrame happyAnim[] = {
    { Expression::HAPPY,   600 },
    { Expression::SURPRISED, 300 },
    { Expression::HAPPY,   800 },
};

static const AnimationFrame sadAnim[] = {
    { Expression::SAD,     2000 },
    { Expression::ANGRY,   500 },
    { Expression::SAD,     1000 },
};

static const AnimationFrame eatingAnim[] = {
    { Expression::EATING,  500 },
    { Expression::HAPPY,   300 },
    { Expression::EATING,  500 },
};

static const AnimationFrame evolveAnim[] = {
    { Expression::SURPRISED, 300 },
    { Expression::ANGRY,     500 },
    { Expression::EVOLVING,  1200 },
    { Expression::HAPPY,     800 },
};

const AnimationFrame* Dragon::getIdleAnimation(uint8_t& count) const {
    count = sizeof(idleAnim) / sizeof(idleAnim[0]);
    return idleAnim;
}

const AnimationFrame* Dragon::getHappyAnimation(uint8_t& count) const {
    count = sizeof(happyAnim) / sizeof(happyAnim[0]);
    return happyAnim;
}

const AnimationFrame* Dragon::getSadAnimation(uint8_t& count) const {
    count = sizeof(sadAnim) / sizeof(sadAnim[0]);
    return sadAnim;
}

const AnimationFrame* Dragon::getEatingAnimation(uint8_t& count) const {
    count = sizeof(eatingAnim) / sizeof(eatingAnim[0]);
    return eatingAnim;
}

const AnimationFrame* Dragon::getEvolutionAnimation(uint8_t& count) const {
    count = sizeof(evolveAnim) / sizeof(evolveAnim[0]);
    return evolveAnim;
}

} // namespace PocketSpirit
