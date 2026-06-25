#ifndef MFCPP_TANKGEARRIGHT_HPP
#define MFCPP_TANKGEARRIGHT_HPP
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Object/CustomTile.hpp"
#include "Core/Object/CustomTile/Identity/SolidType.hpp"

class TankGearRight final : public MFCPP::CustomTile, public MFCPP::SolidType {
public:
    explicit TankGearRight(CustomTileManager& manager, const sf::Vector2f& position);
    ~TankGearRight() override = default;
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

#endif //MFCPP_TANKGEARRIGHT_HPP