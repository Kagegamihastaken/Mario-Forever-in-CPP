#include "Core/Class/ActiveObjectClass.hpp"
namespace MFCPP {
    template <typename T>
    void ActiveObject<T>::setCurrentPosition(sf::Vector2<T> position) noexcept {
        m_curr = position;
    }
    template <typename T>
    sf::Vector2<T> ActiveObject<T>::getCurrentPosition() const noexcept {
        return m_curr;
    }
    template <typename T>
    void ActiveObject<T>::setPreviousPosition(sf::Vector2<T> position) noexcept {
        m_prev = position;
    }
    template <typename T>
    sf::Vector2<T> ActiveObject<T>::getPreviousPosition() const noexcept {
        return m_prev;
    }
    template <typename T>
    void ActiveObject<T>::setInterpolatedPosition(sf::Vector2<T> pos) noexcept {
        m_interpolated_pos = pos;
    }
    template <typename T>
    sf::Vector2<T> ActiveObject<T>::getInterpolatedPosition() const noexcept {
        return m_interpolated_pos;
    }
    template <typename T>
    void ActiveObject<T>::setOrigin(sf::Vector2<T> origin) noexcept {
        m_origin = origin;
    }
    template <typename T>
    sf::Vector2<T> ActiveObject<T>::getOrigin() const noexcept {
        return m_origin;
    }
    template <typename T>
    void ActiveObject<T>::move(sf::Vector2<T> pos) noexcept {
        m_curr += pos;
    }
    template <typename T>
    void ActiveObject<T>::setCurrentAngle(sf::Angle angle) noexcept {
        m_curr_angle = angle;
    }
    template <typename T>
    sf::Angle ActiveObject<T>::getCurrentAngle() const noexcept {
        return m_curr_angle;
    }
    template <typename T>
    void ActiveObject<T>::setPreviousAngle(sf::Angle angle) noexcept {
        m_prev_angle = angle;
    }
    template <typename T>
    sf::Angle ActiveObject<T>::getPreviousAngle() const noexcept {
        return m_prev_angle;
    }
    template <typename T>
    void ActiveObject<T>::rotate(sf::Angle angle) noexcept {
        m_curr_angle += angle;
    }
    template <typename T>
    sf::Angle ActiveObject<T>::getInterpolatedAngle() const noexcept {
        return m_interpolated_angle;
    }
    template <typename T>
    void ActiveObject<T>::setInterpolatedAngle(sf::Angle angle) noexcept {
        m_interpolated_angle = angle;
    }
    template <typename T>
    void ActiveObject<T>::setDestroyed(bool val) noexcept {
        m_destroy = val;
    }
    template <typename T>
    bool ActiveObject<T>::isDestroyed() const noexcept {
        return m_destroy;
    }
    template <typename T>
    void ActiveObject<T>::setHitbox(const sf::Rect<T>& hitbox) noexcept {
        m_hitbox = hitbox;
    }
    template <typename T>
    const sf::Rect<T>& ActiveObject<T>::getHitbox() const noexcept {
        return m_hitbox;
    }
    template <typename T>
    void ActiveObject<T>::forceSetPosition(sf::Vector2<T> position) noexcept {
        m_curr = position;
        m_prev = position;
    }
}