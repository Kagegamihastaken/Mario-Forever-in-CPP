#include "Class/BroAIClass.hpp"

namespace MFCPP {
    BroAI::BroAI(const BroAIType type, const BroAIMovementType movementType, const float speed, const float stop_duration, const float launch_interval, const int launch_count, const float launch_interval_between, const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin) : m_launch_count(launch_count), m_launch_interval_between(launch_interval_between), m_movementType(movementType), m_stop_duration(stop_duration), m_speed(speed), m_launch_interval(launch_interval), m_type(type) {
        setOrigin(origin);
        setCurrentPosition(pos);
        setPreviousPosition(pos);
        setInterpolatedPosition(pos);
        //movement
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
        m_hitbox = hitbox;
        m_JumpClock.restart();
        m_wall_hitbox = sf::FloatRect({hitbox.position.x, hitbox.position.y}, {hitbox.size.x, hitbox.size.y - 6.f});
        //launching
        m_launchRNG = 10.f;
        m_launchWaitTime = 30.f;
        m_launchTickingTime = 0.f;
        m_launch_interval_ticking = 0.f;
        m_launch_interval = 3.f;

        m_launch_counting = m_launch_count;
        m_launch_interval_bticking = m_launch_interval_between;

        m_disabled = true;
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
    void BroAI::setMovementType(const BroAIMovementType val) {
        m_movementType = val;
    }
    BroAIMovementType BroAI::getMovementType() const {
        return m_movementType;
    }
    void BroAI::setLaunchInterval(const float val) {
        m_launch_interval = val;
    }
    float BroAI::getLaunchInterval() const {
        return m_launch_interval;
    }
    float BroAI::getLaunchRNG() const {
        return m_launchRNG;
    }
    void BroAI::setLaunchWaitTime(const float val) {
        m_launchWaitTime = val;
    }
    float BroAI::getLaunchWaitTime() const {
        return m_launchWaitTime;
    }
    void BroAI::setLaunchIntervalTicking(const float val) {
        m_launch_interval_ticking = val;
    }
    float BroAI::getLaunchIntervalTicking() const {
        return m_launch_interval_ticking;
    }
    void BroAI::setLaunchTickingTime(const float val) {
        m_launchTickingTime = val;
    }
    float BroAI::getLaunchTickingTime() const {
        return m_launchTickingTime;
    }
    void BroAI::setWallHitbox(const sf::FloatRect& hitbox) {
        m_wall_hitbox = hitbox;
    }
    sf::FloatRect BroAI::getWallHitbox() const {
        return m_wall_hitbox;
    }
    void BroAI::setLaunchCount(const int val) {
        m_launch_count = val;
    }
    int BroAI::getLaunchCount() const {
        return m_launch_count;
    }
    void BroAI::setLaunchCounting(const int val) {
        m_launch_counting = val;
    }
    int BroAI::getLaunchCounting() const {
        return m_launch_counting;
    }
    void BroAI::setLaunchIntervalBetween(const float val) {
        m_launch_interval_between = val;
    }
    float BroAI::getLaunchIntervalBetween() const {
        return m_launch_interval_between;
    }
    void BroAI::setLaunchIBTicking(const float val) {
        m_launch_interval_bticking = val;
    }
    float BroAI::getLaunchIBTicking() const {
        return m_launch_interval_bticking;
    }
    void BroAI::setDisabled(const bool val) {
        m_disabled = val;
    }
    bool BroAI::isDisabled() const {
        return m_disabled;
    }
    void BroAI::setType(const BroAIType val) {
        m_type = val;
    }
    BroAIType BroAI::getType() const {
        return m_type;
    }
}