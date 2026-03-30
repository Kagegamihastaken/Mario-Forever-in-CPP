#ifndef MFCPP_PIRANHAFIREBALL_HPP
#define MFCPP_PIRANHAFIREBALL_HPP
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/Object/Projectile.hpp"
#include "Core/Object/Projectile/Identity/EnemyProjectileType.hpp"
#include "SFML/System/Vector2.hpp"

class PiranhaFireball final : public MFCPP::Projectile, public MFCPP::EnemyProjectileType {
public:
    explicit PiranhaFireball(ProjectileManager& manager, PiranhaDirection direction, const sf::Vector2f& position);
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
};

#endif //MFCPP_PIRANHAFIREBALL_HPP