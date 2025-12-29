#ifndef MFCPP_GREENMUSHROOM_HPP
#define MFCPP_GREENMUSHROOM_HPP

#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/Enemy.hpp"
#include "Core/Object/GoombaAIType.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"

class GreenMushroom final : public MFCPP::Enemy, public MFCPP::GoombaAIType {
public:
    explicit GreenMushroom(EnemyManager& manager, const sf::Vector2f& position);
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
    MFCPP::StaticAnimationObject m_animation;
    sf::Vector2f m_velocity;
    sf::FloatRect m_wall_hitbox;
    float m_appearingSpeed;
    float m_appearingY;
    float m_appearingYMax;
    bool m_isappearing;
};
#endif //MFCPP_MUSHROOM_HPP