#ifndef MFCPP_GEARLAUNCHERFLIPPED_HPP
#define MFCPP_GEARLAUNCHERFLIPPED_HPP
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/CustomTile.hpp"
#include "Core/Object/CustomTile/Identity/SolidType.hpp"

class GearLauncherFlipped final : public MFCPP::CustomTile, public MFCPP::SolidType {
public:
    explicit GearLauncherFlipped(CustomTileManager& manager, const sf::Vector2f& position);
    void setPreviousData() override;
    void interpolateData(float alpha) override;
    void statusUpdate(float deltaTime) override;
    void HitEvent() override;
    void KickEvent() override;
    void draw() override;
private:
    MFCPP::StaticAnimationObject m_animation;
    float m_timePass;
    float m_timePassLimit;
};

#endif //MFCPP_GEARLAUNCHERFLIPPED_HPP