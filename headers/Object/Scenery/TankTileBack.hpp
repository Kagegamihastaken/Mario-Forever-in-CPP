#ifndef MFCPP_TANKTILEBACK_HPP
#define MFCPP_TANKTILEBACK_HPP
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/Scenery.hpp"
#include "Core/Object/Scenery/Identity/GenericSceneryType.hpp"

class TankTileBack final : public MFCPP::Scenery, public MFCPP::GenericSceneryType {
public:
    explicit TankTileBack(SceneryManager& manager, const sf::Vector2f& position);
    void setPreviousData() override;
    void interpolateData(float alpha) override;
    void draw() override;
private:
    MFCPP::StaticAnimationObject m_animation;
};

#endif //MFCPP_TANKTILEBACK_HPP
