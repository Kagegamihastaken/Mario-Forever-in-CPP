#include "Class/ActiveObjectClass.hpp"

namespace MFCPP {
    void ActiveObject::setCurrentPosition(const sf::Vector2f& position) {
        m_curr = position;
    }
    sf::Vector2f ActiveObject::getCurrentPosition() const {
        return m_curr;
    }
    void ActiveObject::setPreviousPosition(const sf::Vector2f &position) {
        m_prev = position;
    }
    sf::Vector2f ActiveObject::getPreviousPosition() const {
        return m_prev;
    }
    void ActiveObject::setInterpolatedPosition(const sf::Vector2f& pos) {
        m_interpolated_pos = pos;
    }
    sf::Vector2f ActiveObject::getInterpolatedPosition() const {
        return m_interpolated_pos;
    }
    void ActiveObject::setOrigin(const sf::Vector2f& origin) {
        m_origin = origin;
    }
    sf::Vector2f ActiveObject::getOrigin() const {
        return m_origin;
    }
    void ActiveObject::move(const sf::Vector2f& pos) {
        m_curr += pos;
    }
}