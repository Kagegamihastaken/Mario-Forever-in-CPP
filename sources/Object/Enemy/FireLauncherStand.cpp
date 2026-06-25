#include "Object/Enemy/FireLauncherStand.hpp"

#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Tilemap.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Object/CustomTile/Behavior/FireLauncherBehavior.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Mario.hpp"
#include "Object/Enemy/BulletBill.hpp"
#include "Object/Projectile/FireLauncherProjectile.hpp"

FireLauncherStand::FireLauncherStand(CustomTileManager &manager, const sf::Vector2f &position)
    : CustomTile(manager),
    m_transform(position, sf::Vector2f(0.f, 0.f), sf::degrees(0.f)){
    m_animation.setTexture("FireLauncherStand");
    m_hitbox = sf::FloatRect({0.f, 0.f}, {96.f, 64.f});

    const sf::Vector2f original_pos = position - getOrigin();

    MFCPP::Tilemap::setIndexTilemapCollision(original_pos.x, original_pos.y + 32.f, true);
    MFCPP::Tilemap::setIndexTilemapID(original_pos.x, original_pos.y + 32.f, 0);
    MFCPP::Tilemap::setIndexTilemapFloorY(original_pos.x, original_pos.y + 32.f, {0, 32});

    MFCPP::Tilemap::setIndexTilemapCollision(original_pos.x + 32.f, original_pos.y + 32.f, true);
    MFCPP::Tilemap::setIndexTilemapID(original_pos.x + 32.f, original_pos.y + 32.f, 0);
    MFCPP::Tilemap::setIndexTilemapFloorY(original_pos.x + 32.f, original_pos.y + 32.f, {0, 32});

    MFCPP::Tilemap::setIndexTilemapCollision(original_pos.x, original_pos.y + 64.f, true);
    MFCPP::Tilemap::setIndexTilemapID(original_pos.x, original_pos.y + 64.f, 0);
    MFCPP::Tilemap::setIndexTilemapFloorY(original_pos.x, original_pos.y + 64.f, {0, 32});

    MFCPP::Tilemap::setIndexTilemapCollision(original_pos.x + 32.f, original_pos.y + 64.f, true);
    MFCPP::Tilemap::setIndexTilemapID(original_pos.x + 32.f, original_pos.y + 64.f, 0);
    MFCPP::Tilemap::setIndexTilemapFloorY(original_pos.x + 32.f, original_pos.y + 64.f, {0, 32});


    setDrawingPriority(1);
    m_FireInterval = 200.f;
    m_FireIntervalCounting = 0.f;
    m_ProjectileCount = 20;
    m_ProjectileCounting = 0;;
    m_FireBetweenInterval = 3.5f;
    m_FireBetweenIntervalCounting = 0.f;
    m_playSound = false;
}

void FireLauncherStand::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void FireLauncherStand::KickEvent() {}
void FireLauncherStand::HitEvent() {}

void FireLauncherStand::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0)) return;

    bool launch = false, sound = false;
    auto data = FireLauncherBehavior::FireLauncherStatusUpdate(FireLauncherBehavior::FireLauncherData(m_FireInterval, m_FireIntervalCounting, m_ProjectileCount, m_ProjectileCounting, m_FireBetweenInterval, m_FireBetweenIntervalCounting, m_playSound), launch, sound, deltaTime);
    if (sound)
        SoundManager::PlaySound("Volcano");
    if (launch)
        GameScene::projectileManager.addProjectile<FireLauncherProjectile>(m_transform.getCurrentPosition() + sf::Vector2f(19.f, 38.f), sf::Vector2f(-5.f, -8.5f));
    m_FireBetweenIntervalCounting = data.FireBetweenIntervalCounting;
    m_ProjectileCounting = data.ProjectileCounting;
    m_FireIntervalCounting = data.FireIntervalCounting;
    m_playSound = data.PlaySound;
}
void FireLauncherStand::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) return;
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
}

sf::Vector2f FireLauncherStand::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f FireLauncherStand::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect FireLauncherStand::getHitbox() {
    return m_hitbox;
}

bool FireLauncherStand::isDestroyed() {
    return m_transform.isDestroyed();
}

void FireLauncherStand::animationUpdate(float deltaTime) {}

