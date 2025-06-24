#include "Class/MarioProjectileClass.hpp"
#include "Core/WindowFrame.hpp"

namespace MFCPP {
    MarioProjectile::MarioProjectile(const bool direction, const MarioProjectileType type, const MarioProjectileBehavior behaviour, const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin) : m_direction(direction), m_type(type), m_behaviour(behaviour), m_hitbox(hitbox) {
        setCurrentPosition(pos);
        setPreviousPosition(pos);
        setInterpolatedPosition(pos);
        setOrigin(origin);
        m_Xvelo = 8.125f;
        m_Yvelo = 0.f;
        m_destroy = false;
    }
    void MarioProjectile::setYVelo(const float val) {
        m_Yvelo = val;
    }
    float MarioProjectile::getYVelo() const {
        return m_Yvelo;
    }
    void MarioProjectile::setXVelo(const float val) {
        m_Xvelo = val;
    }
    float MarioProjectile::getXVelo() const {
        return m_Xvelo;
    }
    bool MarioProjectile::getDirection() const {
        return m_direction;
    }
    void MarioProjectile::setType(const MarioProjectileType val) {
        m_type = val;
    }
    MarioProjectileType MarioProjectile::getType() const {
        return m_type;
    }
    void MarioProjectile::setBehaviour(const MarioProjectileBehavior val) {
        m_behaviour = val;
    }
    MarioProjectileBehavior MarioProjectile::getBehaviour() const {
        return m_behaviour;
    }
    void MarioProjectile::setHitbox(const sf::FloatRect& hitbox) {
        m_hitbox = hitbox;
    }
    sf::FloatRect MarioProjectile::getHitbox() const {
        return m_hitbox;
    }
    void MarioProjectile::willDestroy(const bool val) {
        m_destroy = val;
    }
    bool MarioProjectile::willBeDestroyed() const {
        return m_destroy;
    }
}