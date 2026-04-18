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
    void setPreviousData() override;
    void interpolateData(const float alpha) override;
    void statusUpdate(const float deltaTime) override;
    void draw() override;
    void Destroy();
private:
    MFCPP::SingleAnimationObject m_animation;
};

#endif //MFCPP_FIREBALLEXPLOSIONEFFECT_HPP
