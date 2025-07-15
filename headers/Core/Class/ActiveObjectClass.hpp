#pragma once

#ifndef ACTIVEOBJECTCLASS_HPP
#define ACTIVEOBJECTCLASS_HPP

namespace MFCPP {
    template <typename T>
    class ActiveObject {
    public:
        ActiveObject() = default;
        void setCurrentPosition(const sf::Vector2<T>& position);
        [[nodiscard]] const sf::Vector2<T>& getCurrentPosition() const;
        void setPreviousPosition(const sf::Vector2<T>& position);
        [[nodiscard]] const sf::Vector2<T>& getPreviousPosition() const;
        void setInterpolatedPosition(const sf::Vector2<T>& pos);
        [[nodiscard]] const sf::Vector2<T>& getInterpolatedPosition() const;
        void setOrigin(const sf::Vector2<T>& origin);
        [[nodiscard]] const sf::Vector2<T>& getOrigin() const;
        void move(const sf::Vector2<T>& pos);
        void setCurrentAngle(const sf::Angle& angle);
        [[nodiscard]] const sf::Angle& getCurrentAngle() const;
        void setPreviousAngle(const sf::Angle& angle);
        [[nodiscard]] const sf::Angle& getPreviousAngle() const;
        void rotate(const sf::Angle& angle);
        [[nodiscard]] const sf::Angle& getInterpolatedAngle() const;
        void setInterpolatedAngle(const sf::Angle& angle);
        void setDestroyed(bool val);
        [[nodiscard]] bool isDestroyed() const;
    private:
        sf::Vector2<T> m_curr{};
        sf::Vector2<T> m_prev{};
        sf::Vector2<T> m_interpolated_pos{};
        sf::Vector2<T> m_origin{};
        sf::Angle    m_curr_angle{};
        sf::Angle    m_prev_angle{};
        sf::Angle    m_interpolated_angle{};
        bool         m_destroy{};
    };

    extern template class ActiveObject<float>;
}

#endif
