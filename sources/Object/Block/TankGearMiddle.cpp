#include "Object/Block/TankGearMiddle.hpp"

#include "Block/CustomBlock.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/Tilemap.hpp"
#include "Core/Scene/GameScene.hpp"

TankGearMiddle::TankGearMiddle(CustomTileManager &manager, const sf::Vector2f &position) : CustomTile(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_animation.setAnimationSequence(TankGearMiddleAnimName);
    m_animation.setAnimation(0, 2, 50, true);
    setHitbox(sf::FloatRect({0.f, 0.f}, {32.f, 32.f}));
    setOrigin(sf::Vector2f(0.f, 0.f));
    MFCPP::setIndexTilemapCollision(position.x, position.y, true);
    MFCPP::setIndexTilemapID(position.x, position.y, 0);
    setDrawingPriority(0);
}

void TankGearMiddle::setPreviousData() {
    if (isDestroyed()) return;
    setPreviousPosition(getCurrentPosition());
}

void TankGearMiddle::interpolateData(float alpha) {
    if (isDestroyed()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}

void TankGearMiddle::KickEvent() {}
void TankGearMiddle::HitEvent() {}
void TankGearMiddle::statusUpdate(float deltaTime) {}
void TankGearMiddle::draw() {
    if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 0.f)) return;
    m_animation.animationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.animationDraw();
}

