#ifndef MFCPP_NORMALCOINBRICK_HPP
#define MFCPP_NORMALCOINBRICK_HPP

#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/CustomTile.hpp"
#include "Core/Object/CustomTile/Identity/BonusType.hpp"
#include "SFML/System/Vector2.hpp"

class NormalCoinBrick final : public MFCPP::CustomTile, public MFCPP::BonusType {
public:
    explicit NormalCoinBrick(CustomTileManager& manager, const sf::Vector2f& position);
    void setPreviousData() override;
    void interpolateData(float alpha) override;
    void statusUpdate(float deltaTime) override;
    void HitEvent() override;
    void KickEvent() override;
    void draw() override;
private:
    void Break();
    void Hit();
    MFCPP::StaticAnimationObject m_animation;
    float m_state_count, m_ypos, m_time, m_time_limit;
    bool m_state, m_updown, m_hitted, m_disabled;
};

#endif //MFCPP_NORMALCOINBRICK_HPP