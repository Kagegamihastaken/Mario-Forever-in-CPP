#include "Object/Block/TankGearMiddle.hpp"

#include "Block/CustomBlock.hpp"
#include "Core/AutoScroll.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/Tilemap.hpp"
#include "Core/Scene/GameScene.hpp"

TankGearMiddle::TankGearMiddle(CustomTileManager &manager, const sf::Vector2f &position)
    : CustomTile(manager),
    m_transform(position, sf::Vector2f(0.f, 0.f), sf::degrees(0.f)) {
    m_animation.setAnimationSequence("TankGearMiddleAnimName");
    m_animation.setAnimation(0, 2, 50, true);
    m_hitbox = sf::FloatRect({0.f, 0.f}, {32.f, 32.f});
    MFCPP::Tilemap::setIndexTilemapCollision(position.x, position.y, true);
    MFCPP::Tilemap::setIndexTilemapID(position.x, position.y, 0);
    MFCPP::Tilemap::setIndexTilemapFloorY(position.x, position.y, {0, 32});
    setDrawingPriority(0);
}

void TankGearMiddle::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void TankGearMiddle::KickEvent() {}
void TankGearMiddle::HitEvent() {}
void TankGearMiddle::statusUpdate(float deltaTime) {
    if (MFCPP::AutoScroll::getTankAnimationStop() && m_animation.getFrequency() > 0)
        m_animation.setFrequencyAnimation(0);
}
void TankGearMiddle::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) {
        m_animation.frameUpdate();
        return;
    }
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
}

void TankGearMiddle::animationUpdate(float deltaTime) {
    m_animation.frameTimeAccumulate(deltaTime);
}

sf::Vector2f TankGearMiddle::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f TankGearMiddle::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect TankGearMiddle::getHitbox() {
    return m_hitbox;
}

bool TankGearMiddle::isDestroyed() {
    return m_transform.isDestroyed();
}
