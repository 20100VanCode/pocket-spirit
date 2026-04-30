#include "AnimationEngine.h"

namespace PocketSpirit {

AnimationEngine::AnimationEngine() {}

void AnimationEngine::init(EyeRenderer* renderer) {
    _renderer = renderer;
}

void AnimationEngine::play(const AnimationFrame* frames, uint8_t count, bool loop) {
    if (!frames || count == 0 || !_renderer) return;

    _currentAnim  = frames;
    _frameCount   = count;
    _currentFrame = 0;
    _frameTimer   = 0.0f;
    _playing      = true;
    _looping      = loop;

    _renderer->setExpression(_currentAnim[0].expression);
}

void AnimationEngine::update(float deltaTimeSec) {
    if (!_playing || !_currentAnim) return;

    _frameTimer += deltaTimeSec * 1000.0f;

    if (_frameTimer >= (float)_currentAnim[_currentFrame].durationMs) {
        _frameTimer = 0.0f;
        _currentFrame++;

        if (_currentFrame >= _frameCount) {
            if (_looping) {
                _currentFrame = 0;
            } else {
                _playing = false;
                playIdle();
                return;
            }
        }

        _renderer->setExpression(_currentAnim[_currentFrame].expression);
    }
}

void AnimationEngine::stop() {
    _playing = false;
    _currentAnim  = nullptr;
    _frameCount   = 0;
    _currentFrame = 0;
}

void AnimationEngine::setIdleAnimation(const AnimationFrame* frames, uint8_t count) {
    _idleAnim       = frames;
    _idleFrameCount = count;
}

void AnimationEngine::playIdle() {
    if (_idleAnim && _idleFrameCount > 0) {
        play(_idleAnim, _idleFrameCount, true);
    } else if (_renderer) {
        _renderer->setExpression(Expression::IDLE);
    }
}

} // namespace PocketSpirit
