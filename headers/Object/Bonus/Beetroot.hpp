#ifndef MFCPP_BEETROOT_HPP
#define MFCPP_BEETROOT_HPP

#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Object/Enemy.hpp"
#include "Core/Object/Enemy/Identity/GoombaAIType.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"

class Beetroot final : public MFCPP::Enemy, public MFCPP::GoombaAIType {
public:
    explicit Beetroot(EnemyManager& manager, const sf::Vector2f& position);
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
    sf::Vector2f m_velocity;
    sf::FloatRect m_wall_hitbox;
    float m_appearingSpeed;
    float m_appearingY;
    float m_appearingYMax;
    bool m_isappearing;
};
#endif //MFCPP_BEETROOT_HPP