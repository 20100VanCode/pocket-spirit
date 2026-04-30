# Architecture

## System Overview

```
┌─────────────────────────────────────────────────┐
│                  PocketSpirit                     │
│                  (Facade)                         │
├─────────┬──────────┬──────────┬────────┬────────┤
│  Core   │ Display  │  Input   │ Power  │Persist │
│ Stats   │ Eyes     │ Touch    │ Sleep  │ NVS    │
│ Evolve  │ Anim     │ Motion   │ Dim    │ CRC    │
├─────────┴──────────┴──────────┴────────┴────────┤
│              Creature System                      │
│  CreatureBase → Lumy / Dragon / Ghost / Custom   │
├──────────────────────────────────────────────────┤
│              Hardware Abstraction                 │
│  LVGL + TFT_eSPI | Touch | IMU | ESP32 NVS      │
└──────────────────────────────────────────────────┘
```

## Layer Architecture

### 1. Core Layer (`src/core/`)
- **CreatureState** — Plain data struct: stats, evolution stage, age, interaction counters
- **Stats** — Manages energy/happiness/affection with configurable decay rates
- **Evolution** — Threshold-based 3-stage evolution (Egg → Baby → Adult)

### 2. Display Layer (`src/display/`)
- **DisplayManager** — LVGL driver init, TFT flush callback, brightness control
- **EyeRenderer** — Dual-eye rendering with pupils, eyelids, mouth, expressions
- **AnimationEngine** — Frame-based animation sequencer with idle/happy/sad/eating/evolving sets

### 3. Input Layer (`src/input/`)
- **TouchHandler** — Touch input with zone detection (center/top/bottom/left/right)
- **MotionHandler** — IMU (MPU6886) with shake/tilt detection (optional, compile-time)
- **InputManager** — Unified input facade combining touch + motion

### 4. Power Layer (`src/power/`)
- **PowerManager** — State machine: Active → Dimming → Idle → Deep Sleep
- Wake sources: touch interrupt + timer (for stat decay during sleep)

### 5. Persistence Layer (`src/persistence/`)
- **StateStorage** — NVS-based blob storage with CRC16 integrity check
- Auto-save on configurable interval + pre-sleep save

### 6. Creature System (`src/creatures/`)
- **CreatureBase** — Abstract interface: identity, appearance, evolution thresholds, animations, stat modifiers
- **CreatureRegistry** — Singleton registry for creature lookup by name
- **Built-in**: Lumy (gold, affectionate), Dragon (green, fierce), Ghost (purple, shy)

## Data Flow

```
Touch/Motion → InputManager → PocketSpirit.handleTouch/handleMotion()
                                    │
                              ┌─────┴─────┐
                              │  pet/feed/ │
                              │   play()   │
                              └─────┬─────┘
                                    │
                    Stats.modify*() + Animation.play()
                                    │
                    ┌───────────────┼───────────────┐
                    │               │               │
              EyeRenderer    Evolution.check    StateStorage
              .setExpression()  → onEvolve()    .saveState()
```

## Memory Budget (ESP32-S3)

| Component | RAM | Flash |
|-----------|-----|-------|
| LVGL (64KB pool) | 64 KB | ~200 KB |
| Display buffer (240×20×2) | 9.6 KB | — |
| CreatureState | ~80 B | — |
| LVGL objects | ~2 KB | — |
| **Total estimated** | **~80 KB** | **~250 KB** |

ESP32-S3 has 512KB SRAM + 8MB PSRAM — plenty of headroom.

## Deep Sleep Strategy

1. **Dim** after 15s inactivity (backlight → 30%)
2. **Idle** after 30s (backlight → off)
3. **Deep sleep** after 60s (full shutdown, state saved to NVS)
4. **Wake on**: touch interrupt OR 60s timer (for stat decay)
5. On timer wake: decay stats, check evolution, re-enter sleep

## Extensibility

- **Custom creatures**: Subclass `CreatureBase`, register with `CreatureRegistry`
- **Custom animations**: Define `AnimationFrame[]` arrays, return from creature methods
- **Custom expressions**: Add to `Expression` enum, implement in `EyeRenderer::applyExpression()`
- **Community folder**: Drop `.h` files in `community_creatures/`
