#pragma once

#include <functional>

namespace PocketSpirit {

enum class MotionEvent : uint8_t {
    NONE       = 0,
    SHAKE      = 1,
    TILT_LEFT  = 2,
    TILT_RIGHT = 3,
    TILT_UP    = 4,
    TILT_DOWN  = 5,
    TAP        = 6
};

struct MotionData {
    float accelX = 0;
    float accelY = 0;
    float accelZ = 0;
};

class MotionHandler {
public:
    MotionHandler();

    bool init(int8_t sdaPin = -1, int8_t sclPin = -1);
    void update();

    MotionData getData() const;
    MotionEvent detectEvent() const;

    void onMotion(std::function<void(MotionEvent)> callback);

    void setShakeThreshold(float threshold);
    void setTiltThreshold(float threshold);

private:
    MotionData _data;
    bool _initialized = false;

    float _shakeThreshold = 2.5f;
    float _tiltThreshold  = 0.7f;

    float _lastAccelMag   = 0;
    unsigned long _lastShakeTime = 0;

    std::function<void(MotionEvent)> _callback;

    void readIMU();
};

} // namespace PocketSpirit
