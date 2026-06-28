#pragma once

#ifndef ACTIVEOBJECT_HPP
#define ACTIVEOBJECT_HPP
#include "Core/Interpolation.hpp"
#include <SFML/System/Vector2.hpp>

namespace MFCPP {
    template <typename T>
    class ActiveObject {
    public:
        explicit ActiveObject(sf::Vector2<T> position, sf::Vector2<T> origin, sf::Angle angle)
            : m_curr(position),
            m_prev(position),
            m_origin(origin),
            m_curr_angle(angle),
            m_prev_angle(angle),
            m_destroy(false) {}
        ~ActiveObject() = default;
        void setCurrentPosition(sf::Vector2<T> position) noexcept {
            m_curr = position;
        }
        void setCurrentAngle(sf::Angle angle) noexcept {
            m_curr_angle = angle;
        }
        void setOrigin(sf::Vector2<T> origin) noexcept {
            m_origin = origin;
        }
        [[nodiscard]] sf::Vector2<T> getCurrentPosition() const noexcept {
            return m_curr;
        }
        void Update() noexcept {
            m_prev = m_curr;
            m_prev_angle = m_curr_angle;
        }
        [[nodiscard]] sf::Vector2<T> getPreviousPosition() const noexcept {
            return m_prev;
        }
        [[nodiscard]] sf::Vector2<T> getInterpolatedPosition(float alpha) const noexcept {
            return Interpolation::linearInterpolation(m_prev, m_curr, alpha);
        }
        [[nodiscard]] sf::Vector2<T> getOrigin() const noexcept {
            return m_origin;
        }
        void move(sf::Vector2<T> pos) noexcept {
            m_curr += pos;
        }
        void moveTeleport(sf::Vector2<T> pos) noexcept {
            move(pos);
            m_prev = m_curr;
        }
        [[nodiscard]] sf::Angle getCurrentAngle() const noexcept {
            return m_curr_angle;
        }
        [[nodiscard]] sf::Angle getPreviousAngle() const noexcept {
            return m_prev_angle;
        }
        void rotate(sf::Angle angle) noexcept {
            m_curr_angle += angle;
        }
        [[nodiscard]] sf::Angle getInterpolatedAngle(float alpha) const noexcept {
            return Interpolation::linearInterpolation(m_prev_angle, m_curr_angle, alpha);
        }
        void destroy() noexcept {
            m_destroy = true;
        }
        [[nodiscard]] bool isDestroyed() const noexcept {
            return m_destroy;
        }
        void teleport(sf::Vector2<T> position) noexcept {
            m_curr = position;
            m_prev = position;
        }
    private:
        sf::Vector2<T> m_curr{};
        sf::Vector2<T> m_prev;
        sf::Vector2<T> m_origin;
        sf::Angle    m_curr_angle;
        sf::Angle    m_prev_angle;
        bool         m_destroy;
    };

    template class ActiveObject<float>;
}

#endif
