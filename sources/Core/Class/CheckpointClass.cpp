#include "Core/Class/CheckpointClass.hpp"

namespace MFCPP {
    Checkpoint::Checkpoint(const sf::FloatRect &hitbox, const sf::Vector2f &pos, const sf::Vector2f &origin) : m_hitbox(hitbox) {
        setCurrentPosition(pos);
        setPreviousPosition(pos);
        setInterpolatedPosition(pos);
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
    void Checkpoint::setHitbox(const sf::FloatRect &val) {
        m_hitbox = val;
    }
    sf::FloatRect Checkpoint::getHitbox() const {
        return m_hitbox;
    }
    void Checkpoint::setLastState(const int val) {
        m_lastState = val;
    }
    int Checkpoint::getLastState() const {
        return m_lastState;
    }
}