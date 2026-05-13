#ifndef MFCPP_FIRELAUNCHERSTAND_HPP
#define MFCPP_FIRELAUNCHERSTAND_HPP
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/CustomTile.hpp"
#include "Core/Object/CustomTile/Identity/SolidType.hpp"

class FireLauncherStand final : public MFCPP::CustomTile, public MFCPP::SolidType {
public:
    explicit FireLauncherStand(CustomTileManager& manager, const sf::Vector2f& position);
    void setPreviousData() override;
    void interpolateData(float alpha) override;
    void statusUpdate(float deltaTime) override;
    void HitEvent() override;
    void KickEvent() override;
    void draw() override;
private:
    MFCPP::StaticAnimationObject m_animation;
    float m_FireInterval, m_FireIntervalCounting;
    int m_ProjectileCount, m_ProjectileCounting;
    float m_FireBetweenInterval, m_FireBetweenIntervalCounting;
    bool m_playSound;
};

#endif //MFCPP_FIRELAUNCHERSTAND_HPP