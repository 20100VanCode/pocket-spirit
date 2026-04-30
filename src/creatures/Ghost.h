#pragma once

#include "CreatureBase.h"

namespace PocketSpirit {

class Ghost : public CreatureBase {
public:
    const char* getName() const override         { return "Ghost"; }
    const char* getDescription() const override   { return "A shy little phantom that loves company"; }

    lv_color_t getPrimaryColor() const override   { return lv_color_make(0xE1, 0xBE, 0xE7); } // Light purple
    lv_color_t getSecondaryColor() const override  { return lv_color_make(0xCE, 0x93, 0xD8); } // Purple
    lv_color_t getEyeColor() const override       { return lv_color_make(0x4A, 0x14, 0x8C); } // Deep purple

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

    float getPetAffectionGain()  const override { return 12.0f; }
    float getFeedEnergyGain()    const override { return 20.0f; }
    float getPlayHappinessGain() const override { return 12.0f; }
};

} // namespace PocketSpirit
