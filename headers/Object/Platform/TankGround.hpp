#ifndef MFCPP_TANKGROUND_HPP
#define MFCPP_TANKGROUND_HPP

#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/MovingBlock.hpp"
#include "Core/Object/MovingBlock/Identity/PlatformType.hpp"
#include "SFML/System/Vector2.hpp"

class TankGround final : public MFCPP::MovingBlock, public MFCPP::PlatformType {
public:
    explicit TankGround(MovingBlockManager& manager, const sf::Vector2f& pos);
    ~TankGround() override = default;
    void updatePreviousData() override;
    void statusUpdate(float deltaTime) override;
    void activate() override;
    void Destroy();
    void draw(float alpha) override;
    void animationUpdate(float deltaTime) override;
    [[nodiscard]] sf::Vector2f getPosition() override;
    [[nodiscard]] sf::Vector2f getOrigin() override;
    [[nodiscard]] sf::FloatRect getHitbox() override;
    [[nodiscard]] sf::Vector2f getPreviousPosition() override;
    [[nodiscard]] bool isDestroyed() override;
private:
    MFCPP::StaticAnimationObject m_animation;
    MFCPP::ActiveObject<float> m_transform;
    sf::FloatRect m_hitbox;
    float m_prev_ViewX;
    bool m_step;
};

#endif //MFCPP_TANKGROUND_HPP