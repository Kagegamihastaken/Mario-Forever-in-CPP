#ifndef MFCPP_REDPLATFORM_HPP
#define MFCPP_REDPLATFORM_HPP

#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/MovingBlock.hpp"
#include "Core/Object/MovingBlock/Identity/PlatformType.hpp"
#include "SFML/System/Vector2.hpp"

class RedPlatform final : public MFCPP::MovingBlock, public MFCPP::PlatformType {
public:
    explicit RedPlatform(MovingBlockManager& manager, const sf::Vector2f& start, const sf::Vector2f& end, float speed, bool smooth, bool fall, bool wait, bool small);
    void setPreviousData() override;
    void interpolateData(float alpha) override;
    void statusUpdate(float deltaTime) override;
    void activate() override;
    void Destroy();
    void draw() override;
private:
    MFCPP::StaticAnimationObject m_animation;
    sf::Vector2f m_end, m_start;
    float m_speed, m_maxSpeed, m_y_velocity, m_smoothStore;
    bool m_smooth, m_fall, m_wait, m_small;
    //Core
    bool m_movingToEnd, m_willFall;
    int m_waitState;
};

#endif //MFCPP_REDPLATFORM_HPP