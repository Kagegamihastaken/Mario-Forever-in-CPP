#ifndef MFCPP_REDROTODISCFLOWER_HPP
#define MFCPP_REDROTODISCFLOWER_HPP

#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/Enemy.hpp"
#include "Core/Object/Enemy/Identity/RotodiscAIType.hpp"
#include "SFML/System/Vector2.hpp"

class RedRotodiscFlower final : public MFCPP::Enemy, public MFCPP::RotodiscAIType {
public:
    explicit RedRotodiscFlower(EnemyManager& manager, const sf::Vector2f& position, unsigned int arrange_mode, float radius, float speed, float speed_x_change, float angle);
    ~RedRotodiscFlower() override = default;
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
    float m_max_radius;
    bool m_change_state;
    float m_speed;
    float m_speed_x_change;
    sf::Vector2f m_position_center;
    float m_angle;
};

#endif //MFCPP_REDROTODISCFLOWER_HPP