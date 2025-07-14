#ifndef PLATFORMCLASS_HPP
#define PLATFORMCLASS_HPP
#include "ActiveObjectClass.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"

namespace MFCPP {
    class Platform final : public ActiveObject, public SingleAnimationObject {
    public:
        explicit Platform(const sf::Vector2f& start, const sf::Vector2f& end, float speed, const sf::FloatRect& hitbox);
        void setHitbox(const sf::FloatRect& val);
        [[nodiscard]] sf::FloatRect getHitbox() const;
        void setStartPoint(const sf::Vector2f& val);
        [[nodiscard]] sf::Vector2f getStartPoint() const;
        void setEndPoint(const sf::Vector2f& val);
        [[nodiscard]] sf::Vector2f getEndPoint() const;
        void setSpeed(const float& val);
        [[nodiscard]] float getSpeed() const;
        void setMovingToEnd(const bool& val);
        [[nodiscard]] bool isMovingToEnd() const;
    private:
        sf::FloatRect m_hitbox;
        sf::Vector2f  m_startPoint;
        sf::Vector2f  m_endPoint;
        float         m_speed;
        bool          m_movingToEnd;
    };
}

#endif //PLATFORMCLASS_HPP
