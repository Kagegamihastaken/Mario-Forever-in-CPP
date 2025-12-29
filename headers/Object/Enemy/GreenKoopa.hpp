#ifndef MFCPP_GREENKOOPA_HPP
#define MFCPP_GREENKOOPA_HPP

#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Object/Enemy.hpp"
#include "Core/Object/GoombaAIType.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"

class GreenKoopa final : public MFCPP::Enemy, public MFCPP::GoombaAIType {
public:
    explicit GreenKoopa(EnemyManager& manager, const sf::Vector2f& position, bool isShell);
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
    void ChangeState();
    [[nodiscard]] bool isDeath() override;
private:
    MFCPP::SingleAnimationObject m_animation;
    sf::Vector2f m_velocity;
    sf::FloatRect m_wall_hitbox;
    short unsigned m_state;
    float m_invincibleTimer;
    short unsigned m_hit_count;
};
#endif //MFCPP_GREENKOOPA_HPP