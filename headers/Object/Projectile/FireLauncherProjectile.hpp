#ifndef MFCPP_FIRELAUNCHERPROJECTILE_HPP
#define MFCPP_FIRELAUNCHERPROJECTILE_HPP
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/Object/Projectile.hpp"
#include "Core/Object/Projectile/Identity/EnemyProjectileType.hpp"
#include "SFML/System/Vector2.hpp"

class FireLauncherProjectile final : public MFCPP::Projectile, public MFCPP::EnemyProjectileType {
public:
    explicit FireLauncherProjectile(ProjectileManager& manager, const sf::Vector2f& position, const sf::Vector2f& velocity);
    void FireballEffect() const;
    void setPreviousData() override;
    void interpolateData(float alpha) override;
    void statusUpdate(float deltaTime) override;
    void CollisionUpdate() override;
    void LevelEndCleanup() override;
    void Destroy();
    void draw() override;
private:
    MFCPP::StaticAnimationObject m_animation;
    sf::Vector2f m_velocity;
    float m_timePass, m_timePassLimit;
};

#endif //MFCPP_FIRELAUNCHERPROJECTILE_HPP