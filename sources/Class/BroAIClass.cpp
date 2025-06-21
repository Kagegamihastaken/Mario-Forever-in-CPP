#include "Class/BroAIClass.hpp"

namespace MFCPP {
    BroAI::BroAI(const float speed, const float stop_duration, const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin) : m_stop_duration(stop_duration), m_speed(speed) {
        setOrigin(origin);
        setCurrentPosition(pos);
        setPreviousPosition(pos);
        setInterpolatedPosition(pos);
        m_state = 1;
        m_fixed_walking_value = 32.f;
        m_fixed_ticking_value = 1.f;
        m_random_offset_wv = 0.f;
        m_time_ticking = 0.f;
        m_direction = false;
        m_ticking_value = m_fixed_ticking_value;
        m_RNG_walking_range = 64.f;
        m_RNG_ticking_range = 5.f;
        m_lastY = -1.f;
        m_isFalling = false;

        m_hitbox = sf::FloatRect({ 0.0f, 0.0f }, { hitbox.size.x, hitbox.size.y });
        m_JumpClock.restart();

    }
    void BroAI::setMovingValue(const float val) {
        m_movingValue = val;
    }
    float BroAI::getMovingValue() const{
        return m_movingValue;
    }
    void BroAI::setSpeed(const float val) {
        m_speed = val;
    }
    float BroAI::getSpeed() const {
        return m_speed;
    }
    void BroAI::setState(const int val) {
        m_state = val;
    }
    int BroAI::getState() const {
        return m_state;
    }
    float BroAI::getFixedWalkingValue() const {
        return m_fixed_walking_value;
    }
    void BroAI::setRandomWalking(const float val) {
        m_random_offset_wv = val;
    }
    float BroAI::getRandomWalking() const {
        return m_random_offset_wv;
    }
    void BroAI::setDirectionWalking(const bool val) {
        m_direction = val;
    }
    bool BroAI::getDirectionWalking() const {
        return m_direction;
    }
    void BroAI::setStopDuration(const float val) {
        m_stop_duration = val;
    }
    float BroAI::getStopDuration() const {
        return m_stop_duration;
    }
    void BroAI::setTimeTicking(const float val) {
        m_time_ticking = val;
    }
    float BroAI::getTimeTicking() const {
        return m_time_ticking;
    }
    void BroAI::setTickingValue(const float val) {
        m_ticking_value = val;
    }
    float BroAI::getTickingValue() const {
        return m_ticking_value;
    }
    float BroAI::getFixedTickingValue() const {
        return m_fixed_ticking_value;
    }
    float BroAI::getWalkingRNGRange() const {
        return m_RNG_walking_range;
    }
    float BroAI::getTickingRNGRange() const {
        return m_RNG_ticking_range;
    }
    void BroAI::setHitbox(const sf::FloatRect& hitbox) {
        m_hitbox = hitbox;
    }
    sf::FloatRect BroAI::getHitbox() const {
        return m_hitbox;
    }
    void BroAI::setYVelo(const float val) {
        m_Yvelo = val;
    }
    float BroAI::getYVelo() const {
        return m_Yvelo;
    }
    void BroAI::restartJumpClock() {
        m_JumpClock.restart();
    }
    sf::Clock BroAI::getJumpClock() const {
        return m_JumpClock;
    }
    void BroAI::setFalling(const bool val) {
        m_isFalling = val;
    }
    bool BroAI::isFalling() const {
        return m_isFalling;
    }
    void BroAI::setLastY(const float val) {
        m_lastY = val;
    }
    float BroAI::getLastY() const {
        return m_lastY;
    }
}