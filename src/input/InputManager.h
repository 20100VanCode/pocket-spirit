#pragma once

#include "TouchHandler.h"
#include "MotionHandler.h"
#include <functional>

namespace PocketSpirit {

class InputManager {
public:
    InputManager();

    bool init(uint16_t screenWidth = 240, uint16_t screenHeight = 240);
    void update();

    void onTouchEvent(std::function<void(TouchZone, const TouchPoint&)> callback);
    void onMotionEvent(std::function<void(MotionEvent)> callback);

    TouchZone getLastTouchZone() const { return _lastZone; }
    MotionEvent getLastMotionEvent() const { return _lastMotion; }

    void enableTouch(bool enable);
    void enableMotion(bool enable);

private:
    TouchHandler  _touch;
    MotionHandler _motion;

    std::function<void(TouchZone, const TouchPoint&)> _touchCb;
    std::function<void(MotionEvent)> _motionCb;

    TouchZone   _lastZone   = TouchZone::NONE;
    MotionEvent _lastMotion = MotionEvent::NONE;

    bool _touchEnabled = true;
    bool _motionEnabled = true;

    void handleTouch(TouchZone zone, const TouchPoint& pt);
    void handleMotion(MotionEvent evt);
};

} // namespace PocketSpirit
