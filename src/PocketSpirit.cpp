#include "PocketSpirit.h"
#include <Arduino.h>
#include <cstring>

namespace PocketSpirit {

PocketSpirit::PocketSpirit() {}

PocketSpirit::~PocketSpirit() {}

bool PocketSpirit::begin(const char* creatureName) {
    if (_initialized) return true;

    // 1. Init persistence
    if (!_storage.init()) return false;

    // 2. Register built-in creatures
    CreatureRegistry::instance().registerCreature(&_lumy);
    CreatureRegistry::instance().registerCreature(&_dragon);
    CreatureRegistry::instance().registerCreature(&_ghost);

    // 3. Init display
    if (!_display.init()) return false;

    // 4. Set up screen background
    lv_obj_t* screen = _display.getScreen();
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

    // 5. Init input
    if (!_input.init(_display.getWidth(), _display.getHeight())) return false;

    // 6. Init power
    if (!_power.init()) return false;

    // 7. Load saved state or create new
    if (_storage.hasState() && _storage.loadState(_state)) {
        _stats.setStats(_state.stats);
        _evolution.setStage(_state.stage);
        _creature = CreatureRegistry::instance().getCreature(_state.creatureType);
    }

    // 8. If no creature loaded, use requested one
    if (!_creature) {
        _creature = CreatureRegistry::instance().getCreature(creatureName);
        if (!_creature) _creature = &_lumy;

        strncpy(_state.creatureType, _creature->getName(), sizeof(_state.creatureType) - 1);
        _state.creatureType[sizeof(_state.creatureType) - 1] = '\0';
        _state.stage = EvolutionStage::EGG;
        _state.ageSeconds = 0;
        _state.totalInteractions = 0;
        _state.totalPets = 0;
        _state.totalFeeds = 0;
        _state.currentExpression = Expression::IDLE;
        _stats.setStats(_state.stats);
    }

    // 9. Apply creature evolution thresholds
    for (uint8_t i = 0; i < Evolution::STAGE_COUNT; i++) {
        EvolutionStage stage = static_cast<EvolutionStage>(i);
        _evolution.setThresholds(stage, _creature->getEvolutionThresholds(stage));
    }

    // 10. Apply creature theme and draw
    applyCreatureTheme();
    _creature->drawBody(_display.getScreen());

    // 11. Init eyes
    _eyes.init(_display.getScreen(), _creature->getPrimaryColor(), _creature->getEyeColor());

    // 12. Init animation engine
    _animation.init(&_eyes);
    uint8_t idleCount;
    const AnimationFrame* idleFrames = _creature->getIdleAnimation(idleCount);
    _animation.setIdleAnimation(idleFrames, idleCount);
    _animation.playIdle();

    // 13. Create stats overlay
    createStatsOverlay();

    // 14. Set up input callbacks
    _input.onTouchEvent([this](TouchZone z, const TouchPoint& p) { handleTouch(z, p); });
    _input.onMotionEvent([this](MotionEvent e) { handleMotion(e); });

    // 15. Set up power callbacks
    _power.onPreSleep([this]() { saveState(); });

    // 16. Configure wake sources
    PowerManager::configureWakeSources(255, 60); // No touch pin, wake every 60s

    _lastUpdateTime = millis();
    _lastAutoSave   = millis();
    _initialized    = true;

    return true;
}

void PocketSpirit::update() {
    if (!_initialized) return;

    unsigned long now = millis();
    float dt = (float)(now - _lastUpdateTime) / 1000.0f;
    _lastUpdateTime = now;

    // Cap delta time (e.g. after wake from sleep)
    if (dt > 5.0f) dt = 0.05f;

    // Update subsystems
    _stats.update(dt);
    _input.update();
    _animation.update(dt);
    _eyes.update(dt);
    _power.update(dt);

    // Update state from stats
    updateState(dt);

    // Determine expression based on stats
    determineExpression();

    // Update display
    _display.update();

    // Update stats overlay
    if (_showStats) updateStatsOverlay();

    // Check auto-save
    checkAutoSave();
}

void PocketSpirit::pet() {
    if (!_creature) return;

    _stats.modifyAffection(_creature->getPetAffectionGain());
    _stats.modifyHappiness(_creature->getPetHappinessGain());
    _state.totalPets++;
    _state.totalInteractions++;
    _power.refresh();

    _creature->onPet();

    uint8_t count;
    const AnimationFrame* frames = _creature->getHappyAnimation(count);
    _animation.play(frames, count, false);

    if (_interactionCb) _interactionCb("pet");
}

void PocketSpirit::feed() {
    if (!_creature) return;

    _stats.modifyEnergy(_creature->getFeedEnergyGain());
    _stats.modifyHappiness(_creature->getFeedHappinessGain());
    _state.totalFeeds++;
    _state.totalInteractions++;
    _power.refresh();

    _creature->onFeed();

    uint8_t count;
    const AnimationFrame* frames = _creature->getEatingAnimation(count);
    _animation.play(frames, count, false);

    if (_interactionCb) _interactionCb("feed");
}

void PocketSpirit::play() {
    if (!_creature) return;

    _stats.modifyHappiness(_creature->getPlayHappinessGain());
    _stats.modifyEnergy(-_creature->getPlayEnergyCost());
    _state.totalInteractions++;
    _power.refresh();

    _creature->onPlay();

    uint8_t count;
    const AnimationFrame* frames = _creature->getHappyAnimation(count);
    _animation.play(frames, count, false);

    if (_interactionCb) _interactionCb("play");
}

void PocketSpirit::setInactivityTimeout(uint32_t ms) {
    _power.setInactivityTimeout(ms);
}

void PocketSpirit::registerCreature(CreatureBase* creature) {
    CreatureRegistry::instance().registerCreature(creature);
}

void PocketSpirit::setActiveCreature(const char* name) {
    CreatureBase* c = CreatureRegistry::instance().getCreature(name);
    if (!c) return;

    _creature = c;
    strncpy(_state.creatureType, c->getName(), sizeof(_state.creatureType) - 1);
    _state.creatureType[sizeof(_state.creatureType) - 1] = '\0';

    // Reset evolution thresholds
    for (uint8_t i = 0; i < Evolution::STAGE_COUNT; i++) {
        EvolutionStage stage = static_cast<EvolutionStage>(i);
        _evolution.setThresholds(stage, c->getEvolutionThresholds(stage));
    }

    applyCreatureTheme();
    _eyes.setColors(c->getPrimaryColor(), c->getEyeColor());
    saveState();
}

void PocketSpirit::onEvolve(std::function<void(EvolutionStage)> callback) {
    _evolveCb = callback;
}

void PocketSpirit::onStatsLow(std::function<void()> callback) {
    _statsLowCb = callback;
}

void PocketSpirit::onInteraction(std::function<void(const char*)> callback) {
    _interactionCb = callback;
}

void PocketSpirit::showStats(bool show) {
    _showStats = show;
    if (_statsBar) {
        if (show) lv_obj_clear_flag(_statsBar, LV_OBJ_FLAG_HIDDEN);
        else      lv_obj_add_flag(_statsBar, LV_OBJ_FLAG_HIDDEN);
    }
}

void PocketSpirit::setAutoSaveInterval(uint32_t ms) {
    _autoSaveMs = ms;
}

// --- Private methods ---

void PocketSpirit::handleTouch(TouchZone zone, const TouchPoint& pt) {
    _power.refresh();

    // Pupil tracking toward touch point
    float nx = ((float)pt.x / (float)_display.getWidth()) * 2.0f - 1.0f;
    float ny = ((float)pt.y / (float)_display.getHeight()) * 2.0f - 1.0f;
    _eyes.trackPupil(nx, ny);

    switch (zone) {
        case TouchZone::CENTER: pet();  break;
        case TouchZone::TOP:    play(); break;
        case TouchZone::BOTTOM: feed(); break;
        case TouchZone::LEFT:
        case TouchZone::RIGHT:
            pet();
            break;
        default: break;
    }
}

void PocketSpirit::handleMotion(MotionEvent event) {
    _power.refresh();

    switch (event) {
        case MotionEvent::SHAKE:
            play();
            break;
        case MotionEvent::TILT_LEFT:
            _eyes.trackPupil(-0.8f, 0.0f);
            break;
        case MotionEvent::TILT_RIGHT:
            _eyes.trackPupil(0.8f, 0.0f);
            break;
        case MotionEvent::TILT_UP:
            _eyes.trackPupil(0.0f, -0.8f);
            break;
        case MotionEvent::TILT_DOWN:
            _eyes.trackPupil(0.0f, 0.8f);
            break;
        default: break;
    }
}

void PocketSpirit::updateState(float dt) {
    _state.stats = _stats.getStats();
    _state.ageSeconds += (uint32_t)dt;
    _state.lastUpdateMs = millis();

    // Check evolution
    if (_evolution.checkEvolution(_state)) {
        _state.stage = _evolution.getStage();

        uint8_t count;
        const AnimationFrame* frames = _creature->getEvolutionAnimation(count);
        _animation.play(frames, count, false);

        if (_evolveCb) _evolveCb(_evolution.getStage());
        saveState();
    }

    // Check low stats
    if (_stats.isCriticalEnergy() || _stats.isLowHappiness()) {
        if (_statsLowCb) _statsLowCb();
    }
}

void PocketSpirit::determineExpression() {
    if (_animation.isPlaying()) return; // Don't override active animation

    Expression desired = Expression::IDLE;

    if (_stats.isCriticalEnergy()) {
        desired = Expression::SLEEPING;
    } else if (_stats.isLowEnergy()) {
        desired = Expression::SLEEPY;
    } else if (_stats.isLowHappiness()) {
        desired = Expression::SAD;
    } else if (_stats.getAffection() > 80.0f && _stats.getHappiness() > 80.0f) {
        desired = Expression::LOVE;
    } else if (_stats.getHappiness() > 70.0f) {
        desired = Expression::HAPPY;
    }

    if (desired != _state.currentExpression) {
        _state.currentExpression = desired;

        if (desired == Expression::IDLE) {
            _animation.playIdle();
        } else {
            _eyes.setExpression(desired);
        }
    }
}

void PocketSpirit::saveState() {
    _state.stats = _stats.getStats();
    _state.stage = _evolution.getStage();
    _storage.saveState(_state);
}

void PocketSpirit::loadState() {
    if (_storage.loadState(_state)) {
        _stats.setStats(_state.stats);
        _evolution.setStage(_state.stage);
    }
}

void PocketSpirit::applyCreatureTheme() {
    if (!_creature) return;

    lv_obj_t* screen = _display.getScreen();
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
}

void PocketSpirit::createStatsOverlay() {
    lv_obj_t* screen = _display.getScreen();

    // Semi-transparent bar at bottom
    _statsBar = lv_obj_create(screen);
    lv_obj_set_size(_statsBar, 200, 35);
    lv_obj_align(_statsBar, LV_ALIGN_BOTTOM_MID, 0, -5);
    lv_obj_set_style_bg_color(_statsBar, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(_statsBar, LV_OPA_70, 0);
    lv_obj_set_style_border_width(_statsBar, 0, 0);
    lv_obj_set_style_radius(_statsBar, 8, 0);
    lv_obj_clear_flag(_statsBar, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);

    // Energy bar
    _energyBar = lv_bar_create(_statsBar);
    lv_obj_set_size(_energyBar, 55, 6);
    lv_obj_align(_energyBar, LV_ALIGN_TOP_LEFT, 8, 8);
    lv_bar_set_range(_energyBar, 0, 100);
    lv_bar_set_value(_energyBar, (int32_t)_stats.getEnergy(), LV_ANIM_OFF);
    lv_obj_set_style_bg_color(_energyBar, lv_color_make(0x4C, 0xAF, 0x50), LV_PART_INDICATOR);

    // Happiness bar
    _happinessBar = lv_bar_create(_statsBar);
    lv_obj_set_size(_happinessBar, 55, 6);
    lv_obj_align(_happinessBar, LV_ALIGN_TOP_MID, 0, 8);
    lv_bar_set_range(_happinessBar, 0, 100);
    lv_bar_set_value(_happinessBar, (int32_t)_stats.getHappiness(), LV_ANIM_OFF);
    lv_obj_set_style_bg_color(_happinessBar, lv_color_make(0xFF, 0xEB, 0x3B), LV_PART_INDICATOR);

    // Affection bar
    _affectionBar = lv_bar_create(_statsBar);
    lv_obj_set_size(_affectionBar, 55, 6);
    lv_obj_align(_affectionBar, LV_ALIGN_TOP_RIGHT, -8, 8);
    lv_bar_set_range(_affectionBar, 0, 100);
    lv_bar_set_value(_affectionBar, (int32_t)_stats.getAffection(), LV_ANIM_OFF);
    lv_obj_set_style_bg_color(_affectionBar, lv_color_make(0xE9, 0x1E, 0x63), LV_PART_INDICATOR);

    // Stage label
    _stageLabel = lv_label_create(_statsBar);
    lv_label_set_text(_stageLabel, evolutionStageToStr(_evolution.getStage()));
    lv_obj_align(_stageLabel, LV_ALIGN_BOTTOM_MID, 0, -2);
    lv_obj_set_style_text_color(_stageLabel, lv_color_white(), 0);
    lv_obj_set_style_text_font(_stageLabel, &lv_font_montserrat_14, 0);
}

void PocketSpirit::updateStatsOverlay() {
    if (!_energyBar) return;

    lv_bar_set_value(_energyBar, (int32_t)_stats.getEnergy(), LV_ANIM_ON);
    lv_bar_set_value(_happinessBar, (int32_t)_stats.getHappiness(), LV_ANIM_ON);
    lv_bar_set_value(_affectionBar, (int32_t)_stats.getAffection(), LV_ANIM_ON);

    if (_stageLabel) {
        lv_label_set_text(_stageLabel, evolutionStageToStr(_evolution.getStage()));
    }
}

void PocketSpirit::checkAutoSave() {
    unsigned long now = millis();
    if ((now - _lastAutoSave) >= _autoSaveMs) {
        saveState();
        _lastAutoSave = now;
    }
}

} // namespace PocketSpirit
