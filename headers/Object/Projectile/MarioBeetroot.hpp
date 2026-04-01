#ifndef MFCPP_MARIOBEETROOT_HPP
#define MFCPP_MARIOBEETROOT_HPP
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/Projectile.hpp"
#include "Core/Object/Projectile/Identity/MarioProjectileType.hpp"
#include "SFML/System/Vector2.hpp"

class MarioBeetroot final : public MFCPP::Projectile, public MFCPP::MarioProjectileType {
public:
    explicit MarioBeetroot(ProjectileManager& manager, bool direction, const sf::Vector2f& position);
    void spin(bool flip, bool sound);
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
    int m_hit_count;
    sf::Vector2f m_velocity;
};

#endif //MFCPP_MARIOBEETROOT_HPP