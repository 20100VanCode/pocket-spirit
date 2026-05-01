# 🎓 Pocket Spirit — Complete Beginner's Tutorial (Part 2: Custom Creatures & Beyond)

**How things work, creating your own creature, sharing it, and fixing problems.**

---

## 8. Understanding How It Works

You don't need this section to use Pocket Spirit, but it helps for customizing later.

### The Big Picture

```
Your Touch → InputManager → PocketSpirit decides what to do
                                    ↓
                            Pet? Feed? Play?
                                    ↓
                    ┌───────────────┼───────────────┐
                    ↓               ↓               ↓
              Stats change    Animation plays    State saved
              (energy, etc.)  (happy face)      (to memory)
                    ↓
              EyeRenderer updates the face on screen
```

### The Code Structure

| Folder | Job | Analogy |
|--------|-----|---------|
| `core/` | Stats, evolution, state | The creature's "brain" |
| `display/` | Screen, eyes, animations | The creature's "face" |
| `input/` | Touch, motion | The creature's "ears" |
| `power/` | Sleep, wake, dimming | The creature's "sleep schedule" |
| `persistence/` | Save/load state | The creature's "diary" |
| `creatures/` | Different creature types | The creature's "species" |

### The Main File (super simple!)

```cpp
#include <PocketSpirit.h>     // Load the library
using namespace PocketSpirit;

PocketSpirit app;              // Create our app

void setup() {                 // Runs once at startup
    app.begin("Lumy");         // Start with Lumy
}

void loop() {                  // Runs forever
    app.update();              // Update everything
    delay(5);                  // Tiny pause
}
```

That's it. `begin()` starts everything, `update()` runs it 20 times per second.

---

## 9. Creating Your Own Custom Creature

### Step 9.1 — Plan Your Creature

Answer these before writing any code:

| Question | Example Answer |
|----------|---------------|
| Name? | "Flamey" |
| Looks like? | Red fire spirit with orange eyes |
| Personality? | Energetic, loves playing, gets hungry fast |
| Evolution difficulty? | Easy to hatch, hard to become adult |
| Special feature? | A little flame on top of its head |

### Step 9.2 — Create the File

1. In VS Code, find `community_creatures → _template`
2. Copy the `_template` folder
3. Paste it in `community_creatures/` and rename to your creature's name (e.g., `flamey`)
4. Rename `MyCreature.h` inside to `Flamey.h`

### Step 9.3 — Edit Identity

```cpp
// Change these two lines:
const char* getName() const override         { return "Flamey"; }
const char* getDescription() const override   { return "A fiery spirit that loves to play"; }
```

> The name must match what you pass to `app.begin("Flamey")`.

### Step 9.4 — Choose Colors

Colors use RGB (Red, Green, Blue), each 0–255:

```cpp
lv_color_t getPrimaryColor() const override   { return lv_color_make(0xFF, 0x33, 0x00); }  // Red-orange
lv_color_t getSecondaryColor() const override  { return lv_color_make(0xCC, 0x00, 0x00); }  // Dark red
lv_color_t getEyeColor() const override       { return lv_color_make(0xFF, 0x99, 0x00); }  // Orange
```

**Common colors:**

| Color | R | G | B |
|-------|---|---|---|
| White | 255 | 255 | 255 |
| Black | 0 | 0 | 0 |
| Red | 255 | 0 | 0 |
| Green | 0 | 255 | 0 |
| Blue | 0 | 0 | 255 |
| Yellow | 255 | 255 | 0 |
| Purple | 128 | 0 | 128 |
| Orange | 255 | 165 | 0 |
| Pink | 255 | 105 | 180 |
| Cyan | 0 | 255 | 255 |
| Brown | 139 | 69 | 19 |

