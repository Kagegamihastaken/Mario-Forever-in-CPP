#ifndef MFCPP_TANKGEARMIDDLE_HPP
#define MFCPP_TANKGEARMIDDLE_HPP
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Object/CustomTile.hpp"
#include "Core/Object/CustomTile/Identity/SolidType.hpp"

class TankGearMiddle final : public MFCPP::CustomTile, public MFCPP::SolidType {
public:
    explicit TankGearMiddle(CustomTileManager& manager, const sf::Vector2f& position);
    void setPreviousData() override;
    void interpolateData(float alpha) override;
    void statusUpdate(float deltaTime) override;
    void HitEvent() override;
    void KickEvent() override;
    void draw() override;
private:
    MFCPP::SingleAnimationObject m_animation;
};

#endif //MFCPP_TANKGEARMIDDLE_HPP