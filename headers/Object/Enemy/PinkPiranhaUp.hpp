#ifndef MFCPP_PINKPIRANHAUP_HPP
#define MFCPP_PINKPIRANHAUP_HPP

#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Object/Enemy.hpp"
#include "Core/Object/Enemy/Identity/PiranhaAIType.hpp"
#include "SFML/System/Vector2.hpp"

class PinkPiranhaUp final : public MFCPP::Enemy, public MFCPP::PiranhaAIType {
public:
    explicit PinkPiranhaUp(EnemyManager& manager, const sf::Vector2f& position);
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
    [[nodiscard]] bool isDeath() override;
private:
    MFCPP::SingleAnimationObject m_animation;
    float m_stop_clock, m_stop_time, m_speed;
    float m_position_limit, m_position_moving;
    bool m_moving_state, m_moving_stop;
    float m_distance_appear;

    int m_fire_counting, m_fire_count;
    float m_fire_ticking, m_fire_interval;
};

#endif //MFCPP_PINKPIRANHAUP_HPP
