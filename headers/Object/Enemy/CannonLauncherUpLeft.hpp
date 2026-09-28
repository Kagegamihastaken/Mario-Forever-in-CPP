#ifndef MFCPP_CANNONLAUNCHER_UPLEFT_HPP
#define MFCPP_CANNONLAUNCHER_UPLEFT_HPP
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/CustomTile.hpp"
#include "Core/Object/CustomTile/Identity/SolidType.hpp"

class CannonLauncherUpLeft final : public MFCPP::CustomTile, public MFCPP::SolidType {
public:
    explicit CannonLauncherUpLeft(CustomTileManager& manager, const sf::Vector2f& position);
    ~CannonLauncherUpLeft() override = default;
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
    float m_launch_interval, m_first_shot_time, m_timing;
    int m_random_fire_interval;
    bool m_disabled, m_state;
};

#endif //MFCPP_CANNONLAUNCHER_UPLEFT_HPP