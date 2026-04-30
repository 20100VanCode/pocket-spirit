#pragma once

#include "CreatureState.h"

namespace PocketSpirit {

class Stats {
public:
    Stats();

    void update(float deltaTimeSec);

    void modifyEnergy(float delta);
    void modifyHappiness(float delta);
    void modifyAffection(float delta);

    float getEnergy() const    { return _stats.energy; }
    float getHappiness() const { return _stats.happiness; }
    float getAffection() const { return _stats.affection; }

    void setDecayRates(float energyPerSec, float happinessPerSec, float affectionPerSec);

    const CreatureStats& getStats() const   { return _stats; }
    void setStats(const CreatureStats& s)   { _stats = s; _stats.clamp(); }

    bool isLowEnergy() const      { return _stats.energy < 25.0f; }
    bool isCriticalEnergy() const { return _stats.energy < 10.0f; }
    bool isLowHappiness() const   { return _stats.happiness < 25.0f; }

private:
    CreatureStats _stats;

    float _energyDecayPerSec    = 0.05f;   // ~5 min to go 100→0
    float _happinessDecayPerSec = 0.03f;   // ~8 min
    float _affectionDecayPerSec = 0.01f;   // ~25 min
};

} // namespace PocketSpirit
