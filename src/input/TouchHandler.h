#pragma once

#include <lvgl.h>
#include <functional>

namespace PocketSpirit {

enum class TouchZone : uint8_t {
    NONE   = 0,
    CENTER = 1,
    TOP    = 2,
    BOTTOM = 3,
    LEFT   = 4,
    RIGHT  = 5
};

struct TouchPoint {
    int16_t x = 0;
    int16_t y = 0;
    bool  pressed = false;
};

class TouchHandler {
public:
    TouchHandler();

    bool init(uint16_t screenWidth = 240, uint16_t screenHeight = 240);
    void update();

    TouchPoint getTouch() const;
    TouchZone getZone(int16_t x, int16_t y) const;

    void onTouch(std::function<void(TouchZone, const TouchPoint&)> callback);

    static constexpr uint8_t ZONE_THRESHOLD = 40;

private:
    uint16_t _screenW = 240;
    uint16_t _screenH = 240;
    TouchPoint _lastTouch;
    bool _wasPressed = false;

    std::function<void(TouchZone, const TouchPoint&)> _callback;

    // LVGL input device (only when touch is enabled)
    #ifdef TOUCH_CS
    lv_indev_drv_t    _indevDrv;
    lv_indev_t*       _indev  = nullptr;

    static void readCallback(lv_indev_drv_t* drv, lv_indev_data_t* data);
    #else
    lv_indev_t*       _indev  = nullptr;
    #endif
};

} // namespace PocketSpirit
