#ifndef MFCPP_CANNON_BULLET_HPP
#define MFCPP_CANNON_BULLET_HPP

#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/Enemy/Identity/BulletBillAIType.hpp"
#include "Core/Object/Enemy.hpp"

class CannonBullet final : public MFCPP::Enemy, public MFCPP::BulletBillAIType {
public:
    explicit CannonBullet(EnemyManager& manager, const sf::Vector2f& position, const sf::Vector2f& velocity, bool direction);
    ~CannonBullet() override = default;
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
    MFCPP::StaticAnimationObject m_animation;
    MFCPP::ActiveObject<float> m_transform;
    sf::FloatRect m_hitbox;
    sf::Vector2f m_velocity;
    float m_timePass;
    unsigned int m_state;
};

#endif //MFCPP_CANNON_BULLET_HPP