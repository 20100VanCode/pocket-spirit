#pragma once

#include <lvgl.h>

namespace PocketSpirit {

class DisplayManager {
public:
    DisplayManager();

    bool init(uint16_t width = 240, uint16_t height = 240);
    void update();

    lv_disp_t* getDisplay() const { return _display; }
    lv_obj_t* getScreen() const;
    uint16_t getWidth()  const { return _width; }
    uint16_t getHeight() const { return _height; }

    void setBrightness(uint8_t percent);
    void dim(uint8_t percent = 30);
    void restoreBrightness();

    static constexpr uint16_t DEFAULT_WIDTH  = 240;
    static constexpr uint16_t DEFAULT_HEIGHT = 240;

private:
    lv_disp_draw_buf_t _drawBuf;
    lv_disp_drv_t      _dispDrv;
    lv_disp_t*         _display = nullptr;
    uint16_t           _width   = DEFAULT_WIDTH;
    uint16_t           _height  = DEFAULT_HEIGHT;
    uint8_t            _brightness = 100;
    uint8_t            _savedBrightness = 100;
    bool               _initialized = false;

    lv_color_t* _buf1 = nullptr;

    static void flushCallback(lv_disp_drv_t* drv, const lv_area_t* area, lv_color_t* color_p);
};

} // namespace PocketSpirit
