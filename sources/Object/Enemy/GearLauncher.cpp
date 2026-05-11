#include "Object/Enemy/GearLauncher.hpp"

#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Tilemap.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Effect/FireballExplosion.hpp"
#include "Object/Projectile/GearProjectile.hpp"

GearLauncher::GearLauncher(CustomTileManager &manager, const sf::Vector2f &position) : CustomTile(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_animation.setTexture("GearLauncherUp");
    setHitbox(sf::FloatRect({0.f, 0.f}, {64.f, 64.f}));
    setOrigin(sf::Vector2f(32.f, 22.f));

    m_timePass = 0.f;
    m_timePassLimit = 150.f; //Will change later depend on usage

    MFCPP::setIndexTilemapCollision(position.x, position.y, true);
    MFCPP::setIndexTilemapID(position.x, position.y, 0);
    setDrawingPriority(0);
    // Setting Collision
    const sf::Vector2f original_pos = position - getOrigin();

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

void GearLauncher::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    if (isOutScreen(getCurrentPosition().x, getCurrentPosition().y, 0.f, 0.f)) return;

    m_timePass += deltaTime;
    if (m_timePass > m_timePassLimit) {
        m_timePass = 0.f;
        GameScene::projectileManager.addProjectile<GearProjectile>(getCurrentPosition());
        AddFireballExplosion(getCurrentPosition().x, getCurrentPosition().y);
        SoundManager::PlaySound("Chilun");
    }
}
void GearLauncher::draw() {
    if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 0.f)) return;
    m_animation.animationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.animationDraw();
}

