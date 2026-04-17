#ifndef MFCPP_NORMALBRICKPARTICLE_HPP
#define MFCPP_NORMALBRICKPARTICLE_HPP

#include <SFML/System/Vector2.hpp>
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/Effect.hpp"
#include "Core/Object/Effect/Identity/GenericEffectType.hpp"

class NormalBrickParticle final : public MFCPP::Effect, public MFCPP::GenericEffectType {
public:
    explicit NormalBrickParticle(EffectManager& manager, const sf::Vector2f& velocity, const sf::Vector2f& position);
    void setPreviousData() override;
    void interpolateData(float alpha) override;
    void statusUpdate(float deltaTime) override;
    void draw() override;
    void Destroy();
private:
    MFCPP::StaticAnimationObject m_animation;
    sf::Vector2f m_velocity;
    bool m_direction;
};

#endif //MFCPP_NORMALBRICKPARTICLE_HPP
