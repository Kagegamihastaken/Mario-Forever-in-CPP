#ifndef MFCPP_BLUEGRASS_HPP
#define MFCPP_BLUEGRASS_HPP
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Object/Scenery.hpp"
#include "Core/Object/Scenery/Identity/GenericSceneryType.hpp"

class BlueGrass final : public MFCPP::Scenery, public MFCPP::GenericSceneryType {
public:
    explicit BlueGrass(SceneryManager& manager, const sf::Vector2f& position);
    void setPreviousData() override;
    void interpolateData(float alpha) override;
    void draw() override;
private:
    MFCPP::SingleAnimationObject m_animation;
};

#endif //MFCPP_BLUEGRASS_HPP
