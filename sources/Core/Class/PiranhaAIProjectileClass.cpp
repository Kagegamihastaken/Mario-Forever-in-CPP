#include "Core/WindowFrame.hpp"
#include "Core/Class/PiranhaAIProjectileClass.hpp"

namespace MFCPP {
    PiranhaAIProjectile::PiranhaAIProjectile(const bool direction, const PiranhaProjectileType type, const PiranhaDirection piranha_dir, const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin)
    : m_direction(direction), m_type(type), m_hitbox(hitbox), m_piranha_dir(piranha_dir) {
        setCurrentPosition(pos);
        setPreviousPosition(pos);
        setInterpolatedPosition(pos);
        setOrigin(origin);
        switch (piranha_dir) {
            case PIRANHA_UP:
                m_Xvelo = static_cast<float>(RandomIntNumberGenerator(0, 4)) - static_cast<float>(RandomIntNumberGenerator(0, 4));
                m_Yvelo = (3.f + static_cast<float>(RandomIntNumberGenerator(0, 8))) * -1.f;
                break;
            case PIRANHA_DOWN:
                m_Xvelo = static_cast<float>(RandomIntNumberGenerator(0, 4)) - static_cast<float>(RandomIntNumberGenerator(0, 4));
                m_Yvelo = (3.f + static_cast<float>(RandomIntNumberGenerator(0, 3)));
                break;
            case PIRANHA_RIGHT:
                m_Xvelo = (3.f + static_cast<float>(RandomIntNumberGenerator(0, 3))) * -1.f;
                m_Yvelo = static_cast<float>(RandomIntNumberGenerator(0, 9)) - static_cast<float>(RandomIntNumberGenerator(0, 9));
                break;
            case PIRANHA_LEFT:
                m_Xvelo = (3.f + static_cast<float>(RandomIntNumberGenerator(0, 3)));
                m_Yvelo = static_cast<float>(RandomIntNumberGenerator(0, 9)) - static_cast<float>(RandomIntNumberGenerator(0, 9));
                break;
            default: ;
        }
    }
    void PiranhaAIProjectile::setYVelo(const float val) {
        m_Yvelo = val;
    }
    float PiranhaAIProjectile::getYVelo() const {
        return m_Yvelo;
    }
    void PiranhaAIProjectile::setXVelo(const float val) {
        m_Xvelo = val;
    }
    float PiranhaAIProjectile::getXVelo() const {
        return m_Xvelo;
    }
    bool PiranhaAIProjectile::getDirection() const {
        return m_direction;
    }
    void PiranhaAIProjectile::setType(const PiranhaProjectileType val) {
        m_type = val;
    }
    PiranhaProjectileType PiranhaAIProjectile::getType() const {
        return m_type;
    }
    void PiranhaAIProjectile::setHitbox(const sf::FloatRect& hitbox) {
        m_hitbox = hitbox;
    }
    sf::FloatRect PiranhaAIProjectile::getHitbox() const {
        return m_hitbox;
    }
    PiranhaDirection PiranhaAIProjectile::getPiranhaDirection() const {
        return m_piranha_dir;
    }
}