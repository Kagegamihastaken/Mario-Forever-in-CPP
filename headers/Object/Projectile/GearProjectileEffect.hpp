#ifndef MFCPP_GEARPROJECTILEEFFECT_HPP
#define MFCPP_GEARPROJECTILEEFFECT_HPP
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/Projectile.hpp"
#include "Core/Object/Projectile/Identity/EnemyProjectileType.hpp"
#include "SFML/System/Vector2.hpp"

class GearProjectileEffect final : public MFCPP::Projectile, public MFCPP::EnemyProjectileType {
public:
    explicit GearProjectileEffect(ProjectileManager& manager, const sf::Vector2f& position, const sf::Angle& angle, bool dir);
    ~GearProjectileEffect() override = default;
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
    float m_alpha;
    bool m_dir;
};

#endif //MFCPP_GEARPROJECTILEEFFECT_HPP