#ifndef MFCPP_RAINEFFECT_HPP
#define MFCPP_RAINEFFECT_HPP

#include <SFML/System/Vector2.hpp>

#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Object/Effect.hpp"
#include "Core/Object/Effect/Identity/ScrollBasedEffectType.hpp"
#include "SFML/Graphics/Rect.hpp"

class RainEffect final : public MFCPP::Effect, public MFCPP::ScrollBasedEffectType {
public:
    explicit RainEffect(EffectManager& manager, const sf::Vector2f& position);
    ~RainEffect() override = default;
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
    void updateState();
private:
    MFCPP::SingleAnimationObject m_animation;
    MFCPP::ActiveObject<float> m_transform;
    sf::FloatRect m_hitbox, m_hitbox_side;
    sf::Vector2f m_velocity;
    bool state, m_platform_collided;
    // false: raining
    // true: hit ground
};

#endif //MFCPP_RAINEFFECT_HPP
