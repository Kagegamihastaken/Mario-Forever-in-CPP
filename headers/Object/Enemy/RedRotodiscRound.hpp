#ifndef MFCPP_REDROTODISCROUND_HPP
#define MFCPP_REDROTODISCROUND_HPP

#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/Enemy.hpp"
#include "Core/Object/Enemy/Identity/RotodiscAIType.hpp"
#include "SFML/System/Vector2.hpp"

class RedRotodiscRound final : public MFCPP::Enemy, public MFCPP::RotodiscAIType {
public:
    explicit RedRotodiscRound(EnemyManager& manager, const sf::Vector2f& position, unsigned int arrange_mode, float radius, float speed, float angle);
    ~RedRotodiscRound() override = default;
    void updatePreviousData() override;
    void statusUpdate(float deltaTime) override;
    void MarioCollision(float MarioYVelocity) override;
    void XUpdate(float deltaTime) override;
    void YUpdate(float deltaTime) override;
    void EnemyCollision() override;
    void draw(float alpha) override;
    void Destroy() override;
    void Death(unsigned int state) override;
    void BlockHit() override;
    void ShellHit() override;
    [[nodiscard]] bool isDeath() override;
    void animationUpdate(float deltaTime) override;
    [[nodiscard]] sf::Vector2f getPosition() override;
    [[nodiscard]] sf::Vector2f getOrigin() override;
    [[nodiscard]] sf::FloatRect getHitbox() override;
    [[nodiscard]] bool isDestroyed() override;
private:
    MFCPP::SingleAnimationObject m_animation;
    MFCPP::StaticAnimationObject m_animation_base;
    MFCPP::ActiveObject<float> m_transform;
    sf::FloatRect m_hitbox;
    float m_radius;
    float m_speed;
    sf::Vector2f m_position_center;
    float m_angle;
};

#endif //MFCPP_REDROTODISCROUND_HPP