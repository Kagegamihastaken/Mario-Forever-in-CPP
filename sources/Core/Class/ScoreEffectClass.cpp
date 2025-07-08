#include "Core/Class/ScoreEffectClass.hpp"

namespace MFCPP {
    ScoreEffect::ScoreEffect(const sf::Vector2f &pos, const sf::Vector2f &origin) {
        setCurrentPosition(pos);
        setPreviousPosition(pos);
        setInterpolatedPosition(pos);
        setOrigin(origin);

        m_alpha = 255.f;
        m_velocity = -1.5f;
    }
    void ScoreEffect::setVelocity(const float val) {
        m_velocity = val;
    }
    float ScoreEffect::getVelocity() const {
        return m_velocity;
    }
    void ScoreEffect::setAlpha(const float val) {
        m_alpha = val;
    }
    float ScoreEffect::getAlpha() const {
        return m_alpha;
    }
}