#ifndef MFCPP_TANKGEARLEFT_HPP
#define MFCPP_TANKGEARLEFT_HPP
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Object/CustomTile.hpp"
#include "Core/Object/CustomTile/Identity/SolidType.hpp"

class TankGearLeft final : public MFCPP::CustomTile, public MFCPP::SolidType {
public:
    explicit TankGearLeft(CustomTileManager& manager, const sf::Vector2f& position);
    ~TankGearLeft() override = default;
    void setPreviousData() override;
    void interpolateData(float alpha) override;
    void statusUpdate(float deltaTime) override;
    void HitEvent() override;
    void KickEvent() override;
    void draw() override;
    void animationUpdate(float deltaTime) override;
private:
    MFCPP::SingleAnimationObject m_animation;
};

#endif //MFCPP_TANKGEARLEFT_HPP