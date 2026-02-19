#ifndef MFCPP_HAMMERBRO_HPP
#define MFCPP_HAMMERBRO_HPP
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Object/Enemy/Identity/BroAIType.hpp"
#include "Core/Object/Enemy.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"

class HammerBro final : public MFCPP::Enemy, public MFCPP::BroAIType {
public:
    explicit HammerBro(EnemyManager& manager, const sf::Vector2f& position);
    void setPreviousData() override;
    void interpolateData(float alpha) override;
    void statusUpdate(float deltaTime) override;
    void MarioCollision(float MarioYVelocity) override;
    void XUpdate(float deltaTime) override;
    void YUpdate(float deltaTime) override;
    void EnemyCollision() override;
    void draw() override;
    void Destroy() override;
    void Death(unsigned int state) override;
    void BlockHit() override;
    void ShellHit() override;
    void AnimationUpdate(bool val);
    [[nodiscard]] bool isDeath() override;
private:
    MFCPP::SingleAnimationObject m_animation;
    sf::Vector2f m_velocity;
    sf::FloatRect m_wall_hitbox;
    short unsigned m_state;
    float m_movingValue, m_timeTicking, m_tickingValue, m_stop_duration, m_randomWalking;
    unsigned int m_WalkingState;
    float m_RNG_walking_range, m_RNG_ticking_range;
    bool m_isFalling;
    float m_timeCounter, m_lastY;
    // for launching
    float m_launchTickingTime, m_LaunchInterval, m_launchIntervalTicking, m_launchRNG, m_launchWaitTime;
    int m_launchCount, m_launchCounting;
    float m_launchIntervalBetween, m_launchIntervalBetweenTicking;

    bool m_lastIsLaunching, m_IsLaunching;
};

#endif //MFCPP_HAMMERBRO_HPP