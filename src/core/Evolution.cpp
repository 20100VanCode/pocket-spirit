#include "Evolution.h"
#include <cstring>

namespace PocketSpirit {

Evolution::Evolution() {
    // EGG → BABY defaults
    _thresholds[0] = {
        .minAgeSeconds     = 60,     // 1 minute
        .minInteractions  = 5,
        .minEnergy        = 30.0f,
        .minHappiness     = 30.0f,
        .minAffection     = 10.0f
    };

    // BABY → ADULT defaults
    _thresholds[1] = {
        .minAgeSeconds     = 300,    // 5 minutes
        .minInteractions  = 20,
        .minEnergy        = 50.0f,
        .minHappiness     = 50.0f,
        .minAffection     = 30.0f
    };

    // ADULT (no further evolution)
    _thresholds[2] = {
        .minAgeSeconds     = UINT32_MAX,
        .minInteractions  = UINT32_MAX,
        .minEnergy        = 999.0f,
        .minHappiness     = 999.0f,
        .minAffection     = 999.0f
    };
}

bool Evolution::checkEvolution(const CreatureState& state) {
    if (_stage == EvolutionStage::ADULT) return false;

    uint8_t idx = static_cast<uint8_t>(_stage);
    if (meetsThresholds(state, _thresholds[idx])) {
        _stage = static_cast<EvolutionStage>(idx + 1);
        return true;
    }
    return false;
}

bool Evolution::meetsThresholds(const CreatureState& state, const EvolutionThresholds& th) const {
    return state.ageSeconds      >= th.minAgeSeconds
        && state.totalInteractions >= th.minInteractions
        && state.stats.energy     >= th.minEnergy
        && state.stats.happiness  >= th.minHappiness
        && state.stats.affection  >= th.minAffection;
}

void Evolution::setThresholds(EvolutionStage targetStage, const EvolutionThresholds& th) {
    uint8_t idx = static_cast<uint8_t>(targetStage);
    if (idx < STAGE_COUNT) {
        _thresholds[idx] = th;
    }
}

const EvolutionThresholds& Evolution::getThresholds(EvolutionStage stage) const {
    uint8_t idx = static_cast<uint8_t>(stage);
    if (idx < STAGE_COUNT) return _thresholds[idx];
    return _thresholds[STAGE_COUNT - 1];
}

} // namespace PocketSpirit
