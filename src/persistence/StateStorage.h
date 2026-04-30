#pragma once

#include <stddef.h>
#include "core/CreatureState.h"

namespace PocketSpirit {

class StateStorage {
public:
    StateStorage();

    bool init(const char* nvsNamespace = "pspirit");
    bool saveState(const CreatureState& state);
    bool loadState(CreatureState& state);
    bool clearState();
    bool hasState();

private:
    bool _initialized = false;
    char _namespace[16];

    bool writeBlob(const char* key, const void* data, size_t len);
    bool readBlob(const char* key, void* data, size_t len);

    static constexpr const char* STATE_KEY = "state";
    static constexpr uint32_t MAGIC_NUMBER = 0x50534354; // "PSCT"
};

} // namespace PocketSpirit
