#include "Core/Class/CheckpointClass.hpp"

namespace MFCPP {
    Checkpoint::Checkpoint(const sf::FloatRect &hitbox, const sf::Vector2f &pos, const sf::Vector2f &origin) : m_hitbox(hitbox) {
        setCurrentPosition(pos);
        setPreviousPosition(pos);
        setInterpolatedPosition(pos);
        setOrigin(origin);
    }
    void Checkpoint::setTouch(const bool val) {
        m_isTouch = val;
    }
    bool Checkpoint::getTouch() const {
        return m_isTouch;
    }
}