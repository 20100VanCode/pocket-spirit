#include "MotionHandler.h"
#include <Arduino.h>

// Optional IMU support - compile-time conditional
#if __has_include(<MPU6886.h>)
  #include <MPU6886.h>
  #define HAS_IMU 1
  static MPU6886 imu;
#endif

namespace PocketSpirit {

MotionHandler::MotionHandler() {}

bool MotionHandler::init(int8_t sdaPin, int8_t sclPin) {
#if HAS_IMU
    if (sdaPin >= 0 && sclPin >= 0) {
        Wire.begin(sdaPin, sclPin);
    } else {
        Wire.begin();
    }

    int result = imu.Init();
    _initialized = (result == 0);
    if (_initialized) {
        imu.SetGyroFsr(GyroScale::MPU6886_GYRO_FS_500);
        imu.SetAccelFsr(AccelScale::MPU6886_ACCEL_FS_2);
    }
#else
    _initialized = false;
#endif
    return _initialized;
}

void MotionHandler::update() {
    if (!_initialized) return;

    readIMU();

    MotionEvent evt = detectEvent();
    if (evt != MotionEvent::NONE && _callback) {
        _callback(evt);
    }
}

void MotionHandler::readIMU() {
#if HAS_IMU
    float ax, ay, az;
    imu.getAccel(&ax, &ay, &az);
    _data.accelX = ax;
    _data.accelY = ay;
    _data.accelZ = az;
#endif
}

MotionData MotionHandler::getData() const {
    return _data;
}

MotionEvent MotionHandler::detectEvent() const {
    float mag = sqrtf(_data.accelX * _data.accelX
                    + _data.accelY * _data.accelY
                    + _data.accelZ * _data.accelZ);

    // Detect shake
    if (mag > _shakeThreshold) {
        unsigned long now = millis();
        if (now - _lastShakeTime > 300) {
            return MotionEvent::SHAKE;
        }
    }

    // Detect tilt (when Z-axis is not dominant)
    if (fabsf(_data.accelZ) < _tiltThreshold) {
        if (_data.accelX > _tiltThreshold)  return MotionEvent::TILT_RIGHT;
        if (_data.accelX < -_tiltThreshold) return MotionEvent::TILT_LEFT;
        if (_data.accelY > _tiltThreshold)  return MotionEvent::TILT_DOWN;
        if (_data.accelY < -_tiltThreshold) return MotionEvent::TILT_UP;
    }

    return MotionEvent::NONE;
}

void MotionHandler::onMotion(std::function<void(MotionEvent)> callback) {
    _callback = callback;
}

void MotionHandler::setShakeThreshold(float threshold) {
    _shakeThreshold = threshold;
}

void MotionHandler::setTiltThreshold(float threshold) {
    _tiltThreshold = threshold;
}

} // namespace PocketSpirit
