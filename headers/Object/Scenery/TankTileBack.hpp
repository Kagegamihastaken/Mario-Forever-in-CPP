#ifndef MFCPP_TANKTILEBACK_HPP
#define MFCPP_TANKTILEBACK_HPP
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Object/Scenery.hpp"
#include "Core/Object/Scenery/Identity/GenericSceneryType.hpp"

class TankTileBack final : public MFCPP::Scenery, public MFCPP::GenericSceneryType {
public:
    explicit TankTileBack(SceneryManager& manager, const sf::Vector2f& position);
    ~TankTileBack() override = default;
    void updatePreviousData() override;
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
};

#endif //MFCPP_TANKTILEBACK_HPP
