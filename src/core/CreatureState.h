#pragma once

#include <cstdint>

namespace PocketSpirit {

struct CreatureStats {
    float energy    = 100.0f;
    float happiness = 100.0f;
    float affection = 50.0f;

    void clamp() {
        if (energy < 0) energy = 0;
        if (energy > 100) energy = 100;
        if (happiness < 0) happiness = 0;
        if (happiness > 100) happiness = 100;
        if (affection < 0) affection = 0;
        if (affection > 100) affection = 100;
    }
};

enum class EvolutionStage : uint8_t {
    EGG   = 0,
    BABY  = 1,
    ADULT = 2
};

inline const char* evolutionStageToStr(EvolutionStage stage) {
    switch (stage) {
        case EvolutionStage::EGG:   return "Egg";
        case EvolutionStage::BABY:  return "Baby";
        case EvolutionStage::ADULT: return "Adult";
        default: return "Unknown";
    }
}

enum class Expression : uint8_t {
    IDLE,
    HAPPY,
    SAD,
    SLEEPY,
    SURPRISED,
    ANGRY,
    LOVE,
    EATING,
    BLINK,
    SLEEPING,
    EVOLVING
};

struct EvolutionThresholds {
    uint32_t minAgeSeconds      = 0;
    uint32_t minInteractions    = 0;
    float    minEnergy          = 0;
    float    minHappiness       = 0;
    float    minAffection       = 0;
};

struct CreatureState {
    char            creatureType[32]   = {0};
    CreatureStats   stats;
    EvolutionStage  stage              = EvolutionStage::EGG;
    uint32_t        ageSeconds         = 0;
    uint32_t        lastUpdateMs       = 0;
    uint32_t        totalInteractions  = 0;
    uint32_t        totalPets          = 0;
    uint32_t        totalFeeds         = 0;
    Expression      currentExpression  = Expression::IDLE;
};

} // namespace PocketSpirit
