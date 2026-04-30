# 🟣 Pocket Spirit

A lightweight ESP32 library for creating digital companions on Waveshare round displays.

**Create. Customize. Share.**

---

## ✨ Features

- 🐣 **3-stage evolution** — Egg → Baby → Adult with configurable thresholds
- 📊 **Stat system** — Energy, Happiness, Affection with natural decay
- 👀 **Expressive eyes** — 11 expressions with smooth blink animations
- 👆 **Touch interaction** — Zone-based input (pet, feed, play)
- 🏃 **Motion support** — Shake to play, tilt for eye tracking (optional IMU)
- 😴 **Deep sleep** — Smart power management with auto-wake
- 💾 **Persistent state** — NVS storage with CRC integrity, survives reboots
- 🎨 **Creature system** — Easy to create and share custom creatures
- 🖥️ **LVGL powered** — Fluid animations on the 1.28" round display

## 📦 Built-in Creatures

| Creature | Color | Personality |
|----------|-------|-------------|
| **Lumy** | 🟡 Gold | Warm, affectionate, loves being pet |
| **Dragon** | 🟢 Green | Fierce, energetic, loves playing |
| **Ghost** | 🟣 Purple | Shy, gentle, craves affection |

## 🚀 Quick Start

```cpp
#include <PocketSpirit.h>

using namespace PocketSpirit;

PocketSpirit app;

void setup() {
    app.begin("Lumy");  // Choose your creature
}

void loop() {
    app.update();
    delay(5);
}
```

That's it. Your creature is alive on the display! 🎉

## 🎮 Interactions

| Touch Zone | Action |
|-----------|--------|
| Center | Pet (↑ affection, ↑ happiness) |
| Top | Play (↑ happiness, ↓ energy) |
| Bottom | Feed (↑ energy, ↑ happiness) |
| Shake device | Play |

## 📁 Project Structure

```
pocket-spirit/
├── src/
│   ├── PocketSpirit.h/cpp          # Main facade
│   ├── core/
│   │   ├── CreatureState.h         # State data structures
│   │   ├── Stats.h/cpp             # Stat decay & modification
│   │   └── Evolution.h/cpp         # 3-stage evolution system
│   ├── display/
│   │   ├── DisplayManager.h/cpp    # LVGL + TFT driver
│   │   ├── EyeRenderer.h/cpp       # Eyes, pupils, mouth, expressions
│   │   └── AnimationEngine.h/cpp   # Frame-based animation sequencer
│   ├── input/
│   │   ├── TouchHandler.h/cpp      # Touch with zone detection
│   │   ├── MotionHandler.h/cpp     # IMU shake/tilt detection
│   │   └── InputManager.h/cpp      # Unified input facade
│   ├── power/
│   │   └── PowerManager.h/cpp      # Deep sleep & dimming
│   ├── persistence/
│   │   └── StateStorage.h/cpp      # NVS save/load with CRC
│   └── creatures/
│       ├── CreatureBase.h          # Abstract creature interface
│       ├── CreatureRegistry.h/cpp  # Creature lookup registry
│       ├── Lumy.h/cpp              # Built-in: Light spirit
│       ├── Dragon.h/cpp            # Built-in: Little dragon
│       └── Ghost.h/cpp             # Built-in: Shy phantom
├── examples/
│   ├── creature_lumy/              # Lumy example project
│   ├── creature_dragon/            # Dragon example project
│   └── creature_ghost/             # Ghost example project
├── community_creatures/
│   ├── README.md                   # Contribution guide
│   └── _template/MyCreature.h      # Starter template
├── docs/
│   ├── ARCHITECTURE.md             # System design
│   ├── API.md                      # Full API reference
│   ├── GETTING_STARTED.md          # Setup guide
│   └── CREATING_CREATURES.md       # Creature creation guide
├── tools/
│   └── sprite_converter.py         # PNG → RGB565 C array
├── lv_conf.h                       # Minimal LVGL config
├── library.json                    # PlatformIO manifest
├── library.properties              # Arduino IDE manifest
└── LICENSE                         # MIT
```

## 🔧 Hardware

- **MCU**: ESP32-S3 (recommended) or any ESP32
- **Display**: [Waveshare 1.28" Round LCD](https://www.waveshare.com/product/1.28inch-lcd-module.htm) (GC9A01, 240×240)
- **IMU** (optional): MPU6886 (on some Waveshare boards)

### Wiring

| Pin | ESP32-S3 |
|-----|----------|
| MOSI | GPIO 11 |
| SCLK | GPIO 10 |
| CS | GPIO 9 |
| DC | GPIO 8 |
| RST | GPIO 12 |
| BL | GPIO 2 |

## 🎨 Create Your Own Creature

```cpp
#include "creatures/CreatureBase.h"

namespace PocketSpirit {

class MyCreature : public CreatureBase {
public:
    const char* getName() const override { return "MyCreature"; }
    const char* getDescription() const override { return "My custom pet"; }
    lv_color_t getPrimaryColor() const override { return lv_color_make(0xFF, 0x00, 0x00); }
    lv_color_t getSecondaryColor() const override { return lv_color_make(0xCC, 0x00, 0x00); }
    lv_color_t getEyeColor() const override { return lv_color_make(0x00, 0x00, 0x00); }
    // ... implement remaining methods
};

}
```

See [Creating Creatures Guide](docs/CREATING_CREATURES.md) for full details.

## 📖 Documentation

- [Getting Started](docs/GETTING_STARTED.md) — Setup, wiring, first flash
- [Architecture](docs/ARCHITECTURE.md) — System design, data flow, memory budget
- [API Reference](docs/API.md) — Complete method & enum documentation
- [Creating Creatures](docs/CREATING_CREATURES.md) — Custom creature guide

## 🤝 Contributing

1. Fork the repo
2. Create your creature in `community_creatures/`
3. Submit a PR!

Please keep creatures family-friendly and test on hardware before submitting.

## 📜 License

MIT — See [LICENSE](LICENSE)
