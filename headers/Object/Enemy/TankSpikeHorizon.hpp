#ifndef MFCPP_TANK_SPIKE_HORIZON_HPP
#define MFCPP_TANK_SPIKE_HORIZON_HPP

#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/Enemy.hpp"
#include "Core/Object/Enemy/Identity/SpikeAIType.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"

class TankSpikeHorizon final : public MFCPP::Enemy, public MFCPP::SpikeAIType {
public:
    explicit TankSpikeHorizon(EnemyManager& manager, const sf::Vector2f& position, bool direction);
    ~TankSpikeHorizon() override = default;
    void updatePreviousData() override;
    void statusUpdate(float deltaTime) override;
    void MarioCollision(float MarioYVelocity) override;
    void XUpdate(float deltaTime) override;
    void YUpdate(float deltaTime) override;
    void EnemyCollision() override;
    void draw(float alpha) override;
    void Destroy() override;
    void Death(unsigned int state) override;
    void BlockHit() override;
    void ShellHit() override;
    [[nodiscard]] bool isDeath() override;
    void animationUpdate(float deltaTime) override;
    [[nodiscard]] sf::Vector2f getPosition() override;
    [[nodiscard]] sf::Vector2f getOrigin() override;
    [[nodiscard]] sf::FloatRect getHitbox() override;
    [[nodiscard]] bool isDestroyed() override;

private:
    MFCPP::StaticAnimationObject m_animation;
    MFCPP::ActiveObject<float> m_transform;
    sf::FloatRect m_hitbox;
};

#endif //MFCPP_TANK_SPIKE_HORIZON_HPP
