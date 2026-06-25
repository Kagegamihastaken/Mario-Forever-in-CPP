#ifndef MFCPP_FIREFLOWER_HPP
#define MFCPP_FIREFLOWER_HPP

#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Object/Enemy.hpp"
#include "Core/Object/Enemy/Identity/GoombaAIType.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"

class FireFlower final : public MFCPP::Enemy, public MFCPP::GoombaAIType {
public:
    explicit FireFlower(EnemyManager& manager, const sf::Vector2f& position);
    ~FireFlower() override = default;
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
    [[nodiscard]] bool isDestroyed() override;
    [[nodiscard]] sf::Vector2f getPosition() override;
    [[nodiscard]] sf::Vector2f getOrigin() override;
    [[nodiscard]] sf::FloatRect getHitbox() override;

private:
    MFCPP::SingleAnimationObject m_animation;
    MFCPP::ActiveObject<float> m_transform;
    sf::Vector2f m_velocity;
    sf::FloatRect m_wall_hitbox;
    sf::FloatRect m_hitbox;
    float m_appearingSpeed;
    float m_appearingY;
    float m_appearingYMax;
    bool m_isappearing;
};
#endif //MFCPP_FIREFLOWER_HPP