#include "Stats.h"

namespace PocketSpirit {

Stats::Stats() : _stats{} {
    _stats.energy    = 100.0f;
    _stats.happiness = 100.0f;
    _stats.affection = 50.0f;
}

void Stats::update(float deltaTimeSec) {
    if (deltaTimeSec <= 0) return;

    _stats.energy    -= _energyDecayPerSec * deltaTimeSec;
    _stats.happiness -= _happinessDecayPerSec * deltaTimeSec;
    _stats.affection -= _affectionDecayPerSec * deltaTimeSec;

    _stats.clamp();
}

void Stats::modifyEnergy(float delta) {
    _stats.energy += delta;
    _stats.clamp();
}

void Stats::modifyHappiness(float delta) {
    _stats.happiness += delta;
    _stats.clamp();
}

void Stats::modifyAffection(float delta) {
    _stats.affection += delta;
    _stats.clamp();
}

void Stats::setDecayRates(float energyPerSec, float happinessPerSec, float affectionPerSec) {
    if (energyPerSec >= 0)    _energyDecayPerSec    = energyPerSec;
    if (happinessPerSec >= 0) _happinessDecayPerSec = happinessPerSec;
    if (affectionPerSec >= 0) _affectionDecayPerSec = affectionPerSec;
}

} // namespace PocketSpirit
