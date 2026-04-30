#include "StateStorage.h"
#include <nvs.h>
#include <nvs_flash.h>
#include <cstring>

namespace PocketSpirit {

struct StorageHeader {
    uint32_t magic;
    uint16_t version;
    uint16_t crc16;
};

StateStorage::StateStorage() {
    strncpy(_namespace, "pspirit", sizeof(_namespace));
}

bool StateStorage::init(const char* nvsNamespace) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK && err != ESP_ERR_NVS_ALREADY_INITIALIZED) {
        return false;
    }

    if (nvsNamespace) {
        strncpy(_namespace, nvsNamespace, sizeof(_namespace) - 1);
        _namespace[sizeof(_namespace) - 1] = '\0';
    }

    _initialized = true;
    return true;
}

static uint16_t crc16_ccitt(const uint8_t* data, size_t len) {
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }
    }
    return crc;
}

bool StateStorage::saveState(const CreatureState& state) {
    if (!_initialized) return false;

    StorageHeader header;
    header.magic   = MAGIC_NUMBER;
    header.version = 1;
    header.crc16   = crc16_ccitt(reinterpret_cast<const uint8_t*>(&state), sizeof(CreatureState));

    nvs_handle_t handle;
    if (nvs_open(_namespace, NVS_READWRITE, &handle) != ESP_OK) return false;

    bool ok = true;
    if (nvs_set_blob(handle, "hdr", &header, sizeof(header)) != ESP_OK) ok = false;
    if (nvs_set_blob(handle, STATE_KEY, &state, sizeof(CreatureState)) != ESP_OK) ok = false;

    if (ok) nvs_commit(handle);
    nvs_close(handle);
    return ok;
}

bool StateStorage::loadState(CreatureState& state) {
    if (!_initialized) return false;

    nvs_handle_t handle;
    if (nvs_open(_namespace, NVS_READONLY, &handle) != ESP_OK) return false;

    StorageHeader header;
    size_t hdrSize = sizeof(header);
    if (nvs_get_blob(handle, "hdr", &header, &hdrSize) != ESP_OK) {
        nvs_close(handle);
        return false;
    }

    if (header.magic != MAGIC_NUMBER || header.version != 1) {
        nvs_close(handle);
        return false;
    }

    CreatureState loaded;
    size_t stateSize = sizeof(CreatureState);
    if (nvs_get_blob(handle, STATE_KEY, &loaded, &stateSize) != ESP_OK) {
        nvs_close(handle);
        return false;
    }
    nvs_close(handle);

    uint16_t crc = crc16_ccitt(reinterpret_cast<const uint8_t*>(&loaded), sizeof(CreatureState));
    if (crc != header.crc16) return false;

    state = loaded;
    return true;
}

bool StateStorage::clearState() {
    if (!_initialized) return false;

    nvs_handle_t handle;
    if (nvs_open(_namespace, NVS_READWRITE, &handle) != ESP_OK) return false;

    esp_err_t err = nvs_erase_key(handle, "hdr");
    nvs_erase_key(handle, STATE_KEY);
    nvs_commit(handle);
    nvs_close(handle);
    return err == ESP_OK;
}

bool StateStorage::hasState() {
    if (!_initialized) return false;

    nvs_handle_t handle;
    if (nvs_open(_namespace, NVS_READONLY, &handle) != ESP_OK) return false;

    StorageHeader header;
    size_t hdrSize = sizeof(header);
    bool exists = (nvs_get_blob(handle, "hdr", &header, &hdrSize) == ESP_OK)
               && (header.magic == MAGIC_NUMBER);
    nvs_close(handle);
    return exists;
}

} // namespace PocketSpirit
