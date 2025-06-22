#include "Class/BroAIProjectileClass.hpp"
#include "Core/WindowFrame.hpp"

namespace MFCPP {
    BroAIProjectile::BroAIProjectile(const bool direction, const BroAIProjectileType type, const BroAIProjectileBehavior behaviour, const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin) : m_direction(direction), m_type(type), m_behaviour(behaviour), m_hitbox(hitbox) {
        setCurrentPosition(pos);
        setPreviousPosition(pos);
        setInterpolatedPosition(pos);
        setOrigin(origin);
        m_Xvelo = 1.f + static_cast<float>(RandomIntNumberGenerator(0, 4));
        m_Yvelo = (6.f + static_cast<float>(RandomIntNumberGenerator(0, 4))) * -1.f;
    }
    void BroAIProjectile::setYVelo(const float val) {
        m_Yvelo = val;
    }
    float BroAIProjectile::getYVelo() const {
        return m_Yvelo;
    }
    void BroAIProjectile::setXVelo(const float val) {
        m_Xvelo = val;
    }
    float BroAIProjectile::getXVelo() const {
        return m_Xvelo;
    }
    bool BroAIProjectile::getDirection() const {
        return m_direction;
    }
    void BroAIProjectile::setType(const BroAIProjectileType val) {
        m_type = val;
    }
    BroAIProjectileType BroAIProjectile::getType() const {
        return m_type;
    }
    void BroAIProjectile::setBehaviour(const BroAIProjectileBehavior val) {
        m_behaviour = val;
    }
    BroAIProjectileBehavior BroAIProjectile::getBehaviour() const {
        return m_behaviour;
    }
    void BroAIProjectile::setHitbox(const sf::FloatRect& hitbox) {
        m_hitbox = hitbox;
    }
    sf::FloatRect BroAIProjectile::getHitbox() const {
        return m_hitbox;
    }
}