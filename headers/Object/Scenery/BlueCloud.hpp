#ifndef MFCPP_BLUECLOUD_HPP
#define MFCPP_BLUECLOUD_HPP
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Object/Scenery.hpp"
#include "Core/Object/Scenery/Identity/GenericSceneryType.hpp"

class BlueCloud final : public MFCPP::Scenery, public MFCPP::GenericSceneryType {
public:
    explicit BlueCloud(SceneryManager& manager, const sf::Vector2f& position);
    void setPreviousData() override;
    void interpolateData(float alpha) override;
    void draw() override;
private:
    MFCPP::SingleAnimationObject m_animation;
};

#endif //MFCPP_BLUECLOUD_HPP
