#ifndef MFCPP_LUCKYBLOCKCOIN_HPP
#define MFCPP_LUCKYBLOCKCOIN_HPP

#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Object/CustomTile.hpp"
#include "Core/Object/CustomTile/Identity/BonusType.hpp"
#include "SFML/System/Vector2.hpp"

class LuckyBlockCoin final : public MFCPP::CustomTile, public MFCPP::BonusType {
public:
    explicit LuckyBlockCoin(CustomTileManager& manager, const sf::Vector2f& position);
    void setPreviousData() override;
    void interpolateData(float alpha) override;
    void statusUpdate(float deltaTime) override;
    void HitEvent() override;
    void KickEvent() override;
    void draw() override;
private:
    void Break();
    void Hit();
    MFCPP::SingleAnimationObject m_animation;
    float m_state_count, m_ypos;
    bool m_state, m_updown, m_disabled;
};

#endif //MFCPP_LUCKYBLOCKCOIN_HPP