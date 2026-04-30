#pragma once

#include "CreatureBase.h"

namespace PocketSpirit {

class Lumy : public CreatureBase {
public:
    const char* getName() const override         { return "Lumy"; }
    const char* getDescription() const override   { return "A warm light spirit that glows with affection"; }

    lv_color_t getPrimaryColor() const override   { return lv_color_make(0xFF, 0xD7, 0x00); } // Gold
    lv_color_t getSecondaryColor() const override  { return lv_color_make(0xFF, 0x8C, 0x00); } // Dark orange
    lv_color_t getEyeColor() const override       { return lv_color_make(0x33, 0x22, 0x00); } // Dark brown

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

    float getPetAffectionGain()  const override { return 10.0f; }
    float getFeedEnergyGain()    const override { return 30.0f; }
    float getPlayHappinessGain() const override { return 18.0f; }
};

} // namespace PocketSpirit
