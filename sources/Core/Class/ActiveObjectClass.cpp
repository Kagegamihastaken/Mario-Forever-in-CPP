#include "Core/Class/ActiveObjectClass.hpp"
namespace MFCPP {
    template <typename T>
    void ActiveObject<T>::setCurrentPosition(const sf::Vector2<T>& position) {
        m_curr = position;
    }
    template <typename T>
    const sf::Vector2<T>& ActiveObject<T>::getCurrentPosition() const {
        return m_curr;
    }
    template <typename T>
    void ActiveObject<T>::setPreviousPosition(const sf::Vector2<T> &position) {
        m_prev = position;
    }
    template <typename T>
    const sf::Vector2<T>& ActiveObject<T>::getPreviousPosition() const {
        return m_prev;
    }
    template <typename T>
    void ActiveObject<T>::setInterpolatedPosition(const sf::Vector2<T>& pos) {
        m_interpolated_pos = pos;
    }
    template <typename T>
    const sf::Vector2<T>& ActiveObject<T>::getInterpolatedPosition() const {
        return m_interpolated_pos;
    }
    template <typename T>
    void ActiveObject<T>::setOrigin(const sf::Vector2<T>& origin) {
        m_origin = origin;
    }
    template <typename T>
    const sf::Vector2<T>& ActiveObject<T>::getOrigin() const {
        return m_origin;
    }
    template <typename T>
    void ActiveObject<T>::move(const sf::Vector2<T>& pos) {
        m_curr += pos;
    }
    template <typename T>
    void ActiveObject<T>::setCurrentAngle(const sf::Angle& angle) {
        m_curr_angle = angle;
    }
    template <typename T>
    const sf::Angle& ActiveObject<T>::getCurrentAngle() const {
        return m_curr_angle;
    }
    template <typename T>
    void ActiveObject<T>::setPreviousAngle(const sf::Angle& angle) {
        m_prev_angle = angle;
    }
    template <typename T>
    const sf::Angle& ActiveObject<T>::getPreviousAngle() const {
        return m_prev_angle;
    }
    template <typename T>
    void ActiveObject<T>::rotate(const sf::Angle& angle) {
        m_curr_angle += angle;
    }
    template <typename T>
    const sf::Angle& ActiveObject<T>::getInterpolatedAngle() const {
        return m_interpolated_angle;
    }
    template <typename T>
    void ActiveObject<T>::setInterpolatedAngle(const sf::Angle& angle) {
        m_interpolated_angle = angle;
    }
    template <typename T>
    void ActiveObject<T>::setDestroyed(const bool val) {
        m_destroy = val;
    }
    template <typename T>
    bool ActiveObject<T>::isDestroyed() const {
        return m_destroy;
    }
    template <typename T>
    void ActiveObject<T>::setHitbox(const sf::Rect<T>& hitbox) {
        m_hitbox = hitbox;
    }
    template <typename T>
    const sf::Rect<T>& ActiveObject<T>::getHitbox() const {
        return m_hitbox;
    }
    template class ActiveObject<float>;
}