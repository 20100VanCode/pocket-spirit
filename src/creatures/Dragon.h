#pragma once

#include "CreatureBase.h"

namespace PocketSpirit {

class Dragon : public CreatureBase {
public:
    const char* getName() const override         { return "Dragon"; }
    const char* getDescription() const override   { return "A fierce little dragon with a heart of gold"; }

    lv_color_t getPrimaryColor() const override   { return lv_color_make(0x4C, 0xAF, 0x50); } // Green
    lv_color_t getSecondaryColor() const override  { return lv_color_make(0x2E, 0x7D, 0x32); } // Dark green
    lv_color_t getEyeColor() const override       { return lv_color_make(0xFF, 0xF1, 0x76); } // Yellow

    EvolutionThresholds getEvolutionThresholds(EvolutionStage stage) const override;

    void drawBody(lv_obj_t* parent) override;

    void onPet() override;
    void onFeed() override;
    void onPlay() override;
    void onWake() override;
    void onSleep() override;

    const AnimationFrame* getIdleAnimation(uint8_t& count) const override;
    const AnimationFrame* getHappyAnimation(uint8_t& count) const override;
    const AnimationFrame* getSadAnimation(uint8_t& count) const override;
    const AnimationFrame* getEatingAnimation(uint8_t& count) const override;
    const AnimationFrame* getEvolutionAnimation(uint8_t& count) const override;

    float getPetAffectionGain()  const override { return 6.0f; }
    float getFeedEnergyGain()    const override { return 35.0f; }
    float getPlayHappinessGain() const override { return 20.0f; }
    float getPlayEnergyCost()    const override { return 15.0f; }
};

} // namespace PocketSpirit
