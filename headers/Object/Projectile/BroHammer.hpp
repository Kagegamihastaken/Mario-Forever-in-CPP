#ifndef MFCPP_BROHAMMER_HPP
#define MFCPP_BROHAMMER_HPP
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/Projectile.hpp"
#include "Core/Object/Projectile/Identity/EnemyProjectileType.hpp"
#include "SFML/System/Vector2.hpp"

class BroHammer final : public MFCPP::Projectile, public MFCPP::EnemyProjectileType {
public:
    explicit BroHammer(ProjectileManager& manager, bool direction, const sf::Vector2f& position);
    ~BroHammer() override = default;
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
    bool m_direction;
    sf::Vector2f m_velocity;
};

#endif //MFCPP_BROHAMMER_HPP