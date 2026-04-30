#pragma once

#include <lvgl.h>
#include "core/CreatureState.h"
#include "display/AnimationEngine.h"

namespace PocketSpirit {

class CreatureBase {
public:
    virtual ~CreatureBase() = default;

    // Identity
    virtual const char* getName() const = 0;
    virtual const char* getDescription() const = 0;

    // Appearance
    virtual lv_color_t getPrimaryColor() const = 0;
    virtual lv_color_t getSecondaryColor() const = 0;
    virtual lv_color_t getEyeColor() const = 0;

    // Evolution
    virtual EvolutionThresholds getEvolutionThresholds(EvolutionStage stage) const = 0;

    // Custom body drawing (called after eyes are set up)
    virtual void drawBody(lv_obj_t* parent) = 0;

    // Interaction responses
    virtual void onPet()  = 0;
    virtual void onFeed() = 0;
    virtual void onPlay() = 0;
    virtual void onWake() = 0;
    virtual void onSleep() = 0;

    // Animations
    virtual const AnimationFrame* getIdleAnimation(uint8_t& count) const = 0;
    virtual const AnimationFrame* getHappyAnimation(uint8_t& count) const = 0;
    virtual const AnimationFrame* getSadAnimation(uint8_t& count) const = 0;
    virtual const AnimationFrame* getEatingAnimation(uint8_t& count) const = 0;
    virtual const AnimationFrame* getEvolutionAnimation(uint8_t& count) const = 0;

    // Stats modifiers (how much each action affects this creature)
    virtual float getPetAffectionGain()   const { return 8.0f; }
    virtual float getPetHappinessGain()   const { return 5.0f; }
    virtual float getFeedEnergyGain()     const { return 25.0f; }
    virtual float getFeedHappinessGain()  const { return 3.0f; }
    virtual float getPlayHappinessGain()  const { return 15.0f; }
    virtual float getPlayEnergyCost()     const { return 10.0f; }
};

} // namespace PocketSpirit
