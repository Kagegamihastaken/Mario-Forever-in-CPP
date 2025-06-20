#include "Class/GoombaAIEffectClass.hpp"

#include <iostream>

namespace MFCPP {
    GoombaAIEffect::GoombaAIEffect(const GoombaAIEffectID id, const int SkinID, const float Yvelo, const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin)
        : m_id(id), m_Yvelo(Yvelo), m_hitbox(hitbox), m_skinID(SkinID) {
        setOrigin(origin);
        setCurrentPosition(pos);
        setPreviousPosition(pos);
        setInterpolatedPosition(pos);
        m_alpha = 255.f;
    }
    void GoombaAIEffect::setID(const GoombaAIEffectID id) {
        m_id = id;
    }
    GoombaAIEffectID GoombaAIEffect::getID() const {
        return m_id;
    }
    void GoombaAIEffect::setYVelo(const float val) {
        m_Yvelo = val;
    }
    float GoombaAIEffect::getYVelo() const {
        return m_Yvelo;
    }
    void GoombaAIEffect::setHitbox(const sf::FloatRect& hitbox) {
        m_hitbox = hitbox;
    }
    sf::FloatRect GoombaAIEffect::getHitbox() const {
        return m_hitbox;
    }
    void GoombaAIEffect::setAlpha(const float val) {
        m_alpha = val;
    }
    float GoombaAIEffect::getAlpha() const {
        return m_alpha;
    }
    void GoombaAIEffect::restartClock() {
        m_clock.restart();
    }
    sf::Clock GoombaAIEffect::getClock() const {
        return m_clock;
    }
    void GoombaAIEffect::setSkinID(const int val) {
        m_skinID = val;
    }
    int GoombaAIEffect::getSkinID() const {
        return m_skinID;
    }
}
