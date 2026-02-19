#ifndef MFCPP_GREENPIRANHAGROUND_HPP
#define MFCPP_GREENPIRANHAGROUND_HPP

#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Object/Enemy.hpp"
#include "Core/Object/Enemy/Identity/SpikeAIType.hpp"
#include "SFML/System/Vector2.hpp"

class GreenPiranhaGround final : public MFCPP::Enemy, public MFCPP::SpikeAIType {
public:
    explicit GreenPiranhaGround(EnemyManager& manager, const sf::Vector2f& position);
    void setPreviousData() override;
    void interpolateData(float alpha) override;
    void statusUpdate(float deltaTime) override;
    void MarioCollision(float MarioYVelocity) override;
    void XUpdate(float deltaTime) override;
    void YUpdate(float deltaTime) override;
    void EnemyCollision() override;
    void draw() override;
    void Destroy() override;
    void Death(unsigned int state) override;
    void BlockHit() override;
    void ShellHit() override;
    [[nodiscard]] bool isDeath() override;
private:
    MFCPP::SingleAnimationObject m_animation;
};

#endif //MFCPP_GREENPIRANHAGROUND_HPP
