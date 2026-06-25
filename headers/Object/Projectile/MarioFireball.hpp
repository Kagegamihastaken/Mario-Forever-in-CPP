#ifndef MFCPP_MARIOFIREBALL_HPP
#define MFCPP_MARIOFIREBALL_HPP
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/Projectile.hpp"
#include "Core/Object/Projectile/Identity/MarioProjectileType.hpp"
#include "SFML/System/Vector2.hpp"

class MarioFireball final : public MFCPP::Projectile, public MFCPP::MarioProjectileType {
public:
    explicit MarioFireball(ProjectileManager& manager, bool direction, const sf::Vector2f& position);
    ~MarioFireball() override = default;
    void FireballEffect();
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

#endif //MFCPP_MARIOFIREBALL_HPP