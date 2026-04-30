#pragma once

#include "creatures/CreatureBase.h"

namespace PocketSpirit {

/**
 * MyCreature - A brief description of your creature
 * 
 * Personality traits, lore, or any fun details go here.
 */
class MyCreature : public CreatureBase {
public:
    // --- Identity ---
    const char* getName() const override         { return "MyCreature"; }
    const char* getDescription() const override   { return "A custom creature template"; }

    // --- Appearance ---
    lv_color_t getPrimaryColor() const override   { return lv_color_make(0xFF, 0xFF, 0xFF); }
    lv_color_t getSecondaryColor() const override  { return lv_color_make(0xCC, 0xCC, 0xCC); }
    lv_color_t getEyeColor() const override       { return lv_color_make(0x00, 0x00, 0x00); }

    // --- Evolution ---
    EvolutionThresholds getEvolutionThresholds(EvolutionStage stage) const override {
        switch (stage) {
            case EvolutionStage::EGG:
                return { .minAgeSeconds = 60, .minInteractions = 5,
                         .minEnergy = 30.0f, .minHappiness = 30.0f, .minAffection = 10.0f };
            case EvolutionStage::BABY:
                return { .minAgeSeconds = 300, .minInteractions = 20,
                         .minEnergy = 50.0f, .minHappiness = 50.0f, .minAffection = 30.0f };
            default:
                return { .minAgeSeconds = UINT32_MAX, .minInteractions = UINT32_MAX,
                         .minEnergy = 999.0f, .minHappiness = 999.0f, .minAffection = 999.0f };
        }
    }

    // --- Custom body drawing ---
    void drawBody(lv_obj_t* parent) override {
        // Add your custom LVGL objects here
        // Example: a small accessory
        lv_obj_t* accessory = lv_obj_create(parent);
        lv_obj_set_size(accessory, 10, 10);
        lv_obj_align(accessory, LV_ALIGN_TOP_RIGHT, -60, 50);
        lv_obj_set_style_bg_color(accessory, getSecondaryColor(), 0);
        lv_obj_set_style_bg_opa(accessory, LV_OPA_COVER, 0);
        lv_obj_set_style_border_width(accessory, 0, 0);
        lv_obj_set_style_radius(accessory, LV_RADIUS_CIRCLE, 0);
        lv_obj_clear_flag(accessory, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    }

    // --- Interaction responses ---
    void onPet() override  { /* custom response */ }
    void onFeed() override { /* custom response */ }
    void onPlay() override { /* custom response */ }
    void onWake() override { /* custom response */ }
    void onSleep() override { /* custom response */ }

    // --- Animations ---
    const AnimationFrame* getIdleAnimation(uint8_t& count) const override {
        static const AnimationFrame frames[] = {
            { Expression::IDLE,  3000 },
            { Expression::BLINK, 150 },
            { Expression::IDLE,  4000 },
        };
        count = sizeof(frames) / sizeof(frames[0]);
        return frames;
    }

    const AnimationFrame* getHappyAnimation(uint8_t& count) const override {
        static const AnimationFrame frames[] = {
            { Expression::HAPPY, 800 },
            { Expression::LOVE,  600 },
            { Expression::HAPPY, 800 },
        };
        count = sizeof(frames) / sizeof(frames[0]);
        return frames;
    }

    const AnimationFrame* getSadAnimation(uint8_t& count) const override {
        static const AnimationFrame frames[] = {
            { Expression::SAD, 1500 },
            { Expression::IDLE, 500 },
            { Expression::SAD, 1000 },
        };
        count = sizeof(frames) / sizeof(frames[0]);
        return frames;
    }

    const AnimationFrame* getEatingAnimation(uint8_t& count) const override {
        static const AnimationFrame frames[] = {
            { Expression::EATING, 400 },
            { Expression::HAPPY,  200 },
            { Expression::EATING, 400 },
        };
        count = sizeof(frames) / sizeof(frames[0]);
        return frames;
    }

    const AnimationFrame* getEvolutionAnimation(uint8_t& count) const override {
        static const AnimationFrame frames[] = {
            { Expression::SURPRISED, 500 },
            { Expression::EVOLVING,  1500 },
            { Expression::HAPPY,     1000 },
        };
        count = sizeof(frames) / sizeof(frames[0]);
        return frames;
    }

    // --- Stat modifiers (optional overrides) ---
    // float getPetAffectionGain()   const override { return 8.0f; }
    // float getPetHappinessGain()   const override { return 5.0f; }
    // float getFeedEnergyGain()     const override { return 25.0f; }
    // float getFeedHappinessGain()  const override { return 3.0f; }
    // float getPlayHappinessGain()  const override { return 15.0f; }
    // float getPlayEnergyCost()     const override { return 10.0f; }
};

} // namespace PocketSpirit
