#include "TouchHandler.h"
#include <TFT_eSPI.h>

namespace PocketSpirit {

// Check if touch is enabled in TFT_eSPI config
// TOUCH_CS is defined when touch support is enabled
#ifdef TOUCH_CS
    #define PSPRIT_TOUCH_ENABLED 1
    // Use TFT_eSPI's built-in touch support (only when available)
    extern TFT_eSPI tft;
#else
    #define PSPRIT_TOUCH_ENABLED 0
#endif

TouchHandler::TouchHandler() {}

bool TouchHandler::init(uint16_t screenWidth, uint16_t screenHeight) {
    _screenW = screenWidth;
    _screenH = screenHeight;

#if PSPRIT_TOUCH_ENABLED
    // Register touch input with LVGL
    lv_indev_drv_init(&_indevDrv);
    _indevDrv.type    = LV_INDEV_TYPE_POINTER;
    _indevDrv.read_cb = readCallback;
    _indevDrv.user_data = this;
    _indev = lv_indev_drv_register(&_indevDrv);

    return _indev != nullptr;
#else
    // Touch not available, return success but don't register input device
    _indev = nullptr;
    return true;
#endif
}

void TouchHandler::update() {
#if PSPRIT_TOUCH_ENABLED
    TouchPoint tp = getTouch();

    if (tp.pressed && !_wasPressed) {
        TouchZone zone = getZone(tp.x, tp.y);
        if (_callback) _callback(zone, tp);
    }
    _wasPressed = tp.pressed;
#endif
}

TouchPoint TouchHandler::getTouch() const {
    TouchPoint tp;
#if PSPRIT_TOUCH_ENABLED
    uint16_t x = 0, y = 0;

    if (tft.getTouch(&x, &y, 600)) {
        tp.x = (int16_t)x;
        tp.y = (int16_t)y;
        tp.pressed = true;
    }
#endif
    return tp;
}

TouchZone TouchHandler::getZone(int16_t x, int16_t y) const {
    int16_t cx = (int16_t)(_screenW / 2);
    int16_t cy = (int16_t)(_screenH / 2);

    int16_t dx = x - cx;
    int16_t dy = y - cy;

    // Check if within center zone
    if (abs(dx) < ZONE_THRESHOLD && abs(dy) < ZONE_THRESHOLD) {
        return TouchZone::CENTER;
    }

    // Determine dominant direction
    if (abs(dx) > abs(dy)) {
        return (dx > 0) ? TouchZone::RIGHT : TouchZone::LEFT;
    } else {
        return (dy > 0) ? TouchZone::BOTTOM : TouchZone::TOP;
    }
}

void TouchHandler::onTouch(std::function<void(TouchZone, const TouchPoint&)> callback) {
    _callback = callback;
}

#if PSPRIT_TOUCH_ENABLED
void TouchHandler::readCallback(lv_indev_drv_t* drv, lv_indev_data_t* data) {
    TouchHandler* self = static_cast<TouchHandler*>(drv->user_data);
    if (!self) return;

    TouchPoint tp = self->getTouch();
    data->point.x = tp.x;
    data->point.y = tp.y;
    data->state = tp.pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
    data->continue_reading = false;
}
#endif

} // namespace PocketSpirit
