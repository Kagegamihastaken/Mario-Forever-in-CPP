#ifndef MFCPP_TANKGEARLEFT_HPP
#define MFCPP_TANKGEARLEFT_HPP
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Object/CustomTile.hpp"
#include "Core/Object/CustomTile/Identity/SolidType.hpp"

class TankGearLeft final : public MFCPP::CustomTile, public MFCPP::SolidType {
public:
    explicit TankGearLeft(CustomTileManager& manager, const sf::Vector2f& position);
    ~TankGearLeft() override = default;
    void updatePreviousData() override;
    void statusUpdate(float deltaTime) override;
    void HitEvent() override;
    void KickEvent() override;
    void draw(float alpha) override;
    [[nodiscard]] sf::Vector2f getPosition() override;
    [[nodiscard]] sf::Vector2f getOrigin() override;
    [[nodiscard]] sf::FloatRect getHitbox() override;
    [[nodiscard]] bool isDestroyed() override;
    void animationUpdate(float deltaTime) override;
private:
    MFCPP::SingleAnimationObject m_animation;
    MFCPP::ActiveObject<float> m_transform;
    sf::FloatRect m_hitbox;
};

#endif //MFCPP_TANKGEARLEFT_HPP