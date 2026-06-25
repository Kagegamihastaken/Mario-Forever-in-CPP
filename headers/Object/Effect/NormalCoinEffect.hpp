#ifndef MFCPP_NORMALCOINEFFECT_HPP
#define MFCPP_NORMALCOINEFFECT_HPP

#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Object/Effect.hpp"
#include "Core/Object/Effect/Identity/GenericEffectType.hpp"

class NormalCoinEffect final : public MFCPP::Effect, public MFCPP::GenericEffectType {
public:
    explicit NormalCoinEffect(EffectManager& manager, const sf::Vector2f& position);
    ~NormalCoinEffect() override = default;
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
    sf::Vector2f m_velocity;
};

#endif //MFCPP_NORMALCOINEFFECT_HPP
