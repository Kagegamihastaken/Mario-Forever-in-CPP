#pragma once

#ifndef BULLETBILLCLASS_HPP
#define BULLETBILLCLASS_HPP
#include "ActiveObjectClass.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "SFML/Graphics/Rect.hpp"

namespace MFCPP {
    class BulletBill final : public ActiveObject, public SingleAnimationObject {
    public:
        BulletBill(float speed, bool direction, const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin);
        ~BulletBill() = default;
        void setSpeed(float speed);
        [[nodiscard]] float getSpeed() const;
        void setHitbox(const sf::FloatRect& hitbox);
        [[nodiscard]] sf::FloatRect getHitbox() const;
        void setDirection(bool direction);
        [[nodiscard]] bool getDirection() const;
    private:
        float         m_speed;
        sf::FloatRect m_hitbox;
        bool          m_direction;
    };
}

#endif //BULLETBILLCLASS_HPP
