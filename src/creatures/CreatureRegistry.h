#pragma once

#include "CreatureBase.h"

namespace PocketSpirit {

class CreatureRegistry {
public:
    static CreatureRegistry& instance();

    void registerCreature(CreatureBase* creature);
    CreatureBase* getCreature(const char* name) const;
    uint8_t getCreatureCount() const { return _count; }
    CreatureBase* getCreatureByIndex(uint8_t index) const;

private:
    CreatureRegistry() = default;
    CreatureRegistry(const CreatureRegistry&) = delete;
    CreatureRegistry& operator=(const CreatureRegistry&) = delete;

    static constexpr uint8_t MAX_CREATURES = 16;
    CreatureBase* _creatures[MAX_CREATURES] = {nullptr};
    uint8_t _count = 0;
};

} // namespace PocketSpirit
