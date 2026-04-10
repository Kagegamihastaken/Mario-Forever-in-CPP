#ifndef MFCPP_REDPLATFORM_HPP
#define MFCPP_REDPLATFORM_HPP

#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/MovingBlock.hpp"
#include "Core/Object/MovingBlock/Identity/PlatformType.hpp"
#include "SFML/System/Vector2.hpp"

class RedPlatform final : public MFCPP::MovingBlock, public MFCPP::PlatformType {
public:
    explicit RedPlatform(MovingBlockManager& manager, const sf::Vector2f& start, const sf::Vector2f& end, float speed, bool smooth, bool fall, bool wait);
    void setPreviousData() override;
    void interpolateData(float alpha) override;
    void statusUpdate(float deltaTime) override;
    void draw() override;
private:
    MFCPP::StaticAnimationObject m_animation;
    sf::Vector2f m_velocity;
};

#endif //MFCPP_REDPLATFORM_HPP