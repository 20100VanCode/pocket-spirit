# Creating Custom Creatures

## Overview

Every creature in PocketSpirit is a subclass of `CreatureBase`. You define its personality through:

1. **Visual identity** — Colors and custom body elements
2. **Evolution rules** — How hard it is to evolve
3. **Animations** — Sequences of expressions for different moods
4. **Stat modifiers** — How much each interaction affects it

## Step-by-Step Guide

### 1. Create the Header File

Create `community_creatures/YourName/YourCreature.h`:

```cpp
#pragma once
#include "creatures/CreatureBase.h"

namespace PocketSpirit {

class YourCreature : public CreatureBase {
public:
    const char* getName() const override { return "YourCreature"; }
    const char* getDescription() const override { return "Description here"; }

    lv_color_t getPrimaryColor() const override { return lv_color_make(0xFF, 0x00, 0x00); }
    lv_color_t getSecondaryColor() const override { return lv_color_make(0xAA, 0x00, 0x00); }
    lv_color_t getEyeColor() const override { return lv_color_make(0x00, 0x00, 0x00); }

    // ... implement remaining methods
};

} // namespace PocketSpirit
```

### 2. Define Evolution Thresholds

Make your creature easier or harder to evolve:

```cpp
EvolutionThresholds getEvolutionThresholds(EvolutionStage stage) const override {
    switch (stage) {
        case EvolutionStage::EGG:
            // Easy to hatch - just interact a few times
            return { .minAgeSeconds = 30, .minInteractions = 3,
                     .minEnergy = 20.0f, .minHappiness = 20.0f, .minAffection = 5.0f };
        case EvolutionStage::BABY:
            // Harder to reach adult - needs dedication
            return { .minAgeSeconds = 600, .minInteractions = 50,
                     .minEnergy = 70.0f, .minHappiness = 70.0f, .minAffection = 60.0f };
        default:
            // No further evolution
            return { .minAgeSeconds = UINT32_MAX, .minInteractions = UINT32_MAX,
                     .minEnergy = 999.0f, .minHappiness = 999.0f, .minAffection = 999.0f };
    }
}
```

### 3. Design Animations

Animations are arrays of `{Expression, durationMs}`:

```cpp
const AnimationFrame* getIdleAnimation(uint8_t& count) const override {
    static const AnimationFrame frames[] = {
        { Expression::IDLE,    3000 },  // Neutral for 3s
        { Expression::BLINK,   150 },  // Quick blink
        { Expression::IDLE,    4000 },  // Back to neutral
        { Expression::HAPPY,   300 },  // Brief smile
        { Expression::IDLE,    2000 },  // Neutral again
    };
    count = sizeof(frames) / sizeof(frames[0]);
    return frames;
}
```

**Available expressions**: `IDLE`, `HAPPY`, `SAD`, `SLEEPY`, `SURPRISED`, `ANGRY`, `LOVE`, `EATING`, `BLINK`, `SLEEPING`, `EVOLVING`

### 4. Add Custom Body Elements

Use LVGL objects to add horns, tails, accessories:

```cpp
void drawBody(lv_obj_t* parent) override {
    // Add a small crown
    lv_obj_t* crown = lv_obj_create(parent);
    lv_obj_set_size(crown, 30, 15);
    lv_obj_align(crown, LV_ALIGN_TOP_MID, 0, 50);
    lv_obj_set_style_bg_color(crown, lv_color_make(0xFF, 0xD7, 0x00), 0);
    lv_obj_set_style_bg_opa(crown, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(crown, 0, 0);
    lv_obj_set_style_radius(crown, 2, 0);
    lv_obj_clear_flag(crown, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
}
```

### 5. Tune Stat Modifiers

Override the stat gain/cost values to shape personality:

```cpp
// A lazy creature that gains lots of energy from food
float getFeedEnergyGain() const override { return 50.0f; }

// A playful creature that loves games
float getPlayHappinessGain() const override { return 25.0f; }
float getPlayEnergyCost() const override { return 5.0f; }

// A shy creature that slowly warms up
float getPetAffectionGain() const override { return 3.0f; }
```

### 6. Register and Use

```cpp
#include "YourCreature.h"

YourCreature myCreature;
PocketSpirit app;

void setup() {
    app.registerCreature(&myCreature);
    app.begin("YourCreature");
}

void loop() {
    app.update();
    delay(5);
}
```

## Design Tips

- **Color contrast**: Ensure eye color contrasts with primary color for readability
- **Animation rhythm**: Mix long idle pauses with quick expression changes for natural feel
- **Personality through stats**: High energy cost = active, low affection gain = shy
- **Evolution pacing**: Short EGG→BABY keeps users engaged; long BABY→ADULT rewards dedication
- **Body elements**: Keep them simple — LVGL objects add memory overhead on ESP32
