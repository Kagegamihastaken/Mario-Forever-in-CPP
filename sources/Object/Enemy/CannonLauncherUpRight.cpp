#include "Object/Enemy/CannonLauncherUpRight.hpp"

#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Tilemap.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Object/CustomTile/Behavior/BulletBillLauncherBehavior.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Mario.hpp"
#include "Object/Effect/FireballExplosionEffect.hpp"
#include "Object/Enemy/BulletBill.hpp"
#include "Object/Enemy/CannonBullet.hpp"

CannonLauncherUpRight::CannonLauncherUpRight(CustomTileManager &manager, const sf::Vector2f &position)
    : CustomTile(manager),
    m_transform(position, sf::Vector2f(16.f, 31.f), sf::degrees(0.f)){
    m_animation.setTexture("CANNON_LAUNCHER_UP_RIGHT");
    m_hitbox = sf::FloatRect({0.f, 0.f}, {32.f, 32.f});
    MFCPP::Tilemap::setIndexTilemapCollision(position.x - getOrigin().x, position.y - getOrigin().y, true);
    MFCPP::Tilemap::setIndexTilemapID(position.x - getOrigin().x, position.y - getOrigin().y, 0);
    MFCPP::Tilemap::setIndexTilemapFloorY(position.x - getOrigin().x, position.y - getOrigin().y, {0, 32});
    setDrawingPriority(1);

    m_launch_interval = 100.f;

    m_random_fire_interval = 0;
    m_first_shot_time = 25.f;
    m_timing = 0.f;
    m_disabled = false;
    m_state = false;
}

void CannonLauncherUpRight::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void CannonLauncherUpRight::KickEvent() {}
void CannonLauncherUpRight::HitEvent() {}

void CannonLauncherUpRight::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0)) return;

    bool shoot = false;
    BulletBillLauncherBehavior::BulletBillLauncherData data = BulletBillLauncherBehavior::BulletBillLauncherUpdate(BulletBillLauncherBehavior::BulletBillLauncherData(
    m_transform.getCurrentPosition(), m_disabled, m_state, m_timing, m_launch_interval, m_first_shot_time, m_random_fire_interval), shoot, deltaTime, true);

    m_disabled = data.disabled;
    m_state = data.state;
    m_timing = data.timing;

    if (shoot) {
        SoundManager::PlaySound(SoundID::GAME_CANNON);
        GameScene::effectManager.addEffect<FireballExplosionEffect>(m_transform.getCurrentPosition() - m_transform.getOrigin() + sf::Vector2f(24.f, 8.f));
        GameScene::enemyManager.addEnemy<CannonBullet>(m_transform.getCurrentPosition(), sf::Vector2f(2.75f, -2.75f), true);
    }
}
void CannonLauncherUpRight::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) return;
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
}

sf::Vector2f CannonLauncherUpRight::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f CannonLauncherUpRight::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect CannonLauncherUpRight::getHitbox() {
    return m_hitbox;
}

bool CannonLauncherUpRight::isDestroyed() {
    return m_transform.isDestroyed();
}

void CannonLauncherUpRight::animationUpdate(float deltaTime) {}
