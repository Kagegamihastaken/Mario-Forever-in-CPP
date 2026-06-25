#ifndef MFCPP_SCORE500EFFECT_HPP
#define MFCPP_SCORE500EFFECT_HPP
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/Effect.hpp"
#include "Core/Object/Effect/Identity/ScrollBasedEffectType.hpp"
#include <SFML/System/Vector2.hpp>

class Score500Effect : public MFCPP::Effect, public MFCPP::ScrollBasedEffectType {
public:
    explicit Score500Effect(EffectManager& manager, const sf::Vector2f& position);
    ~Score500Effect() override = default;
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
    MFCPP::StaticAnimationObject m_animation;
    MFCPP::ActiveObject<float> m_transform;
    sf::FloatRect m_hitbox;
    sf::Vector2f m_velocity;
    float m_alpha;
};

#endif //MFCPP_SCORE500EFFECT_HPP
