#ifndef MFCPP_NORMALCOINEFFECT_HPP
#define MFCPP_NORMALCOINEFFECT_HPP

#include "cmake-build-minsizerel/_deps/sfml-src/include/SFML/System/Vector2.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Object/Effect.hpp"
#include "Core/Object/Effect/Identity/GenericEffectType.hpp"

class NormalCoinEffect final : public MFCPP::Effect, public MFCPP::GenericEffectType {
public:
    explicit NormalCoinEffect(EffectManager& manager, const sf::Vector2f& position);
    void setPreviousData() override;
    void interpolateData(float alpha) override;
    void statusUpdate(float deltaTime) override;
    void draw() override;
    void Destroy();
private:
    MFCPP::SingleAnimationObject m_animation;
    sf::Vector2f m_velocity;
};

#endif //MFCPP_NORMALCOINEFFECT_HPP
