#include "Object/Enemy/GearLauncher.hpp"

#include "Block/CustomBlock.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/Tilemap.hpp"
#include "Core/Scene/GameScene.hpp"

GearLauncher::GearLauncher(CustomTileManager &manager, const sf::Vector2f &position) : CustomTile(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_animation.setTexture("GearLauncherUp");
    setHitbox(sf::FloatRect({0.f, 0.f}, {64.f, 64.f}));
    setOrigin(sf::Vector2f(32.f, 22.f));
    MFCPP::setIndexTilemapCollision(position.x, position.y, true);
    MFCPP::setIndexTilemapID(position.x, position.y, 0);
    setDrawingPriority(0);
    // Setting Collision
    sf::Vector2f original_pos = position - getOrigin();
    MFCPP::setIndexTilemapCollision(original_pos.x, original_pos.y, true);
    MFCPP::setIndexTilemapID(original_pos.x, original_pos.y, 0);
    MFCPP::setIndexTilemapFloorY(original_pos.x, original_pos.y, {22, 32});

    MFCPP::setIndexTilemapCollision(original_pos.x + 32.f, original_pos.y, true);
    MFCPP::setIndexTilemapID(original_pos.x + 32.f, original_pos.y, 0);
    MFCPP::setIndexTilemapFloorY(original_pos.x + 32.f, original_pos.y, {22, 32});

    MFCPP::setIndexTilemapCollision(original_pos.x, original_pos.y + 32.f, true);
    MFCPP::setIndexTilemapID(original_pos.x, original_pos.y + 32.f, 0);
    MFCPP::setIndexTilemapFloorY(original_pos.x, original_pos.y + 32.f, {0, 32});

    MFCPP::setIndexTilemapCollision(original_pos.x + 32.f, original_pos.y + 32.f, true);
    MFCPP::setIndexTilemapID(original_pos.x + 32.f, original_pos.y + 32.f, 0);
    MFCPP::setIndexTilemapFloorY(original_pos.x + 32.f, original_pos.y + 32.f, {0, 32});
}

void GearLauncher::setPreviousData() {
    if (isDestroyed()) return;
    setPreviousPosition(getCurrentPosition());
}

void GearLauncher::interpolateData(float alpha) {
    if (isDestroyed()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}

void GearLauncher::KickEvent() {}
void GearLauncher::HitEvent() {}
void GearLauncher::statusUpdate(float deltaTime) {}
void GearLauncher::draw() {
    if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 0.f)) return;
    m_animation.animationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.animationDraw();
}

