#pragma once

#include <functional>

namespace PocketSpirit {

enum class PowerState : uint8_t {
    ACTIVE     = 0,
    DIMMING    = 1,
    IDLE       = 2,
    DEEP_SLEEP = 3
};

class PowerManager {
public:
    PowerManager();

    bool init();
    void update(float deltaTimeSec);

    void refresh();
    void setInactivityTimeout(uint32_t ms);
    void setDimTimeout(uint32_t ms);
    void setSleepTimeout(uint32_t ms);

    PowerState getState() const { return _state; }
    void enterDeepSleep();

    void onPreSleep(std::function<void()> callback);
    void onWake(std::function<void()> callback);

    static void configureWakeSources(uint8_t touchPin, uint32_t sleepIntervalSec);

    static constexpr uint32_t DEFAULT_INACTIVITY_MS = 30000;  // 30s
    static constexpr uint32_t DEFAULT_DIM_MS        = 15000;  // 15s
    static constexpr uint32_t DEFAULT_SLEEP_MS      = 60000;  // 60s

private:
    PowerState  _state = PowerState::ACTIVE;
    uint32_t    _inactivityMs   = DEFAULT_INACTIVITY_MS;
    uint32_t    _dimMs          = DEFAULT_DIM_MS;
    uint32_t    _sleepMs        = DEFAULT_SLEEP_MS;
    unsigned long _lastActivity = 0;
    bool        _initialized    = false;

    std::function<void()> _preSleepCb;
    std::function<void()> _wakeCb;
};

} // namespace PocketSpirit
