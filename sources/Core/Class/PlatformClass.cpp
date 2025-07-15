#include "Core/Class/PlatformClass.hpp"

namespace MFCPP {
    Platform::Platform(const sf::Vector2f& start, const sf::Vector2f& end, const float speed, const sf::FloatRect& hitbox, const bool smooth, const bool fall, const bool wait)
    : m_hitbox(hitbox), m_startPoint(start), m_endPoint(end), m_speed(speed), m_smooth(smooth), m_fall(fall), m_wait(wait) {
        setCurrentPosition(start);
        setPreviousPosition(start);
        setInterpolatedPosition(start);
        setOrigin(sf::Vector2f(0.f, 0.f));
        m_movingToEnd = true;
        m_currentSpeed = 0.f;
        m_smoothStore = 0.f;
        m_isFall = false;
        m_Yvelo = 0.f;
        m_waitState = 0.f;
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
    void Platform::setSpeed(const float val) {
        m_speed = val;
    }
    float Platform::getSpeed() const {
        return m_speed;
    }
    void Platform::setMovingToEnd(const bool val) {
        m_movingToEnd = val;
    }
    bool Platform::isMovingToEnd() const {
        return m_movingToEnd;
    }
    void Platform::setCurrentSpeed(const float val) {
        m_currentSpeed = val;
    }
    float Platform::getCurrentSpeed() const {
        return m_currentSpeed;
    }
    void Platform::setSmoothStore(const float val) {
        m_smoothStore = val;
    }
    float Platform::getSmoothStore() const {
        return m_smoothStore;
    }
    bool Platform::isSmooth() const {
        return m_smooth;
    }
    bool Platform::isFall() const {
        return m_fall;
    }
    void Platform::setYVelo(const float val) {
        m_Yvelo = val;
    }
    float Platform::getYVelo() const {
        return m_Yvelo;
    }
    void Platform::setIsFall(const bool val) {
        m_isFall = val;
    }
    bool Platform::willFall() const {
        return m_isFall;
    }
    void Platform::setWaitSate(const unsigned val) {
        m_waitState = val;
    }
    unsigned Platform::getWaitState() const {
        return m_waitState;
    }
    bool Platform::isWait() const {
        return m_wait;
    }
}