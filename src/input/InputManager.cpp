#include "InputManager.h"

namespace PocketSpirit {

InputManager::InputManager() {}

bool InputManager::init(uint16_t screenWidth, uint16_t screenHeight) {
    bool touchOk = _touch.init(screenWidth, screenHeight);
    _touch.onTouch([this](TouchZone z, const TouchPoint& p) { handleTouch(z, p); });

    bool motionOk = _motion.init();
    _motion.onMotion([this](MotionEvent e) { handleMotion(e); });

    _touchEnabled  = touchOk;
    _motionEnabled = motionOk;

    return touchOk; // Touch is primary; motion is optional
}

void InputManager::update() {
    if (_touchEnabled)  _touch.update();
    if (_motionEnabled) _motion.update();
}

void InputManager::onTouchEvent(std::function<void(TouchZone, const TouchPoint&)> callback) {
    _touchCb = callback;
}

void InputManager::onMotionEvent(std::function<void(MotionEvent)> callback) {
    _motionCb = callback;
}

void InputManager::enableTouch(bool enable) {
    _touchEnabled = enable;
}

void InputManager::enableMotion(bool enable) {
    _motionEnabled = enable;
}

void InputManager::handleTouch(TouchZone zone, const TouchPoint& pt) {
    _lastZone = zone;
    if (_touchCb) _touchCb(zone, pt);
}

void InputManager::handleMotion(MotionEvent evt) {
    _lastMotion = evt;
    if (_motionCb) _motionCb(evt);
}

} // namespace PocketSpirit
