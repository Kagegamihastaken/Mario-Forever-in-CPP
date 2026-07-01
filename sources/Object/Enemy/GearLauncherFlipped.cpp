#include "Object/Enemy/GearLauncherFlipped.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Tilemap.hpp"
#include "Core/Utility.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Effect/FireballExplosion.hpp"
#include "Object/Enemy/GearLauncher.hpp"
#include "Object/Projectile/GearProjectile.hpp"

GearLauncherFlipped::GearLauncherFlipped(CustomTileManager &manager, const sf::Vector2f &position)
    : CustomTile(manager),
    m_transform(position, sf::Vector2f(32.f, 41.f), sf::degrees(0.f)){
    m_animation.setTexture("GearLauncherDown");
    m_hitbox = sf::FloatRect({0.f, 0.f}, {64.f, 64.f});

    m_timePass = 0.f;
    m_timePassLimit = 150.f; //Will change later depend on usage

    MFCPP::Tilemap::setIndexTilemapCollision(position.x, position.y, true);
    MFCPP::Tilemap::setIndexTilemapID(position.x, position.y, 0);
    setDrawingPriority(0);
    // Setting Collision
    const sf::Vector2f original_pos = position - getOrigin();

    MFCPP::Tilemap::setIndexTilemapCollision(original_pos.x, original_pos.y, true);
    MFCPP::Tilemap::setIndexTilemapID(original_pos.x, original_pos.y, 0);
    MFCPP::Tilemap::setIndexTilemapFloorY(original_pos.x, original_pos.y, {0, 32});

    MFCPP::Tilemap::setIndexTilemapCollision(original_pos.x + 32.f, original_pos.y, true);
    MFCPP::Tilemap::setIndexTilemapID(original_pos.x + 32.f, original_pos.y, 0);
    MFCPP::Tilemap::setIndexTilemapFloorY(original_pos.x + 32.f, original_pos.y, {0, 32});

    MFCPP::Tilemap::setIndexTilemapCollision(original_pos.x, original_pos.y + 32.f, true);
    MFCPP::Tilemap::setIndexTilemapID(original_pos.x, original_pos.y + 32.f, 0);
    MFCPP::Tilemap::setIndexTilemapFloorY(original_pos.x, original_pos.y + 32.f, {0, 10});

    MFCPP::Tilemap::setIndexTilemapCollision(original_pos.x + 32.f, original_pos.y + 32.f, true);
    MFCPP::Tilemap::setIndexTilemapID(original_pos.x + 32.f, original_pos.y + 32.f, 0);
    MFCPP::Tilemap::setIndexTilemapFloorY(original_pos.x + 32.f, original_pos.y + 32.f, {0, 10});
}

void GearLauncherFlipped::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void GearLauncherFlipped::KickEvent() {}
void GearLauncherFlipped::HitEvent() {}

void GearLauncherFlipped::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    //Update before checking if outside screen
    m_timePass += deltaTime;
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), -32.f)) return;

    if (m_timePass > m_timePassLimit) {
        m_timePass = 0.f;
        GameScene::projectileManager.addProjectile<GearProjectile>(m_transform.getCurrentPosition(), sf::Vector2f(Utility::RandomFloatNumberGenerator(0.f, 2.f) - Utility::RandomFloatNumberGenerator(0, 4), 4.f + static_cast<float>(Utility::RandomIntNumberGenerator(0, 1))));
        AddFireballExplosion(m_transform.getCurrentPosition().x, m_transform.getCurrentPosition().y);
        SoundManager::PlaySound(SoundID::GAME_CHILUN);
    }
}
void GearLauncherFlipped::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) return;
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), m_transform.getCurrentPosition() - getOrigin(), sf::Color::Blue));
}

sf::Vector2f GearLauncherFlipped::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f GearLauncherFlipped::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect GearLauncherFlipped::getHitbox() {
    return m_hitbox;
}

bool GearLauncherFlipped::isDestroyed() {
    return m_transform.isDestroyed();
}

void GearLauncherFlipped::animationUpdate(float deltaTime) {}
