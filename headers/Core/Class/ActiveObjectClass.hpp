#pragma once

#ifndef ACTIVEOBJECTCLASS_HPP
#define ACTIVEOBJECTCLASS_HPP

namespace MFCPP {
    template <typename T>
    class ActiveObject {
    public:
        virtual ~ActiveObject() = default;
        explicit ActiveObject() = default;
        void setCurrentPosition(sf::Vector2<T> position) noexcept;
        [[nodiscard]] sf::Vector2<T> getCurrentPosition() const noexcept;
        void setPreviousPosition(sf::Vector2<T> position) noexcept;
        [[nodiscard]] sf::Vector2<T> getPreviousPosition() const noexcept;
        void setInterpolatedPosition(sf::Vector2<T> pos) noexcept;
        [[nodiscard]] sf::Vector2<T> getInterpolatedPosition() const noexcept;
        void setOrigin(sf::Vector2<T> origin) noexcept;
        [[nodiscard]] sf::Vector2<T> getOrigin() const noexcept;
        void move(sf::Vector2<T> pos) noexcept;
        [[nodiscard]] const sf::Rect<T>& getHitbox() const noexcept;
        void setHitbox(const sf::Rect<T>& hitbox) noexcept;
        void setCurrentAngle(sf::Angle angle) noexcept;
        [[nodiscard]] sf::Angle getCurrentAngle() const noexcept;
        void setPreviousAngle(sf::Angle angle) noexcept;
        [[nodiscard]] sf::Angle getPreviousAngle() const noexcept;
        void rotate(sf::Angle angle) noexcept;
        [[nodiscard]] sf::Angle getInterpolatedAngle() const noexcept;
        void setInterpolatedAngle(sf::Angle angle) noexcept;
        void setDestroyed(bool val) noexcept;
        [[nodiscard]] bool isDestroyed() const noexcept;
        void forceSetPosition(sf::Vector2<T> position) noexcept;
    private:
        sf::Vector2<T> m_curr{};
        sf::Vector2<T> m_prev{};
        sf::Vector2<T> m_interpolated_pos{};
        sf::Vector2<T> m_origin{};
        sf::Angle    m_curr_angle{};
        sf::Angle    m_prev_angle{};
        sf::Angle    m_interpolated_angle{};
        sf::Rect<T>  m_hitbox{};
        bool         m_destroy = false;
    };

    template class ActiveObject<float>;
}

#endif
