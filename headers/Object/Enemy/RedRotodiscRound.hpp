#ifndef MFCPP_REDROTODISCROUND_HPP
#define MFCPP_REDROTODISCROUMD_HPP

#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/Enemy.hpp"
#include "Core/Object/RotodiscAIType.hpp"
#include "SFML/System/Vector2.hpp"

class RedRotodiscRound final : public MFCPP::Enemy, public MFCPP::RotodiscAIType {
public:
    explicit RedRotodiscRound(EnemyManager& manager, const sf::Vector2f& position, unsigned int arrange_mode, float radius, float speed);
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
    MFCPP::StaticAnimationObject m_animation_base;
    unsigned int m_arrange_mode;
    float m_radius;
    float m_speed;
    sf::Vector2f m_position_center;
    float m_angle;
};

#endif //MFCPP_REDROTODISCROUND_HPP