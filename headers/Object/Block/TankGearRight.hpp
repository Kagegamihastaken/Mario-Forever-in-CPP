#ifndef MFCPP_TANKGEARRIGHT_HPP
#define MFCPP_TANKGEARRIGHT_HPP
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Object/CustomTile.hpp"
#include "Core/Object/CustomTile/Identity/SolidType.hpp"

class TankGearRight final : public MFCPP::CustomTile, public MFCPP::SolidType {
public:
    explicit TankGearRight(CustomTileManager& manager, const sf::Vector2f& position);
    void setPreviousData() override;
    void interpolateData(float alpha) override;
    void statusUpdate(float deltaTime) override;
    void HitEvent() override;
    void KickEvent() override;
    void draw() override;
private:
    MFCPP::SingleAnimationObject m_animation;
};

#endif //MFCPP_TANKGEARRIGHT_HPP