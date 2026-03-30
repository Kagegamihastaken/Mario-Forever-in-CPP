#ifndef MFCPP_BROHAMMER_HPP
#define MFCPP_BROHAMMER_HPP
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/Projectile.hpp"
#include "Core/Object/Projectile/Identity/EnemyProjectileType.hpp"
#include "SFML/System/Vector2.hpp"

class BroHammer final : public MFCPP::Projectile, public MFCPP::EnemyProjectileType {
public:
    explicit BroHammer(ProjectileManager& manager, bool direction, const sf::Vector2f& position);
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
    bool m_direction;
    sf::Vector2f m_velocity;
};

#endif //MFCPP_BROFIREBALL_HPP