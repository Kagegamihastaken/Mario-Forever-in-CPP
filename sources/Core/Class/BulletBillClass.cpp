#include "Core/Class/BulletBillClass.hpp"

namespace MFCPP {
    BulletBill::BulletBill(const float speed, const BulletType type, const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin) : m_speed(speed), m_hitbox(hitbox), m_type(type) {
        setCurrentPosition(pos);
        setPreviousPosition(pos);
        setInterpolatedPosition(pos);

        setOrigin(origin);
    }
    void BulletBill::setSpeed(const float speed) {
        m_speed = speed;
    }
    float BulletBill::getSpeed() const {
        return m_speed;
    }
    void BulletBill::setHitbox(const sf::FloatRect &hitbox) {
        m_hitbox = hitbox;
    }
    sf::FloatRect BulletBill::getHitbox() const {
        return m_hitbox;
    }
    void BulletBill::setType(const BulletType val) {
        m_type = val;
    }
    BulletType BulletBill::getType() const {
        return m_type;
    }

}