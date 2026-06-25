#ifndef MFCPP_TANKGEARMIDDLE_HPP
#define MFCPP_TANKGEARMIDDLE_HPP
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Object/CustomTile.hpp"
#include "Core/Object/CustomTile/Identity/SolidType.hpp"

class TankGearMiddle final : public MFCPP::CustomTile, public MFCPP::SolidType {
public:
    explicit TankGearMiddle(CustomTileManager& manager, const sf::Vector2f& position);
    ~TankGearMiddle() override = default;
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
    MFCPP::SingleAnimationObject m_animation;
    MFCPP::ActiveObject<float> m_transform;
    sf::FloatRect m_hitbox;
};

#endif //MFCPP_TANKGEARMIDDLE_HPP