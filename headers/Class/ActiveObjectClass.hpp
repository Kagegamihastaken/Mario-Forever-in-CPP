#ifndef ACTIVEOBJECTCLASS_HPP
#define ACTIVEOBJECTCLASS_HPP

#include <SFML/Graphics.hpp>

namespace MFCPP {
    class ActiveObject {
    public:
        void setCurrentPosition(const sf::Vector2f& position);
        [[nodiscard]] sf::Vector2f getCurrentPosition() const;
        void setPreviousPosition(const sf::Vector2f& position);
        [[nodiscard]] sf::Vector2f getPreviousPosition() const;
        void setInterpolatedPosition(const sf::Vector2f& pos);
        [[nodiscard]] sf::Vector2f getInterpolatedPosition() const;
        void setOrigin(const sf::Vector2f& origin);
        [[nodiscard]] sf::Vector2f getOrigin() const;
        void move(const sf::Vector2f& pos);
    private:
        sf::Vector2f          m_curr{};
        sf::Vector2f          m_prev{};
        sf::Vector2f          m_interpolated_pos{};
        sf::Vector2f          m_origin{};
    };
}

#endif
