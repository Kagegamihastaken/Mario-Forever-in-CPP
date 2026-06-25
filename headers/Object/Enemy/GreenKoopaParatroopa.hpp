#ifndef MFCPP_GREENKOOPAPARATROOPA_HPP
#define MFCPP_GREENKOOPAPARATROOPA_HPP

#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Object/Enemy.hpp"
#include "Core/Object/Enemy/Identity/GoombaAIType.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"

class GreenKoopaParatroopa final : public MFCPP::Enemy, public MFCPP::GoombaAIType {
public:
    explicit GreenKoopaParatroopa(EnemyManager& manager, const sf::Vector2f& position, float angle);
    ~GreenKoopaParatroopa() override = default;
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
    void ChangeState();
    [[nodiscard]] bool isDeath() override;
    void animationUpdate(float deltaTime) override;
    [[nodiscard]] sf::Vector2f getPosition() override;
    [[nodiscard]] sf::Vector2f getOrigin() override;
    [[nodiscard]] sf::FloatRect getHitbox() override;
    [[nodiscard]] bool isDestroyed() override;
private:
    MFCPP::SingleAnimationObject m_animation;
    MFCPP::ActiveObject<float> m_transform;
    sf::FloatRect m_hitbox;
    sf::Vector2f m_velocity;
    sf::Vector2f m_origin_pos;
    sf::Vector2f m_max_radius;
    float m_angle, m_speed;
    sf::FloatRect m_wall_hitbox;
    short unsigned m_state;
};
#endif //MFCPP_GREENKOOPAPARATROOPA_HPP