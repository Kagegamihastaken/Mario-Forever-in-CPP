#include "Core/Class/PlatformClass.hpp"

namespace MFCPP {
    Platform::Platform(const sf::Vector2f& start, const sf::Vector2f& end, const float speed, const sf::FloatRect& hitbox) : m_hitbox(hitbox), m_startPoint(start), m_endPoint(end), m_speed(speed) {
        setCurrentPosition(start);
        setPreviousPosition(start);
        setInterpolatedPosition(start);
        setOrigin(sf::Vector2f(0.f, 0.f));
        m_movingToEnd = true;
    }
    void Platform::setHitbox(const sf::FloatRect &val) {
        m_hitbox = val;
    }
    sf::FloatRect Platform::getHitbox() const {
        return m_hitbox;
    }
    void Platform::setStartPoint(const sf::Vector2f &val) {
        m_startPoint = val;
    }
    sf::Vector2f Platform::getStartPoint() const {
        return m_startPoint;
    }
    void Platform::setEndPoint(const sf::Vector2f &val) {
        m_endPoint = val;
    }
    sf::Vector2f Platform::getEndPoint() const {
        return m_endPoint;
    }
    void Platform::setSpeed(const float &val) {
        m_speed = val;
    }
    float Platform::getSpeed() const {
        return m_speed;
    }
    void Platform::setMovingToEnd(const bool &val) {
        m_movingToEnd = val;
    }
    bool Platform::isMovingToEnd() const {
        return  m_movingToEnd;
    }
}