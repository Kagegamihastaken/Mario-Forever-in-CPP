#include "Class/ActiveObjectClass.hpp"

namespace MFCPP {
    ActiveObject::ActiveObject() {
        m_destroy = false;
    };
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
    void ActiveObject::setCurrentAngle(const sf::Angle& angle) {
        m_curr_angle = angle;
    }
    sf::Angle ActiveObject::getCurrentAngle() const {
        return m_curr_angle;
    }
    void ActiveObject::setPreviousAngle(const sf::Angle& angle) {
        m_prev_angle = angle;
    }
    sf::Angle ActiveObject::getPreviousAngle() const {
        return m_prev_angle;
    }
    void ActiveObject::rotate(const sf::Angle& angle) {
        m_curr_angle += angle;
    }
    sf::Angle ActiveObject::getInterpolatedAngle() const {
        return m_interpalated_angle;
    }
    void ActiveObject::setInterpolatedAngle(const sf::Angle& angle) {
        m_interpalated_angle = angle;
    }
    void ActiveObject::willDestroy(const bool val) {
        m_destroy = val;
    }
    bool ActiveObject::willBeDestroyed() const {
        return m_destroy;
    }
}