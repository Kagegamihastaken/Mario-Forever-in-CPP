#ifndef MFCPP_BULLETBILL_HPP
#define MFCPP_BULLETBILL_HPP

#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/Enemy/Identity/BulletBillAIType.hpp"
#include "Core/Object/Enemy.hpp"

class BulletBill final : public MFCPP::Enemy, public MFCPP::BulletBillAIType {
public:
    explicit BulletBill(EnemyManager& manager, const sf::Vector2f& position, float speed, bool direction);
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
    unsigned int m_state;
};

#endif //MFCPP_BULLETBILL_HPP