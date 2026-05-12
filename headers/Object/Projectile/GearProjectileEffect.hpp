#ifndef MFCPP_GEARPROJECTILEEFFECT_HPP
#define MFCPP_GEARPROJECTILEEFFECT_HPP
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/Projectile.hpp"
#include "Core/Object/Projectile/Identity/EnemyProjectileType.hpp"
#include "SFML/System/Vector2.hpp"

class GearProjectileEffect final : public MFCPP::Projectile, public MFCPP::EnemyProjectileType {
public:
    explicit GearProjectileEffect(ProjectileManager& manager, const sf::Vector2f& position, const sf::Angle& angle, bool dir);
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
    float m_alpha;
    bool m_dir;
};

#endif //MFCPP_GEARPROJECTILEEFFECT_HPP