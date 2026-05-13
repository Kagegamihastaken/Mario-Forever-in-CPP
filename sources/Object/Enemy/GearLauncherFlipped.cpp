#include "Object/Enemy/GearLauncherFlipped.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Tilemap.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Effect/FireballExplosion.hpp"
#include "Object/Projectile/GearProjectile.hpp"

GearLauncherFlipped::GearLauncherFlipped(CustomTileManager &manager, const sf::Vector2f &position) : CustomTile(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_animation.setTexture("GearLauncherDown");
    setHitbox(sf::FloatRect({0.f, 0.f}, {64.f, 64.f}));
    setOrigin(sf::Vector2f(32.f, 41.f));

    m_timePass = 0.f;
    m_timePassLimit = 150.f; //Will change later depend on usage

    MFCPP::setIndexTilemapCollision(position.x, position.y, true);
    MFCPP::setIndexTilemapID(position.x, position.y, 0);
    setDrawingPriority(0);
    // Setting Collision
    const sf::Vector2f original_pos = position - getOrigin();

    MFCPP::setIndexTilemapCollision(original_pos.x, original_pos.y, true);
    MFCPP::setIndexTilemapID(original_pos.x, original_pos.y, 0);
    MFCPP::setIndexTilemapFloorY(original_pos.x, original_pos.y, {0, 32});

    MFCPP::setIndexTilemapCollision(original_pos.x + 32.f, original_pos.y, true);
    MFCPP::setIndexTilemapID(original_pos.x + 32.f, original_pos.y, 0);
    MFCPP::setIndexTilemapFloorY(original_pos.x + 32.f, original_pos.y, {0, 32});

    MFCPP::setIndexTilemapCollision(original_pos.x, original_pos.y + 32.f, true);
    MFCPP::setIndexTilemapID(original_pos.x, original_pos.y + 32.f, 0);
    MFCPP::setIndexTilemapFloorY(original_pos.x, original_pos.y + 32.f, {0, 10});

    MFCPP::setIndexTilemapCollision(original_pos.x + 32.f, original_pos.y + 32.f, true);
    MFCPP::setIndexTilemapID(original_pos.x + 32.f, original_pos.y + 32.f, 0);
    MFCPP::setIndexTilemapFloorY(original_pos.x + 32.f, original_pos.y + 32.f, {0, 10});
}

void GearLauncherFlipped::setPreviousData() {
    if (isDestroyed()) return;
    setPreviousPosition(getCurrentPosition());
}

void GearLauncherFlipped::interpolateData(float alpha) {
    if (isDestroyed()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}

void GearLauncherFlipped::KickEvent() {}
void GearLauncherFlipped::HitEvent() {}

void GearLauncherFlipped::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    //Update before checking if outside screen
    m_timePass += deltaTime;
    if (isOutScreen(getCurrentPosition().x, getCurrentPosition().y, 0.f, 0.f)) return;

    if (m_timePass > m_timePassLimit) {
        m_timePass = 0.f;
        GameScene::projectileManager.addProjectile<GearProjectile>(getCurrentPosition(), sf::Vector2f(static_cast<float>(RandomIntNumberGenerator(0, 2)) - static_cast<float>(RandomIntNumberGenerator(0, 4)), 4.f + static_cast<float>(RandomIntNumberGenerator(0, 1))));
        AddFireballExplosion(getCurrentPosition().x, getCurrentPosition().y);
        SoundManager::PlaySound("Chilun");
    }
}
void GearLauncherFlipped::draw() {
    if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 0.f)) return;
    m_animation.animationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), getCurrentPosition() - getOrigin(), sf::Color::Blue));
}

