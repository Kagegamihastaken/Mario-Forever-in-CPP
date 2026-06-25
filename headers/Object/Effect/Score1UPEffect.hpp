#ifndef MFCPP_SCORE1UPEFFECT_HPP
#define MFCPP_SCORE1UPEFFECT_HPP
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/Effect.hpp"
#include "Core/Object/Effect/Identity/ScrollBasedEffectType.hpp"
#include <SFML/System/Vector2.hpp>

class Score1UPEffect : public MFCPP::Effect, public MFCPP::ScrollBasedEffectType {
public:
    explicit Score1UPEffect(EffectManager& manager, const sf::Vector2f& position);
    ~Score1UPEffect() override = default;
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

#endif //MFCPP_SCORE1UPEFFECT_HPP
