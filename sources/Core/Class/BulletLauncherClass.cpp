#include "Core/Class/BulletLauncherClass.hpp"

namespace MFCPP {
    BulletLauncher::BulletLauncher(const float launch_interval, const sf::Vector2f& pos) : m_launch_interval(launch_interval) {
        setCurrentPosition(pos);
        setPreviousPosition(pos);
        setInterpolatedPosition(pos);
        setOrigin(sf::Vector2f(16.f, 31.f));

        m_random_fire_interval = 149;
        m_first_shot_time = 25.f;
        m_timing = 0.f;
        m_disabled = false;
        m_state = false;
    }
    void BulletLauncher::setLaunchInterval(const float val) {
        m_launch_interval = val;
    }
    float BulletLauncher::getLaunchInterval() const {
        return m_launch_interval;
    }
    void BulletLauncher::setFirstShotTime(const float val) {
        m_first_shot_time = val;
    }
    float BulletLauncher::getFirstShotTime() const {
        return m_first_shot_time;
    }
    void BulletLauncher::setRandomFireInterval(const int val) {
        m_random_fire_interval = val;
    }
    int BulletLauncher::getRandomFireInterval() const {
        return m_random_fire_interval;
    }
    void BulletLauncher::setTiming(const float val) {
        m_timing = val;
    }
    float BulletLauncher::getTiming() const {
        return m_timing;
    }
    void BulletLauncher::setDisabled(const bool val) {
        m_disabled = val;
    }
    bool BulletLauncher::isDisabled() const {
        return m_disabled;
    }
    void BulletLauncher::setState(const bool val) {
        m_state = val;
    }
    bool BulletLauncher::getState() const {
        return m_state;
    }
}
