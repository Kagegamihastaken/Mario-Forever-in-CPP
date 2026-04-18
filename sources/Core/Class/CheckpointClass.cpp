#include "Core/Class/CheckpointClass.hpp"

namespace MFCPP {
    Checkpoint::Checkpoint(const sf::FloatRect &hitbox, const sf::Vector2f &pos, const sf::Vector2f &origin) {
        setCurrentPosition(pos);
        setPreviousPosition(pos);
        setInterpolatedPosition(pos);
        setHitbox(hitbox);
        setOrigin(origin);
        m_isTouch = false;
        m_lastState = -1;
    }
    void Checkpoint::setTouch(const bool val) {
        m_isTouch = val;
    }
    bool Checkpoint::getTouch() const {
        return m_isTouch;
    }
    void Checkpoint::setLastState(const int val) {
        m_lastState = val;
    }
    int Checkpoint::getLastState() const {
        return m_lastState;
    }
}