#include "Object/Enemy/FireLauncher.hpp"

#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Tilemap.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Object/CustomTile/Behavior/FireLauncherBehavior.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Mario.hpp"
#include "Object/Enemy/BulletBill.hpp"
#include "Object/Projectile/FireLauncherProjectile.hpp"

FireLauncher::FireLauncher(CustomTileManager &manager, const sf::Vector2f &position)
    : CustomTile(manager),
    m_transform(position, sf::Vector2f(0.f, 0.f), sf::degrees(0.f)){
    m_animation.setTexture("FireLauncher");
    m_hitbox = sf::FloatRect({0.f, 0.f}, {32.f, 32.f});
    MFCPP::Tilemap::setIndexTilemapCollision(position.x - getOrigin().x, position.y - getOrigin().y, true);
    MFCPP::Tilemap::setIndexTilemapID(position.x - getOrigin().x, position.y - getOrigin().y, 0);
    MFCPP::Tilemap::setIndexTilemapFloorY(position.x - getOrigin().x, position.y - getOrigin().y, {0, 32});
    setDrawingPriority(1);
    m_FireInterval = 200.f;
    m_FireIntervalCounting = 0.f;
    m_ProjectileCount = 20;
    m_ProjectileCounting = 0;;
    m_FireBetweenInterval = 2.5f;
    m_FireBetweenIntervalCounting = 0.f;
    m_playSound = false;
}

void FireLauncher::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void FireLauncher::KickEvent() {}
void FireLauncher::HitEvent() {}

void FireLauncher::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0)) return;

    bool launch = false, sound = false;
    auto data = FireLauncherBehavior::FireLauncherStatusUpdate(FireLauncherBehavior::FireLauncherData(m_FireInterval, m_FireIntervalCounting, m_ProjectileCount, m_ProjectileCounting, m_FireBetweenInterval, m_FireBetweenIntervalCounting, m_playSound), launch, sound, deltaTime);
    if (sound)
        SoundManager::PlaySound("Volcano");
    if (launch)
        GameScene::projectileManager.addProjectile<FireLauncherProjectile>(m_transform.getCurrentPosition() + sf::Vector2f(16.f, 16.f), sf::Vector2f(-8.5f, 0.f));
    m_FireBetweenIntervalCounting = data.FireBetweenIntervalCounting;
    m_ProjectileCounting = data.ProjectileCounting;
    m_FireIntervalCounting = data.FireIntervalCounting;
    m_playSound = data.PlaySound;
}
void FireLauncher::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) return;
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
}

sf::Vector2f FireLauncher::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f FireLauncher::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect FireLauncher::getHitbox() {
    return m_hitbox;
}

bool FireLauncher::isDestroyed() {
    return m_transform.isDestroyed();
}

void FireLauncher::animationUpdate(float deltaTime) {}

