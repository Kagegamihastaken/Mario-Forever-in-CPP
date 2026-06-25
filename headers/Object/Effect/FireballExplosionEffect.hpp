//
// Created by dovie on 4/18/2026.
//

#ifndef MFCPP_FIREBALLEXPLOSIONEFFECT_HPP
#define MFCPP_FIREBALLEXPLOSIONEFFECT_HPP
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Object/Effect.hpp"
#include "Core/Object/Effect/Identity/GenericEffectType.hpp"

class FireballExplosionEffect final : public MFCPP::Effect, public MFCPP::GenericEffectType {
public:
    explicit FireballExplosionEffect(EffectManager& manager, const sf::Vector2f& position);
    ~FireballExplosionEffect() override = default;
    void updatePreviousData() override;
    void statusUpdate(float deltaTime) override;
    void draw(float alpha) override;
    void Destroy();
    void animationUpdate(float deltaTime) override;
    [[nodiscard]] sf::Vector2f getPosition() override;
    [[nodiscard]] sf::Vector2f getOrigin() override;
    [[nodiscard]] sf::FloatRect getHitbox() override;
    [[nodiscard]] bool isDestroyed() override;
    void teleport(sf::Vector2f val) override;
private:
    MFCPP::SingleAnimationObject m_animation;
    MFCPP::ActiveObject<float> m_transform;
    sf::FloatRect m_hitbox;
};

#endif //MFCPP_FIREBALLEXPLOSIONEFFECT_HPP
