#include "Core/Class/SpikeClass.hpp"

namespace MFCPP {
    Spike::Spike(const SpikeID id, const sf::FloatRect &hitbox, const sf::Vector2f &pos, const sf::Vector2f &origin)
        : m_id(id), m_hitbox(hitbox){
        setCurrentPosition(pos);
        setPreviousPosition(pos);
        setInterpolatedPosition(pos);
        setOrigin(origin);
    }
    void Spike::setID(const SpikeID val) {
        m_id = val;
    }
    SpikeID Spike::getID() const {
        return m_id;
    }
    void Spike::setHitbox(const sf::FloatRect &hitbox) {
        m_hitbox = hitbox;
    }
    sf::FloatRect Spike::getHitbox() const {
        return m_hitbox;
    }





}