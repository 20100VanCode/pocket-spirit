# Getting Started

## Hardware Requirements

- **MCU**: ESP32-S3 (recommended) or any ESP32 variant
- **Display**: Waveshare 1.28" Round LCD (GC9A01, 240×240)
- **Touch**: Built-in capacitive touch on Waveshare display
- **IMU** (optional): MPU6886 (built-in on some Waveshare boards)

## Wiring (Waveshare 1.28" Round Display)

| Display Pin | ESP32-S3 Pin | Function |
|-------------|-------------|----------|
| VCC | 3.3V | Power |
| GND | GND | Ground |
| SDA (MOSI) | GPIO 11 | SPI Data |
| SCL (SCLK) | GPIO 10 | SPI Clock |
| CS | GPIO 9 | Chip Select |
| DC | GPIO 8 | Data/Command |
| RST | GPIO 12 | Reset |
| BL | GPIO 2 | Backlight (PWM) |

## Quick Start

### 1. Install PlatformIO

```bash
pip install platformio
```

### 2. Clone the Repository

```bash
git clone https://github.com/20100VanCode/pocket-spirit.git
cd pocket-spirit
```

### 3. Build an Example

```bash
cd examples/creature_lumy
pio run
```

### 4. Flash to ESP32

```bash
pio run --target upload
```

### 5. Monitor Serial Output

```bash
pio device monitor
```

## Using in Your Own Project

### PlatformIO

Add to your `platformio.ini`:

```ini
lib_deps =
    https://github.com/20100VanCode/pocket-spirit.git

build_flags =
    -DGC9A01_DRIVER=1
    -DTFT_WIDTH=240
    -DTFT_HEIGHT=240
    -DTFT_MOSI=11
    -DTFT_SCLK=10
    -DTFT_CS=9
    -DTFT_DC=8
    -DTFT_RST=12
    -DTFT_BL=2
    -DSPI_FREQUENCY=40000000
    -DLV_CONF_PATH=../../lv_conf.h
```

### Minimal Code

```cpp
#include <PocketSpirit.h>

using namespace PocketSpirit;

PocketSpirit app;

void setup() {
    app.begin("Lumy");  // or "Dragon" or "Ghost"
}

void loop() {
    app.update();
    delay(5);
}
```

## Creating a Custom Creature

1. Create a new `.h` file that subclasses `CreatureBase`
2. Implement all pure virtual methods
3. Register and use it:

```cpp
#include "MyCreature.h"

MyCreature myCreature;
PocketSpirit app;

void setup() {
    app.registerCreature(&myCreature);
    app.begin("MyCreature");
}

void loop() {
    app.update();
    delay(5);
}
```

See `community_creatures/_template/MyCreature.h` for a complete template.

## Configuration

### Adjusting Decay Rates

Stats decay over time. Adjust in your creature or after `begin()`:

```cpp
// Inside your CreatureBase subclass - override stat modifiers
float getFeedEnergyGain() const override { return 40.0f; }  // More energy per feed
```

### Power Management

```cpp
app.setInactivityTimeout(60000);  // 60s before sleep
```

### Auto-Save

```cpp
app.setAutoSaveInterval(15000);  // Save every 15s
```

### Stats Display

```cpp
app.showStats(false);  // Hide the stats bars
```

## Troubleshooting

| Issue | Solution |
|-------|----------|
| Display stays black | Check wiring, verify `GC9A01_DRIVER` is defined |
| Touch not responding | Verify touch IC is compatible with TFT_eSPI |
| LVGL crash (reset) | Increase `LV_MEM_SIZE` in `lv_conf.h` |
| Deep sleep immediate | Check `setInactivityTimeout()` value |
| Stats reset on reboot | NVS may be corrupted; flash erase + re-flash |
