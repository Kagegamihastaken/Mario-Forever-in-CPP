#ifndef MFCPP_SCORE100EFFECT_HPP
#define MFCPP_SCORE100EFFECT_HPP
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/Effect.hpp"
#include "Core/Object/Effect/Identity/ScrollBasedEffectType.hpp"
#include <SFML/System/Vector2.hpp>

class Score100Effect : public MFCPP::Effect, public MFCPP::ScrollBasedEffectType {
public:
    explicit Score100Effect(EffectManager& manager, const sf::Vector2f& position);
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

#endif //MFCPP_SCORE100EFFECT_HPP
