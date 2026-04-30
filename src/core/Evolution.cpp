#include "Evolution.h"
#include <cstring>

namespace PocketSpirit {

Evolution::Evolution() {
    // EGG → BABY defaults
    _thresholds[0].minAgeSeconds = 60;     // 1 minute
    _thresholds[0].minInteractions = 5;
    _thresholds[0].minEnergy = 30.0f;
    _thresholds[0].minHappiness = 30.0f;
    _thresholds[0].minAffection = 10.0f;

    // BABY → ADULT defaults
    _thresholds[1].minAgeSeconds = 300;    // 5 minutes
    _thresholds[1].minInteractions = 20;
    _thresholds[1].minEnergy = 50.0f;
    _thresholds[1].minHappiness = 50.0f;
    _thresholds[1].minAffection = 30.0f;

    // ADULT (no further evolution)
    _thresholds[2].minAgeSeconds = UINT32_MAX;
    _thresholds[2].minInteractions = UINT32_MAX;
    _thresholds[2].minEnergy = 999.0f;
    _thresholds[2].minHappiness = 999.0f;
    _thresholds[2].minAffection = 999.0f;
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