Use [colorpicker.me](https://colorpicker.me) to find exact values.

> 💡 Eye color must contrast with primary color. Orange eyes on red body = great. Dark eyes on dark body = invisible.

### Step 9.5 — Set Evolution Requirements

Lower numbers = easier to evolve:

```cpp
EvolutionThresholds getEvolutionThresholds(EvolutionStage stage) const override {
    EvolutionThresholds th;
    switch (stage) {
        case EvolutionStage::EGG:
            // Hatch: 30 seconds old, 5 interactions
            th.minAgeSeconds = 30;
            th.minInteractions = 5;
            th.minEnergy = 20.0f;
            th.minHappiness = 20.0f;
            th.minAffection = 5.0f;
            return th;
        case EvolutionStage::BABY:
            // Adult: 10 minutes, 50 interactions, high stats
            th.minAgeSeconds = 600;
            th.minInteractions = 50;
            th.minEnergy = 70.0f;
            th.minHappiness = 70.0f;
            th.minAffection = 60.0f;
            return th;
        default:
            // No evolution after adult
            th.minAgeSeconds = UINT32_MAX;
            th.minInteractions = UINT32_MAX;
            th.minEnergy = 999.0f;
            th.minHappiness = 999.0f;
            th.minAffection = 999.0f;
            return th;
    }
}
```

| Threshold | Meaning | Example |
|-----------|---------|---------|
| `minAgeSeconds` | Minimum age in seconds | 30 = 30 seconds |
| `minInteractions` | Minimum total touches | 5 = five touches |
| `minEnergy` | Minimum energy % | 20.0 = 20% |
| `minHappiness` | Minimum happiness % | 20.0 = 20% |
| `minAffection` | Minimum affection % | 5.0 = 5% |

### Step 9.6 — Design Animations

Animations are sequences of expressions with timing — like a flipbook.

**Available expressions:**

| Expression | Face |
|-----------|------|
| `Expression::IDLE` | Normal, eyes open |
| `Expression::HAPPY` | Squinted happy eyes 😊 |
| `Expression::SAD` | Droopy eyes, frown 😢 |
| `Expression::SLEEPY` | Half-closed 😪 |
| `Expression::SURPRISED` | Wide eyes, O-mouth 😲 |
| `Expression::ANGRY` | Angled lids, flat mouth 😠 |
| `Expression::LOVE` | Dilated pupils, smile 😍 |
| `Expression::EATING` | Small happy eyes, open mouth 😋 |
| `Expression::BLINK` | Quick eyelid close |
| `Expression::SLEEPING` | Fully closed eyes 😴 |
| `Expression::EVOLVING` | Glowing yellow eyes (special) |

**Example — Flamey's idle animation:**

```cpp
const AnimationFrame* getIdleAnimation(uint8_t& count) const override {
    static const AnimationFrame frames[] = {
        { Expression::IDLE,    3000 },  // Normal for 3 seconds
        { Expression::BLINK,   150 },   // Quick blink
        { Expression::IDLE,    4000 },  // Normal for 4 seconds
        { Expression::HAPPY,   300 },   // Brief smile
        { Expression::IDLE,    2000 },  // Back to normal
    };
    count = sizeof(frames) / sizeof(frames[0]);  // Don't change this line
    return frames;
}
```

**How to read this:**
- Each line = one "frame" of the animation
- The number = how long to hold that expression (in milliseconds)
- 1000ms = 1 second
- The animation loops when it reaches the end

**You need 5 animation sets:**

1. **Idle** — What it does when nothing's happening (loops forever)
2. **Happy** — Plays when you pet it (plays once, then returns to idle)
3. **Sad** — Plays when stats are low (plays once)
4. **Eating** — Plays when you feed it (plays once)
5. **Evolution** — Plays when it evolves (plays once, dramatic!)

**Animation design tips:**
- Start and end idle animations with `Expression::IDLE`
- Include a `BLINK` every 3–5 seconds in idle
- Keep happy/sad animations short (500–1000ms per expression)
- Evolution should feel dramatic: surprise → glow → joy

### Step 9.7 — Add Custom Body Features

Draw extra things on your creature — horns, flames, tails, hats.

The screen is 240×240 pixels. The creature's face is roughly centered. You add colored shapes using LVGL objects.

**Understanding the coordinate system:**

```
(0,0) ─────────────────── (240,0)
  │                           │
  │      ┌───────────┐       │
  │      │  Creature  │       │
  │      │   Face     │       │
  │      └───────────┘       │
  │                           │
(0,240) ───────────────── (240,240)
```

- X goes left to right (0 = left edge, 240 = right edge)
- Y goes top to bottom (0 = top, 240 = bottom)
- The eyes are around Y=78, the body is centered around (120, 75)

**Example — A flame on top of the head:**

```cpp
void drawBody(lv_obj_t* parent) override {
    // Create a flame shape
    lv_obj_t* flame = lv_obj_create(parent);
    lv_obj_set_size(flame, 20, 30);                    // 20px wide, 30px tall
    lv_obj_set_pos(flame, 110, 45);                    // Position: center-top area
    lv_obj_set_style_bg_color(flame, lv_color_make(0xFF, 0x66, 0x00), 0);  // Orange
    lv_obj_set_style_bg_opa(flame, LV_OPA_COVER, 0);  // Fully visible
    lv_obj_set_style_border_width(flame, 0, 0);        // No border
    lv_obj_set_style_radius(flame, 10, 0);             // Rounded corners
    lv_obj_clear_flag(flame, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
}
```

**What each line does:**

| Line | Meaning |
|------|---------|
| `lv_obj_create(parent)` | Create a new shape on the screen |
| `lv_obj_set_size(obj, W, H)` | Set width and height in pixels |
| `lv_obj_set_pos(obj, X, Y)` | Set position (top-left corner) |
| `lv_obj_set_style_bg_color(obj, color, 0)` | Set fill color |
| `lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0)` | Make it fully visible |
| `lv_obj_set_style_border_width(obj, 0, 0)` | Remove the border |
| `lv_obj_set_style_radius(obj, R, 0)` | Set corner roundness (0 = square, 255 = circle) |
| `lv_obj_clear_flag(obj, ...)` | Make it non-interactive (always do this) |

**You can add multiple shapes!** Just repeat the pattern:

```cpp
void drawBody(lv_obj_t* parent) override {
    // Flame (orange, on top)
    lv_obj_t* flame = lv_obj_create(parent);
    lv_obj_set_size(flame, 20, 30);
    lv_obj_set_pos(flame, 110, 45);
    lv_obj_set_style_bg_color(flame, lv_color_make(0xFF, 0x66, 0x00), 0);
    lv_obj_set_style_bg_opa(flame, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(flame, 0, 0);
    lv_obj_set_style_radius(flame, 10, 0);
    lv_obj_clear_flag(flame, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);

    // Inner flame (yellow, smaller)
    lv_obj_t* inner = lv_obj_create(parent);
    lv_obj_set_size(inner, 10, 18);
    lv_obj_set_pos(inner, 115, 50);
    lv_obj_set_style_bg_color(inner, lv_color_make(0xFF, 0xFF, 0x00), 0);
    lv_obj_set_style_bg_opa(inner, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(inner, 0, 0);
    lv_obj_set_style_radius(inner, 5, 0);
    lv_obj_clear_flag(inner, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
}
```

### Step 9.8 — Tune Stat Modifiers

These numbers shape your creature's personality:

```cpp
// Override these in your creature class:

float getPetAffectionGain()   const override { return 10.0f; }  // How much affection per pet
float getPetHappinessGain()   const override { return 5.0f; }   // How much happiness per pet
float getFeedEnergyGain()     const override { return 30.0f; }  // How much energy per feed
float getFeedHappinessGain()  const override { return 3.0f; }   // How much happiness per feed
float getPlayHappinessGain()  const override { return 18.0f; }  // How much happiness per play
float getPlayEnergyCost()     const override { return 10.0f; }   // How much energy play costs
```

**Personality examples:**

| Personality | Pet Affection | Feed Energy | Play Happiness | Play Cost |
|-------------|--------------|-------------|----------------|-----------|
| **Cuddly** (Lumy) | 10 | 30 | 18 | 10 |
| **Fierce** (Dragon) | 6 | 35 | 20 | 15 |
| **Shy** (Ghost) | 12 | 20 | 12 | 10 |
| **Lazy** | 5 | 50 | 8 | 5 |
| **Hyper** | 3 | 15 | 30 | 20 |

### Step 9.9 — Interaction Hooks (Optional)

These are called when the creature is pet, fed, etc. You can add custom behavior:

```cpp
void onPet() override {
    // Could add a sound effect, LED flash, or serial message
}
void onFeed() override { }
void onPlay() override { }
void onWake() override { }
void onSleep() override { }
```

For now, leave them empty `{ }` — they're just hooks for future expansion.

### Step 9.10 — Register and Use Your Creature

Create a new example project or modify an existing one:

```cpp
#include <PocketSpirit.h>
#include "Flamey.h"          // Your creature's header

using namespace PocketSpirit;

Flamey flamey;               // Create an instance
PocketSpirit app;

void setup() {
    app.registerCreature(&flamey);   // Register it
    app.begin("Flamey");             // Start with Flamey
}

void loop() {
    app.update();
    delay(5);
}
```

Then build and flash just like in Part 1!

---

## 10. Sharing Your Creature With the World

### Step 10.1 — Test on Hardware

Before sharing, make sure:
- ✅ It compiles without errors
- ✅ It displays correctly on the round screen
- ✅ Touch interactions work (pet, feed, play)
- ✅ Animations play smoothly
- ✅ Evolution triggers at the right time
- ✅ Colors look good (contrast is readable)

### Step 10.2 — Share on GitHub

1. Create a GitHub account at [github.com](https://github.com) (free)
2. Fork the Pocket Spirit repository (click "Fork" on the repo page)
3. Add your creature file to `community_creatures/yourname-creaturename/`
4. Update `community_creatures/README.md` to add your creature to the table
5. Create a **Pull Request** — this is how you ask the project to include your work

### Step 10.3 — What to Include

- Your `.h` file with the creature class
- A brief comment at the top describing the creature
- Keep it family-friendly!

---

## 11. Troubleshooting — When Things Go Wrong

### Display Issues

| Problem | Solution |
|---------|----------|
| Screen stays black | Check VCC→3.3V, GND connections. Verify BL→GPIO2 wire. |
| Screen shows garbage/noise | Check all SPI wires (MOSI, SCLK, CS, DC, RST). Make sure they're on the correct GPIO pins. |
| Screen is dim | Check BL (backlight) wire. Try touching the screen to wake from dim mode. |
| Screen is upside down | Change rotation in code: `tft.setRotation(2);` in DisplayManager.cpp |
| Colors look wrong | Check `LV_COLOR_16_SWAP` is set to 1 in lv_conf.h |

### Build Issues

| Problem | Solution |
|---------|----------|
| "Could not find library" | Check internet connection. PlatformIO downloads libraries on first build. |
| "lv_conf.h not found" | Make sure you opened the entire `pocket-spirit` folder, not a subfolder. |
| Build takes forever | First build downloads everything. Subsequent builds are fast (~30s). |
| "undefined reference" | Make sure all .cpp files are in the `src/` folder structure. |
| Out of memory | Increase `LV_MEM_SIZE` in lv_conf.h (try 80K or 96K). |

### Upload Issues

| Problem | Solution |
|---------|----------|
| "No device found" | Check USB cable (must be data cable). Try different port. |
| "Failed to connect to ESP32" | Hold the BOOT button on the ESP32 while clicking Upload. Release after upload starts. |
| Upload starts but fails | Install CP210x or CH340 USB driver (search "ESP32 USB driver"). |

### Runtime Issues

| Problem | Solution |
|---------|----------|
| Creature resets randomly | Likely out of memory. Reduce LV_MEM_SIZE or simplify drawBody(). |
| Stats reset on reboot | NVS may be corrupted. Flash erase: `pio run --target erase` then re-flash. |
| Touch not responding | Verify touch IC is compatible with TFT_eSPI. Some displays need different touch drivers. |
| Goes to sleep immediately | Increase inactivity timeout: `app.setInactivityTimeout(60000);` |
| Expression stuck on one face | Check that `app.update()` is being called in `loop()`. |

### Getting Help

- **GitHub Issues:** [github.com/20100VanCode/pocket-spirit/issues](https://github.com/20100VanCode/pocket-spirit/issues)
- **Include:** Your board type, wiring, error message, and what you've already tried

---

## 12. Glossary — What Do All These Words Mean?

| Term | Meaning |
|------|---------|
| **ESP32** | A small, cheap computer chip that can run programs and connect to WiFi/Bluetooth |
| **GPIO** | General Purpose Input/Output — the numbered pins on the ESP32 that connect to things |
| **Flash** | (1) The permanent memory on the ESP32 where your program is stored. (2) The act of writing a program to that memory |
| **LVGL** | Light and Versatile Graphics Library — draws things on screens efficiently |
| **NVS** | Non-Volatile Storage — memory that survives power loss (like a tiny hard drive) |
| **PlatformIO** | A tool that builds code and uploads it to microcontrollers like the ESP32 |
| **RGB565** | A color format that uses 16 bits: 5 for red, 6 for green, 5 for blue |
| **SPI** | Serial Peripheral Interface — a fast way for chips to talk to each other |
| **TFT** | Thin Film Transistor — a type of LCD screen |
| **GC9A01** | The specific display controller chip on the Waveshare 1.28" round screen |
| **Deep sleep** | A very low power mode where almost everything is turned off |
| **Framework** | In PlatformIO, the programming style: "Arduino" means Arduino-style code |
| **Jumper wire** | A short wire with connectors on each end, used for prototyping |
| **Header file** | A `.h` file that declares what a piece of code does (like a table of contents) |
| **Namespace** | A way to group code names together so they don't conflict (`PocketSpirit::`) |
| **Override** | Replacing a parent class's method with your own version |
| **Facade** | A design pattern where one simple class hides many complex subsystems |
| **CRC** | Cyclic Redundancy Check — a way to verify data wasn't corrupted |
| **IMU** | Inertial Measurement Unit — a chip that detects motion/tilt |
| **PWM** | Pulse Width Modulation — a way to control brightness by rapidly turning on/off |

---

**🎉 You now know everything you need to create and share Pocket Spirit creatures!**

Start with the template, experiment with colors and animations, and have fun bringing your digital companion to life.
