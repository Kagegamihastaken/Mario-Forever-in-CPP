#include "Object/Block/TankGearLeft.hpp"

#include "Block/CustomBlock.hpp"
#include "Core/AutoScroll.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/Tilemap.hpp"
#include "Core/Scene/GameScene.hpp"

TankGearLeft::TankGearLeft(CustomTileManager &manager, const sf::Vector2f &position) : CustomTile(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_animation.setAnimationSequence(TankGearLeftAnimName);
    m_animation.setAnimation(0, 2, 50, true);
    setHitbox(sf::FloatRect({0.f, 0.f}, {32.f, 32.f}));
    setOrigin(sf::Vector2f(0.f, 0.f));
    MFCPP::setIndexTilemapCollision(position.x, position.y, true);
    MFCPP::setIndexTilemapID(position.x, position.y, 0);
    MFCPP::setIndexTilemapFloorY(position.x, position.y, {0, 32});
    setDrawingPriority(0);
}

void TankGearLeft::setPreviousData() {
    if (isDestroyed()) return;
    setPreviousPosition(getCurrentPosition());
}

void TankGearLeft::interpolateData(float alpha) {
    if (isDestroyed()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}

void TankGearLeft::KickEvent() {}
void TankGearLeft::HitEvent() {}
void TankGearLeft::statusUpdate(float deltaTime) {
    if (MFCPP::AutoScroll::getTankAnimationStop() && m_animation.getFrequency() > 0)
        m_animation.setFrequencyAnimation(0);
}
void TankGearLeft::draw() {
    if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 0.f)) return;
    m_animation.animationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.animationDraw();
}

