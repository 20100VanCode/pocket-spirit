# 🎓 Pocket Spirit — Complete Beginner's Tutorial (Part 1: Setup & First Flash)

**No experience needed.** Step by step, from buying parts to your first living creature.

---

## Table of Contents

1. [What is Pocket Spirit?](#1-what-is-pocket-spirit)
2. [What You'll Need to Buy](#2-what-youll-need-to-buy)
3. [Setting Up Your Computer](#3-setting-up-your-computer)
4. [Downloading Pocket Spirit](#4-downloading-pocket-spirit)
5. [Connecting Everything Together](#5-connecting-everything-together)
6. [Flashing Your First Creature](#6-flashing-your-first-creature)
7. [Playing With Your Creature](#7-playing-with-your-creature)

---

## 1. What is Pocket Spirit?

A tiny digital pet — like a Tamagotchi — that lives on a small round screen. It has eyes that blink, a mouth that smiles or frowns, and feelings that change over time. You can pet it, feed it, and play with it by touching the screen.

**Pocket Spirit** is the free, open-source software that makes this possible. It runs on a small, inexpensive computer chip called an ESP32.

### What your creature does

- 🔋 **Energy** — Goes down over time. Feed to refill.
- 😊 **Happiness** — Goes down if ignored. Pet and play to raise.
- 💜 **Affection** — Goes up when you interact. Shows how much it loves you.
- 🥚→🐣→🐤 **Evolution** — Egg → Baby → Adult, but only if you take good care of it!
- 😴 **Sleeps** — Dims and sleeps to save battery. Touch to wake.
- 💾 **Remembers** — Survives reboots and deep sleep.

---

## 2. What You'll Need to Buy

Total cost: approximately **$15–25 USD**.

### The Screen (required)

**Waveshare 1.28-inch Round LCD Display Module (GC9A01)**

- Small circular screen, about watch-face size (32mm diameter)
- 240×240 pixels, has touch layer

Where to buy:
- [Waveshare official](https://www.waveshare.com/product/1.28inch-lcd-module.htm)
- AliExpress / Amazon — search "Waveshare 1.28 round display GC9A01"

> ⚠️ Make sure it says "GC9A01" or "1.28 inch". The round shape is the giveaway.

### The Computer Chip (required)

**ESP32-S3 development board** — small circuit board (~5cm × 2.5cm) with USB connector.

Recommended: ESP32-S3-DevKitC-1, or Waveshare ESP32-S3-PICO. Any board with "ESP32-S3" and ≥8MB flash works.

Where to buy: AliExpress / Amazon / Adafruit — search "ESP32-S3 devkit"

### USB Cable (required)

USB-C or Micro-USB (match your board's port). **Must be a data cable**, not just charging.

### Jumper Wires (required)

7 female-to-female jumper wires to connect display to ESP32.

### Shopping List

| Item | Approx. Price |
|------|---------------|
| Waveshare 1.28" Round LCD | $8–12 |
| ESP32-S3 Dev Board | $5–10 |
| USB Data Cable | $3–5 |
| Jumper Wires (F-F, 7 minimum) | $2–3 |
| **Total** | **~$18–30** |

---

## 3. Setting Up Your Computer

### Step 3.1 — Install Python

1. Go to [python.org/downloads](https://www.python.org/downloads/)
2. Download and run the installer
3. ⚠️ **Windows users:** Check the box **"Add Python to PATH"** before clicking Install
4. Verify: open Terminal/Command Prompt, type `python --version` — should show version number

### Step 3.2 — Install VS Code

1. Go to [code.visualstudio.com](https://code.visualstudio.com/)
2. Download and install with default settings

### Step 3.3 — Install PlatformIO

1. Open VS Code
2. Click the **Extensions** icon (four squares) in left sidebar
3. Search: `PlatformIO IDE`
4. Click **Install** (by "PlatformIO")
5. Wait 2–5 minutes. You'll see a 👽 alien icon in the sidebar when done
6. Reload VS Code if prompted

### Step 3.4 — Install Git

**Mac:** Open Terminal, type `git --version`. If it asks to install tools, click Install.

**Windows:** Download from [git-scm.com/download/win](https://git-scm.com/download/win), run with defaults.

---

## 4. Downloading Pocket Spirit

Open Terminal (Mac) or Command Prompt (Windows):

```bash
# Go to your Desktop
cd ~/Desktop          # Mac
cd %USERPROFILE%\Desktop   # Windows

# Download the code
git clone https://github.com/20100VanCode/pocket-spirit.git
```

Open in VS Code: **File → Open Folder → select `pocket-spirit`**

---

## 5. Connecting Everything Together

### Wire the Display to the ESP32

Take 7 jumper wires and connect:

| Display Pin | → | ESP32-S3 Pin | Suggested Wire Color |
|-------------|---|-------------|---------------------|
| VCC | → | 3V3 (3.3V) | Red |
| GND | → | GND | Black |
| SDA (MOSI) | → | GPIO 11 | Blue |
| SCL (SCLK) | → | GPIO 10 | Yellow |
| CS | → | GPIO 9 | Green |
| DC | → | GPIO 8 | Orange |
| RST | → | GPIO 12 | White |
| BL | → | GPIO 2 | Purple |

> ⚠️ **VCC must go to 3.3V, NOT 5V.** The display can be damaged by 5 volts.

### Connect to Computer

1. Plug USB cable into ESP32, other end into computer
2. Board should light up (usually a red LED)
3. In VS Code: click 👽 PlatformIO → **Devices** — you should see your board listed

If not visible: try a different USB cable (must be data cable) or different USB port.

---

## 6. Flashing Your First Creature

### Step 6.1 — Open the Example

In VS Code file explorer: `examples → creature_lumy → platformio.ini`

> If your board isn't "esp32-s3-devkitc-1", change the `board =` line in platformio.ini. See [board list](https://docs.platformio.org/en/latest/platforms/espressif32.html).

### Step 6.2 — Build

Click the **✓ checkmark** in VS Code's bottom status bar.

First build takes 3–10 minutes (downloads libraries). Success = `=== [SUCCESS] ===`

### Step 6.3 — Flash

Click the **→ right arrow** in bottom status bar.

Takes ~30–60 seconds. Success = `=== [SUCCESS] ===`

### Step 6.4 — See Your Creature!

The display lights up with **Lumy** — a golden creature with blinking eyes. Three bars at the bottom show Energy, Happiness, Affection.

**🎉 Your creature is alive!**

---

## 7. Playing With Your Creature

### Touch Zones

```
         ┌─────────┐
         │   TOP    │  ← Touch to PLAY
    ┌────┼─────────┼────┐
    │LEFT│ CENTER  │RIGHT│  ← Touch to PET
    └────┼─────────┼────┘
         │ BOTTOM  │  ← Touch to FEED
         └─────────┘
```

### What Each Action Does

| Action | How | Energy | Happiness | Affection |
|--------|-----|--------|-----------|-----------|
| **Pet** | Touch center/left/right | — | +5 | +8 |
| **Feed** | Touch bottom | +25 | +3 | — |
| **Play** | Touch top | −10 | +15 | — |

> Numbers vary by creature. Lumy loves pets (+10 affection), Dragon loves play (+20 happiness), Ghost is shy but craves affection (+12/pet).

### Stats Decay

| Stat | Time from 100 → 0 |
|------|-------------------|
| Energy | ~33 min |
| Happiness | ~55 min |
| Affection | ~166 min |

### Evolution (Lumy example)

| Stage | Requirements |
|-------|-------------|
| 🥚 Egg | Starting state |
| 🐣 Baby | Age ≥ 1 min + 5 interactions + decent stats |
| 🐤 Adult | Age ≥ 5 min + 25 interactions + good stats |

### Sleep

| Inactive | What Happens |
|----------|-------------|
| 15 sec | Screen dims to 30% |
| 30 sec | Screen goes dark |
| 60 sec | Deep sleep (almost no power) |

Touch to wake instantly. Creature remembers everything!

### Serial Monitor

Click the **🔌 plug icon** in VS Code bottom bar to see messages like:
```
Lumy is alive!
Lumy evolved to: Baby
Lumy needs attention!
```

---

**Continue to [Part 2: Custom Creatures & Beyond](TUTORIAL_PART2.md) →**
