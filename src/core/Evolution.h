#pragma once

#include "CreatureState.h"

namespace PocketSpirit {

class Evolution {
public:
    Evolution();

    EvolutionStage getStage() const { return _stage; }
    void setStage(EvolutionStage stage) { _stage = stage; }

    bool checkEvolution(const CreatureState& state);

    void setThresholds(EvolutionStage targetStage, const EvolutionThresholds& th);
    const EvolutionThresholds& getThresholds(EvolutionStage stage) const;

    static constexpr uint8_t STAGE_COUNT = 3;

private:
    EvolutionStage _stage = EvolutionStage::EGG;
    EvolutionThresholds _thresholds[STAGE_COUNT];

    bool meetsThresholds(const CreatureState& state, const EvolutionThresholds& th) const;
};

} // namespace PocketSpirit
