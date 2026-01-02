#include "Core/Class/BulletBillClass.hpp"

namespace MFCPP {
    BulletBillAI::BulletBillAI(const float speed, const BulletType type, const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin) : m_speed(speed), m_hitbox(hitbox), m_type(type) {
        setCurrentPosition(pos);
        setPreviousPosition(pos);
        setInterpolatedPosition(pos);

        setOrigin(origin);
    }
    void BulletBillAI::setSpeed(const float speed) {
        m_speed = speed;
    }
    float BulletBillAI::getSpeed() const {
        return m_speed;
    }
    void BulletBillAI::setHitbox(const sf::FloatRect &hitbox) {
        m_hitbox = hitbox;
    }
    sf::FloatRect BulletBillAI::getHitbox() const {
        return m_hitbox;
    }
    void BulletBillAI::setType(const BulletType val) {
        m_type = val;
    }
    BulletType BulletBillAI::getType() const {
        return m_type;
    }

}