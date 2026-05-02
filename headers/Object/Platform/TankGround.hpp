#ifndef MFCPP_TANKGROUND_HPP
#define MFCPP_TANKGROUND_HPP

#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/MovingBlock.hpp"
#include "Core/Object/MovingBlock/Identity/PlatformType.hpp"
#include "SFML/System/Vector2.hpp"

class TankGround final : public MFCPP::MovingBlock, public MFCPP::PlatformType {
public:
    explicit TankGround(MovingBlockManager& manager, const sf::Vector2f& pos);
    void setPreviousData() override;
    void interpolateData(float alpha) override;
    void statusUpdate(float deltaTime) override;
    void activate() override;
    void Destroy();
    void draw() override;
private:
    MFCPP::StaticAnimationObject m_animation;
    float m_prev_ViewX;
    bool m_step;
};

#endif //MFCPP_TANKGROUND_HPP