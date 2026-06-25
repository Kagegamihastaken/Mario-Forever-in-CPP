#ifndef MFCPP_BLUEGRASS_HPP
#define MFCPP_BLUEGRASS_HPP
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Object/Scenery.hpp"
#include "Core/Object/Scenery/Identity/GenericSceneryType.hpp"

class BlueGrass final : public MFCPP::Scenery, public MFCPP::GenericSceneryType {
public:
    explicit BlueGrass(SceneryManager& manager, const sf::Vector2f& position);
    ~BlueGrass() override = default;
    void updatePreviousData() override;
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

#endif //MFCPP_BLUEGRASS_HPP
