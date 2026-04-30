#include "PowerManager.h"
#include <Arduino.h>
#include <esp_sleep.h>
#include <esp_pm.h>

namespace PocketSpirit {

PowerManager::PowerManager() {}

bool PowerManager::init() {
    _lastActivity = millis();
    _state = PowerState::ACTIVE;

    // Configure backlight PWM on GPIO2 (Waveshare 1.28)
    ledcSetup(0, 5000, 8);
    ledcAttachPin(2, 0);
    ledcWrite(0, 255);

    _initialized = true;
    return true;
}

void PowerManager::update(float deltaTimeSec) {
    if (!_initialized) return;

    unsigned long now = millis();
    unsigned long elapsed = now - _lastActivity;

    switch (_state) {
        case PowerState::ACTIVE:
            if (elapsed >= _dimMs) {
                _state = PowerState::DIMMING;
            }
            break;

        case PowerState::DIMMING:
            if (elapsed >= _inactivityMs) {
                _state = PowerState::IDLE;
            } else if (elapsed < _dimMs) {
                _state = PowerState::ACTIVE;
            }
            break;

        case PowerState::IDLE:
            if (elapsed >= _sleepMs) {
                enterDeepSleep();
            } else if (elapsed < _dimMs) {
                _state = PowerState::ACTIVE;
            }
            break;

        case PowerState::DEEP_SLEEP:
            break;
    }
}

void PowerManager::refresh() {
    _lastActivity = millis();
    _state = PowerState::ACTIVE;
}

void PowerManager::setInactivityTimeout(uint32_t ms) {
    _inactivityMs = ms;
}

void PowerManager::setDimTimeout(uint32_t ms) {
    _dimMs = ms;
}

void PowerManager::setSleepTimeout(uint32_t ms) {
    _sleepMs = ms;
}

void PowerManager::enterDeepSleep() {
    if (_preSleepCb) _preSleepCb();

    _state = PowerState::DEEP_SLEEP;

    // Turn off display and backlight
    ledcWrite(0, 0);

    esp_deep_sleep_start();
}

void PowerManager::onPreSleep(std::function<void()> callback) {
    _preSleepCb = callback;
}

void PowerManager::onWake(std::function<void()> callback) {
    _wakeCb = callback;
}

void PowerManager::configureWakeSources(uint8_t touchPin, uint32_t sleepIntervalSec) {
    // Wake on touch (GPIO interrupt)
    if (touchPin < 255) {
        esp_sleep_enable_ext0_wakeup((gpio_num_t)touchPin, 1);
    }

    // Wake on timer (periodic stat decay while sleeping)
    if (sleepIntervalSec > 0) {
        esp_sleep_enable_timer_wakeup((uint64_t)sleepIntervalSec * 1000000ULL);
    }
}

} // namespace PocketSpirit
