#include "Core/Class/CheckpointClass.hpp"

namespace MFCPP {
    Checkpoint::Checkpoint(const sf::FloatRect &hitbox, const sf::Vector2f &pos, const sf::Vector2f &origin)
        : m_hitbox(hitbox),
        m_transform(pos, origin, sf::degrees(0.f)){
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

    sf::FloatRect Checkpoint::getHitbox() const {
        return m_hitbox;
    }

    sf::Vector2f Checkpoint::getInterpolatedPosition(float alpha) {
        return m_transform.getInterpolatedPosition(alpha);
    }

    sf::Vector2f Checkpoint::getPosition() {
        return m_transform.getCurrentPosition();
    }

    sf::Vector2f Checkpoint::getOrigin() {
        return m_transform.getOrigin();
    }
}
