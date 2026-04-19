#ifndef MFCPP_SCORE5000EFFECT_HPP
#define MFCPP_SCORE5000EFFECT_HPP
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/Effect.hpp"
#include "Core/Object/Effect/Identity/ScrollBasedEffectType.hpp"
#include <SFML/System/Vector2.hpp>

class Score5000Effect : public MFCPP::Effect, public MFCPP::ScrollBasedEffectType {
public:
    explicit Score5000Effect(EffectManager& manager, const sf::Vector2f& position);
    void setPreviousData() override;
    void interpolateData(float alpha) override;
    void statusUpdate(float deltaTime) override;
    void draw() override;
    void Destroy();
private:
    MFCPP::StaticAnimationObject m_animation;
    sf::Vector2f m_velocity;
    float m_alpha;
};

#endif //MFCPP_SCORE5000EFFECT_HPP
