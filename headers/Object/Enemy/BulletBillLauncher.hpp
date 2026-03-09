#ifndef MFCPP_BULLETBILLLAUNCHER_HPP
#define MFCPP_BULLETBILLLAUNCHER_HPP
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/CustomTile.hpp"
#include "Core/Object/CustomTile/Identity/SolidType.hpp"

class BulletBillLauncher final : public MFCPP::CustomTile, public MFCPP::SolidType {
public:
    explicit BulletBillLauncher(CustomTileManager& manager, const sf::Vector2f& position);
    void setPreviousData() override;
    void interpolateData(float alpha) override;
    void statusUpdate(float deltaTime) override;
    void HitEvent() override;
    void KickEvent() override;
    void draw() override;
private:
    MFCPP::StaticAnimationObject m_animation;
    float m_launch_interval, m_first_shot_time, m_timing;
    int m_random_fire_interval;
    bool m_disabled, m_state;
};

#endif //MFCPP_BULLETBILLLAUNCHER_HPP