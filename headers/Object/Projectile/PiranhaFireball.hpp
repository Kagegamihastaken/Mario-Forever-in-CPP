#ifndef MFCPP_PIRANHAFIREBALL_HPP
#define MFCPP_PIRANHAFIREBALL_HPP
#include "Core/Animate/StaticAnimationObject.hpp"
#include "../../Core/Enumeration/enum.hpp"
#include "Core/Object/Projectile.hpp"
#include "Core/Object/Projectile/Identity/EnemyProjectileType.hpp"
#include "SFML/System/Vector2.hpp"

class PiranhaFireball final : public MFCPP::Projectile, public MFCPP::EnemyProjectileType {
public:
    explicit PiranhaFireball(ProjectileManager& manager, PiranhaDirection direction, const sf::Vector2f& position);
    ~PiranhaFireball() override = default;
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
};

#endif //MFCPP_PIRANHAFIREBALL_HPP