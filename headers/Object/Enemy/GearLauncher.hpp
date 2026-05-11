#ifndef MFCPP_GEARLAUNCHER_HPP
#define MFCPP_GEARLAUNCHER_HPP
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/CustomTile.hpp"
#include "Core/Object/CustomTile/Identity/SolidType.hpp"

class GearLauncher final : public MFCPP::CustomTile, public MFCPP::SolidType {
public:
    explicit GearLauncher(CustomTileManager& manager, const sf::Vector2f& position);
    void setPreviousData() override;
    void interpolateData(float alpha) override;
    void statusUpdate(float deltaTime) override;
    void HitEvent() override;
    void KickEvent() override;
    void draw() override;
private:
    MFCPP::StaticAnimationObject m_animation;
};

#endif //MFCPP_GEARLAUNCHER_HPP