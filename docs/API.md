# API Reference

## PocketSpirit (Main Class)

### Lifecycle

| Method | Description |
|--------|-------------|
| `bool begin(const char* creatureName = "Lumy")` | Initialize everything. Loads saved state or creates new creature. Returns `true` on success. |
| `void update()` | Main loop call. Updates stats, input, animation, display, power, auto-save. Call in `loop()`. |

### Interactions

| Method | Description |
|--------|-------------|
| `void pet()` | Pet the creature. Increases affection + happiness. |
| `void feed()` | Feed the creature. Increases energy + happiness. |
| `void play()` | Play with the creature. Increases happiness, costs energy. |

### Queries

| Method | Returns |
|--------|---------|
| `const CreatureState& getState()` | Full state struct |
| `float getEnergy()` | 0–100 |
| `float getHappiness()` | 0–100 |
| `float getAffection()` | 0–100 |
| `EvolutionStage getEvolutionStage()` | `EGG`, `BABY`, or `ADULT` |
| `Expression getCurrentExpression()` | Current facial expression |

### Configuration

| Method | Description |
|--------|-------------|
| `void setInactivityTimeout(uint32_t ms)` | Time before dimming/sleep |
| `void registerCreature(CreatureBase* creature)` | Register a custom creature |
| `void setActiveCreature(const char* name)` | Switch active creature at runtime |
| `void showStats(bool show)` | Show/hide stats overlay |
| `void setAutoSaveInterval(uint32_t ms)` | Auto-save frequency (default 30s) |

### Callbacks

| Method | Signature |
|--------|-----------|
| `void onEvolve(...)` | `std::function<void(EvolutionStage)>` |
| `void onStatsLow(...)` | `std::function<void()>` |
| `void onInteraction(...)` | `std::function<void(const char* type)>` |

---

## CreatureBase (Abstract Interface)

Override all virtual methods to create a custom creature.

### Identity

```cpp
virtual const char* getName() const = 0;
virtual const char* getDescription() const = 0;
```

### Appearance

```cpp
virtual lv_color_t getPrimaryColor() const = 0;   // Body + eyelid color
virtual lv_color_t getSecondaryColor() const = 0;  // Accent color
virtual lv_color_t getEyeColor() const = 0;        // Pupil + mouth color
```

### Evolution

```cpp
virtual EvolutionThresholds getEvolutionThresholds(EvolutionStage stage) const = 0;
```

`EvolutionThresholds` fields:
- `minAgeSeconds` — Minimum age in seconds
- `minInteractions` — Minimum total interactions
- `minEnergy` — Minimum energy level
- `minHappiness` — Minimum happiness level
- `minAffection` — Minimum affection level

### Drawing

```cpp
virtual void drawBody(lv_obj_t* parent) = 0;
```

Called once at init. Add LVGL objects to `parent` for custom body features (horns, tails, accessories, etc.).

### Interaction Hooks

```cpp
virtual void onPet() = 0;
virtual void onFeed() = 0;
virtual void onPlay() = 0;
virtual void onWake() = 0;
virtual void onSleep() = 0;
```

### Animations

```cpp
virtual const AnimationFrame* getIdleAnimation(uint8_t& count) const = 0;
virtual const AnimationFrame* getHappyAnimation(uint8_t& count) const = 0;
virtual const AnimationFrame* getSadAnimation(uint8_t& count) const = 0;
virtual const AnimationFrame* getEatingAnimation(uint8_t& count) const = 0;
virtual const AnimationFrame* getEvolutionAnimation(uint8_t& count) const = 0;
```

`AnimationFrame` struct:
```cpp
struct AnimationFrame {
    Expression expression;   // Expression to show
    uint16_t durationMs;     // Duration in milliseconds
};
```

### Stat Modifiers (Optional Overrides)

```cpp
virtual float getPetAffectionGain()   const { return 8.0f; }
virtual float getPetHappinessGain()   const { return 5.0f; }
virtual float getFeedEnergyGain()     const { return 25.0f; }
virtual float getFeedHappinessGain()  const { return 3.0f; }
virtual float getPlayHappinessGain()  const { return 15.0f; }
virtual float getPlayEnergyCost()    const { return 10.0f; }
```

---

## Enums

### Expression

| Value | Description |
|-------|-------------|
| `IDLE` | Default neutral face |
| `HAPPY` | Squinted happy eyes |
| `SAD` | Droopy eyes, frown |
| `SLEEPY` | Half-closed eyelids |
| `SURPRISED` | Wide eyes, small pupils, O-mouth |
| `ANGRY` | Angled eyelids, flat mouth |
| `LOVE` | Dilated pupils, smile |
| `EATING` | Small happy eyes, open mouth |
| `BLINK` | Eyelid animation trigger |
| `SLEEPING` | Fully closed eyes |
| `EVOLVING` | Glowing yellow eyes, surprised mouth |

### EvolutionStage

| Value | Description |
|-------|-------------|
| `EGG` | Initial stage |
| `BABY` | First evolution |
| `ADULT` | Final stage |

### TouchZone

| Value | Description |
|-------|-------------|
| `CENTER` | Center of screen |
| `TOP` | Upper region |
| `BOTTOM` | Lower region |
| `LEFT` | Left region |
| `RIGHT` | Right region |

### MotionEvent

| Value | Description |
|-------|-------------|
| `SHAKE` | Device shaken |
| `TILT_LEFT/RIGHT/UP/DOWN` | Tilt detected |
| `TAP` | Single tap on device |

---

## Touch Zone Mapping

| Zone | Action |
|------|--------|
| Center | Pet |
| Top | Play |
| Bottom | Feed |
| Left/Right | Pet |
