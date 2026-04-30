#include "CreatureRegistry.h"
#include <cstring>

namespace PocketSpirit {

CreatureRegistry& CreatureRegistry::instance() {
    static CreatureRegistry registry;
    return registry;
}

void CreatureRegistry::registerCreature(CreatureBase* creature) {
    if (!creature || _count >= MAX_CREATURES) return;

    // Avoid duplicates
    for (uint8_t i = 0; i < _count; i++) {
        if (strcmp(_creatures[i]->getName(), creature->getName()) == 0) return;
    }

    _creatures[_count++] = creature;
}

CreatureBase* CreatureRegistry::getCreature(const char* name) const {
    for (uint8_t i = 0; i < _count; i++) {
        if (strcmp(_creatures[i]->getName(), name) == 0) {
            return _creatures[i];
        }
    }
    return nullptr;
}

CreatureBase* CreatureRegistry::getCreatureByIndex(uint8_t index) const {
    if (index < _count) return _creatures[index];
    return nullptr;
}

} // namespace PocketSpirit
