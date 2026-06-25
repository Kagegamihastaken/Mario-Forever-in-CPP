#ifndef MFCPP_GEARPROJECTILE_HPP
#define MFCPP_GEARPROJECTILE_HPP
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/Projectile.hpp"
#include "Core/Object/Projectile/Identity/EnemyProjectileType.hpp"
#include "SFML/System/Vector2.hpp"

class GearProjectile final : public MFCPP::Projectile, public MFCPP::EnemyProjectileType {
public:
    explicit GearProjectile(ProjectileManager& manager, const sf::Vector2f& position, const sf::Vector2f& velocity);
    ~GearProjectile() override = default;
    void FireballEffect() const;
    void updatePreviousData() override;
    void statusUpdate(float deltaTime) override;
    void CollisionUpdate() override;
    void LevelEndCleanup() override;
    void Destroy();
    void draw(float alpha) override;
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
    float m_timeEffect, m_timeEffectMax;
};

#endif //MFCPP_GEARPROJECTILE_HPP