#include "Object/Enemy/GearLauncher.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Tilemap.hpp"
#include "Core/Utility.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Effect/FireballExplosion.hpp"
#include "Object/Projectile/GearProjectile.hpp"

GearLauncher::GearLauncher(CustomTileManager &manager, const sf::Vector2f &position)
    : CustomTile(manager),
    m_transform(position, sf::Vector2f(32.f, 22.f), sf::degrees(0.f)){
    m_animation.setTexture("GearLauncherUp");
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
    MFCPP::Tilemap::setIndexTilemapFloorY(original_pos.x, original_pos.y, {22, 32});

    MFCPP::Tilemap::setIndexTilemapCollision(original_pos.x + 32.f, original_pos.y, true);
    MFCPP::Tilemap::setIndexTilemapID(original_pos.x + 32.f, original_pos.y, 0);
    MFCPP::Tilemap::setIndexTilemapFloorY(original_pos.x + 32.f, original_pos.y, {22, 32});

    MFCPP::Tilemap::setIndexTilemapCollision(original_pos.x, original_pos.y + 32.f, true);
    MFCPP::Tilemap::setIndexTilemapID(original_pos.x, original_pos.y + 32.f, 0);
    MFCPP::Tilemap::setIndexTilemapFloorY(original_pos.x, original_pos.y + 32.f, {0, 32});

    MFCPP::Tilemap::setIndexTilemapCollision(original_pos.x + 32.f, original_pos.y + 32.f, true);
    MFCPP::Tilemap::setIndexTilemapID(original_pos.x + 32.f, original_pos.y + 32.f, 0);
    MFCPP::Tilemap::setIndexTilemapFloorY(original_pos.x + 32.f, original_pos.y + 32.f, {0, 32});
}

void GearLauncher::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void GearLauncher::KickEvent() {}
void GearLauncher::HitEvent() {}

void GearLauncher::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    //Update before checking if outside screen
    m_timePass += deltaTime;
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), -32.f)) return;

    if (m_timePass > m_timePassLimit) {
        m_timePass = 0.f;
        GameScene::projectileManager.addProjectile<GearProjectile>(m_transform.getCurrentPosition(), sf::Vector2f(Utility::RandomFloatNumberGenerator(0.f, 2.f) - Utility::RandomFloatNumberGenerator(0, 4), (8.f + static_cast<float>(Utility::RandomIntNumberGenerator(0, 2))) * -1.f));
        AddFireballExplosion(m_transform.getCurrentPosition().x, m_transform.getCurrentPosition().y);
        SoundManager::PlaySound(SoundID::GAME_CHILUN);
    }
}
void GearLauncher::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) return;
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), m_transform.getCurrentPosition() - getOrigin(), sf::Color::Blue));
}

sf::Vector2f GearLauncher::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f GearLauncher::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect GearLauncher::getHitbox() {
    return m_hitbox;
}

bool GearLauncher::isDestroyed() {
    return m_transform.isDestroyed();
}

void GearLauncher::animationUpdate(float deltaTime) {}

