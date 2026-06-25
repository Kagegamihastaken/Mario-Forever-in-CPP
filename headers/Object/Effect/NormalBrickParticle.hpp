#ifndef MFCPP_NORMALBRICKPARTICLE_HPP
#define MFCPP_NORMALBRICKPARTICLE_HPP

#include <SFML/System/Vector2.hpp>
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/Effect.hpp"
#include "Core/Object/Effect/Identity/GenericEffectType.hpp"

class NormalBrickParticle final : public MFCPP::Effect, public MFCPP::GenericEffectType {
public:
    explicit NormalBrickParticle(EffectManager& manager, const sf::Vector2f& velocity, const sf::Vector2f& position);
    ~NormalBrickParticle() override = default;
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
    bool m_direction;
};

#endif //MFCPP_NORMALBRICKPARTICLE_HPP
