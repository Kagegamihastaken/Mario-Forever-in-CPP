#ifndef MFCPP_GEARLAUNCHER_HPP
#define MFCPP_GEARLAUNCHER_HPP
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/CustomTile.hpp"
#include "Core/Object/CustomTile/Identity/SolidType.hpp"

class GearLauncher final : public MFCPP::CustomTile, public MFCPP::SolidType {
public:
    explicit GearLauncher(CustomTileManager& manager, const sf::Vector2f& position);
    ~GearLauncher() override = default;
    void updatePreviousData() override;
    void statusUpdate(float deltaTime) override;
    void HitEvent() override;
    void KickEvent() override;
    void draw(float alpha) override;
    void animationUpdate(float deltaTime) override;
    [[nodiscard]] sf::Vector2f getPosition() override;
    [[nodiscard]] sf::Vector2f getOrigin() override;
    [[nodiscard]] sf::FloatRect getHitbox() override;
    [[nodiscard]] bool isDestroyed() override;

private:
    MFCPP::StaticAnimationObject m_animation;
    MFCPP::ActiveObject<float> m_transform;
    sf::FloatRect m_hitbox;
    float m_timePass;
    float m_timePassLimit;
};

#endif //MFCPP_GEARLAUNCHER_HPP