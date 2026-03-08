#ifndef MFCPP_NORMALBRICK_HPP
#define MFCPP_NORMALBRICK_HPP

#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/CustomTile.hpp"
#include "Core/Object/CustomTile/Identity/BonusType.hpp"
#include "SFML/System/Vector2.hpp"

class NormalBrick final : public MFCPP::CustomTile, public MFCPP::BonusType {
public:
    explicit NormalBrick(CustomTileManager& manager, const sf::Vector2f& position);
    void setPreviousData() override;
    void interpolateData(float alpha) override;
    void statusUpdate(float deltaTime) override;
    void HitEvent() override;
    void KickEvent() override;
    void draw() override;
private:
    void Break();
    MFCPP::StaticAnimationObject m_animation;
    float m_state_count, m_ypos;
    bool m_state, m_updown;
};

#endif //MFCPP_NORMALBRICK_HPP