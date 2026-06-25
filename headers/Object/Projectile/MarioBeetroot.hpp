#ifndef MFCPP_MARIOBEETROOT_HPP
#define MFCPP_MARIOBEETROOT_HPP
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/Projectile.hpp"
#include "Core/Object/Projectile/Identity/MarioProjectileType.hpp"
#include "SFML/System/Vector2.hpp"

class MarioBeetroot final : public MFCPP::Projectile, public MFCPP::MarioProjectileType {
public:
    explicit MarioBeetroot(ProjectileManager& manager, bool direction, const sf::Vector2f& position);
    ~MarioBeetroot() override = default;
    void spin(bool flip, bool sound);
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
    int m_hit_count;
    sf::Vector2f m_velocity;
};

#endif //MFCPP_MARIOBEETROOT_HPP