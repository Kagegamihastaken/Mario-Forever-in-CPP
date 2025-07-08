#pragma once

#ifndef BULLETBILLCLASS_HPP
#define BULLETBILLCLASS_HPP
#include "ActiveObjectClass.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "SFML/Graphics/Rect.hpp"

namespace MFCPP {
    class BulletBill final : public ActiveObject, public SingleAnimationObject {
    public:
        BulletBill(float speed, BulletType type, const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin);
        ~BulletBill() = default;
        void setSpeed(float speed);
        [[nodiscard]] float getSpeed() const;
        void setHitbox(const sf::FloatRect& hitbox);
        [[nodiscard]] sf::FloatRect getHitbox() const;
        void setType(BulletType val);
        [[nodiscard]] BulletType getType() const;
    private:
        float         m_speed;
        sf::FloatRect m_hitbox;
        BulletType    m_type;
    };
}

#endif //BULLETBILLCLASS_HPP
