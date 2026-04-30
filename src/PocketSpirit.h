#pragma once

#include "core/CreatureState.h"
#include "core/Stats.h"
#include "core/Evolution.h"
#include "persistence/StateStorage.h"
#include "display/DisplayManager.h"
#include "display/EyeRenderer.h"
#include "display/AnimationEngine.h"
#include "input/InputManager.h"
#include "input/TouchHandler.h"
#include "input/MotionHandler.h"
#include "power/PowerManager.h"
#include "creatures/CreatureBase.h"
#include "creatures/CreatureRegistry.h"
#include "creatures/Lumy.h"
#include "creatures/Dragon.h"
#include "creatures/Ghost.h"

#include <functional>

namespace PocketSpirit {

class PocketSpirit {
public:
    PocketSpirit();
    ~PocketSpirit();

    bool begin(const char* creatureName = "Lumy");
    void update();

    // Interactions
    void pet();
    void feed();
    void play();

    // Queries
    const CreatureState& getState() const  { return _state; }
    float getEnergy() const                { return _stats.getEnergy(); }
    float getHappiness() const             { return _stats.getHappiness(); }
    float getAffection() const             { return _stats.getAffection(); }
    EvolutionStage getEvolutionStage() const { return _evolution.getStage(); }
    Expression getCurrentExpression() const { return _state.currentExpression; }

    // Configuration
    void setInactivityTimeout(uint32_t ms);
    void registerCreature(CreatureBase* creature);
    void setActiveCreature(const char* name);

    // Callbacks
    void onEvolve(std::function<void(EvolutionStage)> callback);
    void onStatsLow(std::function<void()> callback);
    void onInteraction(std::function<void(const char*)> callback);

    // Stats display
    void showStats(bool show);
    void setAutoSaveInterval(uint32_t ms);

private:
    Stats           _stats;
    Evolution       _evolution;
    StateStorage    _storage;
    DisplayManager  _display;
    EyeRenderer     _eyes;
    AnimationEngine _animation;
    InputManager    _input;
    PowerManager    _power;
    CreatureBase*   _creature = nullptr;
    CreatureState   _state;

    // Built-in creatures
    Lumy   _lumy;
    Dragon _dragon;
    Ghost  _ghost;

    bool          _initialized      = false;
    unsigned long _lastUpdateTime   = 0;
    unsigned long _lastAutoSave     = 0;
    uint32_t      _autoSaveMs       = 30000; // 30s
    bool          _showStats        = true;

    lv_obj_t* _statsBar       = nullptr;
    lv_obj_t* _energyBar      = nullptr;
    lv_obj_t* _happinessBar   = nullptr;
    lv_obj_t* _affectionBar   = nullptr;
    lv_obj_t* _stageLabel     = nullptr;

    std::function<void(EvolutionStage)> _evolveCb;
    std::function<void()>               _statsLowCb;
    std::function<void(const char*)>    _interactionCb;

    void handleTouch(TouchZone zone, const TouchPoint& pt);
    void handleMotion(MotionEvent event);
    void updateState(float dt);
    void saveState();
    void loadState();
    void applyCreatureTheme();
    void createStatsOverlay();
    void updateStatsOverlay();
    void determineExpression();
    void checkAutoSave();

    static constexpr float UPDATE_INTERVAL_MS = 50.0f;
};

} // namespace PocketSpirit
