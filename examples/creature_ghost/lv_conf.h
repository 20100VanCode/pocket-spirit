/**
 * @file lv_conf.h
 * Minimal LVGL 8.x configuration for PocketSpirit
 * Target: ESP32 + Waveshare 1.28" Round Display (240x240)
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/* Color settings */
#define LV_COLOR_DEPTH      16
#define LV_COLOR_16_SWAP   1

/* Memory settings */
#define LV_MEM_CUSTOM       0
#define LV_MEM_SIZE         (64U * 1024U)
#define LV_MEM_ADR          0
#define LV_MEM_BUF_MAX_NUM  16

/* HAL settings */
#define LV_TICK_CUSTOM       1
#define LV_TICK_CUSTOM_INCLUDE  "Arduino.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR  (millis())

/* Font usage */
#define LV_FONT_MONTSERRAT_8    0
#define LV_FONT_MONTSERRAT_10   0
#define LV_FONT_MONTSERRAT_12   0
#define LV_FONT_MONTSERRAT_14   1
#define LV_FONT_MONTSERRAT_16   0
#define LV_FONT_MONTSERRAT_18   0
#define LV_FONT_MONTSERRAT_20   0
#define LV_FONT_MONTSERRAT_22   0
#define LV_FONT_MONTSERRAT_24   0
#define LV_FONT_MONTSERRAT_26   0
#define LV_FONT_MONTSERRAT_28   0
#define LV_FONT_MONTSERRAT_30   0
#define LV_FONT_MONTSERRAT_32   0
#define LV_FONT_MONTSERRAT_34   0
#define LV_FONT_MONTSERRAT_36   0
#define LV_FONT_MONTSERRAT_38   0
#define LV_FONT_MONTSERRAT_40   0
#define LV_FONT_MONTSERRAT_42   0
#define LV_FONT_MONTSERRAT_44   0
#define LV_FONT_MONTSERRAT_46   0
#define LV_FONT_MONTSERRAT_48   0

#define LV_FONT_DEFAULT        &lv_font_montserrat_14
#define LV_FONT_FMT_TXT_LARGE  0

/* Widget usage - enable only what we need */
#define LV_USE_ARC      0
#define LV_USE_BAR      1
#define LV_USE_BTN      0
#define LV_USE_BTNMATRIX 0
#define LV_USE_CANVAS   0
#define LV_USE_CHECKBOX 0
#define LV_USE_DROPDOWN 0
#define LV_USE_IMG      0
#define LV_USE_LABEL    1
#define LV_USE_LINE     0
#define LV_USE_ROLLER   0
#define LV_USE_SLIDER   0
#define LV_USE_SWITCH   0
#define LV_USE_TEXTAREA 0
#define LV_USE_TABLE    0

/* Extra widgets */
#define LV_USE_CHART      0
#define LV_USE_LED        0
#define LV_USE_METER      0
#define LV_USE_MSGBOX     0
#define LV_USE_SPAN       0
#define LV_USE_SPINBOX    0
#define LV_USE_SPINNER    0
#define LV_USE_TABVIEW    0
#define LV_USE_TILEVIEW   0
#define LV_USE_WIN        0
#define LV_USE_LIST       0
#define LV_USE_COLORWHEEL 0
#define LV_USE_IMGBTN     0
#define LV_USE_KEYBOARD   0

/* Themes */
#define LV_USE_THEME_DEFAULT  1
#define LV_THEME_DEFAULT_DARK 1
#define LV_THEME_DEFAULT_GROW 0
#define LV_THEME_DEFAULT_TRANSITION_TIME 0

/* Animation */
#define LV_USE_ANIMATION  1

/* GPU */
#define LV_USE_GPU_STM32_DMA2D  0
#define LV_USE_GPU_NXP_PXP      0
#define LV_USE_GPU_NXP_VG_LITE  0
#define LV_USE_GPU_SDL          0

/* Subpixel rendering */
#define LV_USE_FONT_SUBPX      0

/* Log */
#define LV_USE_LOG        0

/* Assert */
#define LV_USE_ASSERT_NULL          0
#define LV_USE_ASSERT_MALLOC        0
#define LV_USE_ASSERT_STYLE         0
#define LV_USE_ASSERT_MEM_INTEGRITY 0
#define LV_USE_ASSERT_OBJ           0

/* Complex widgets */
#define LV_USE_FLEX  0
#define LV_USE_GRID  0

/* Snapshot */
#define LV_USE_SNAPSHOT  0

/* Language support */
#define LV_USE_LANG  0

/* Sysmon */
#define LV_USE_SYSMON  0

/* OS */
#define LV_USE_OS_NONE  1

/* Draw */
#define LV_DRAW_SW_COMPLEX  0

#endif /* LV_CONF_H */
