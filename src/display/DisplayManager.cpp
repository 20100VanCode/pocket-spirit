#include "DisplayManager.h"
#include <TFT_eSPI.h>
#include <esp_timer.h>

namespace PocketSpirit {

TFT_eSPI tft = TFT_eSPI();

DisplayManager::DisplayManager() {}

bool DisplayManager::init(uint16_t width, uint16_t height) {
    if (_initialized) return true;

    _width  = width;
    _height = height;

    // Init TFT_eSPI
    tft.begin();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);

    // Simple backlight control - just turn it on
    #ifdef TFT_BL
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);  // Full brightness
    #endif

    // Init LVGL
    lv_init();

    _buf1 = new lv_color_t[_width * 20];
    if (!_buf1) return false;

    lv_disp_draw_buf_init(&_drawBuf, _buf1, nullptr, _width * 20);

    lv_disp_drv_init(&_dispDrv);
    _dispDrv.hor_res  = _width;
    _dispDrv.ver_res  = _height;
    _dispDrv.flush_cb = flushCallback;
    _dispDrv.draw_buf = &_drawBuf;
    _dispDrv.user_data = this;
    _dispDrv.sw_rotate = 0;
    _dispDrv.rotated   = LV_DISP_ROT_NONE;

    _display = lv_disp_drv_register(&_dispDrv);
    if (!_display) {
        delete[] _buf1;
        _buf1 = nullptr;
        return false;
    }

    _initialized = true;
    return true;
}

void DisplayManager::update() {
    if (!_initialized) return;
    lv_timer_handler();
}

lv_obj_t* DisplayManager::getScreen() const {
    if (!_display) return nullptr;
    return lv_scr_act();
}

void DisplayManager::setBrightness(uint8_t percent) {
    if (percent > 100) percent = 100;
    _brightness = percent;
    #ifdef TFT_BL
    // Simple on/off - no PWM dimming for now
    digitalWrite(TFT_BL, percent > 0 ? HIGH : LOW);
    #endif
}

void DisplayManager::dim(uint8_t percent) {
    _savedBrightness = _brightness;
    setBrightness(percent);
}

void DisplayManager::restoreBrightness() {
    setBrightness(_savedBrightness);
}

void DisplayManager::flushCallback(lv_disp_drv_t* drv, const lv_area_t* area, lv_color_t* color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t*)&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(drv);
}

} // namespace PocketSpirit
