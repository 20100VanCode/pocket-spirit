#pragma once

#include "EyeRenderer.h"
#include "core/CreatureState.h"

namespace PocketSpirit {

struct AnimationFrame {
    Expression  expression;
    uint16_t    durationMs;
};

class AnimationEngine {
public:
    AnimationEngine();

    void init(EyeRenderer* renderer);
    void play(const AnimationFrame* frames, uint8_t count, bool loop = false);
    void update(float deltaTimeSec);
    void stop();

    bool isPlaying() const { return _playing; }

    void setIdleAnimation(const AnimationFrame* frames, uint8_t count);
    void playIdle();

private:
    EyeRenderer* _renderer = nullptr;

    const AnimationFrame* _currentAnim = nullptr;
    uint8_t _frameCount   = 0;
    uint8_t _currentFrame = 0;
    float   _frameTimer   = 0.0f;
    bool    _playing      = false;
    bool    _looping      = false;

    const AnimationFrame* _idleAnim   = nullptr;
    uint8_t _idleFrameCount = 0;
};

} // namespace PocketSpirit
