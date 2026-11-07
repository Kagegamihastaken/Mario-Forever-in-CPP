#include "Object/Enemy/CannonLauncherDownRight.hpp"

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

CannonLauncherDownRight::CannonLauncherDownRight(CustomTileManager &manager, const sf::Vector2f &position)
    : CustomTile(manager),
    m_transform(position, sf::Vector2f(16.f, 31.f), sf::degrees(0.f)){
    m_animation.setTexture("CANNON_LAUNCHER_DOWN_RIGHT");
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

void CannonLauncherDownRight::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void CannonLauncherDownRight::KickEvent() {}
void CannonLauncherDownRight::HitEvent() {}

void CannonLauncherDownRight::statusUpdate(float deltaTime) {
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
        GameScene::effectManager.addEffect<FireballExplosionEffect>(m_transform.getCurrentPosition() - m_transform.getOrigin() + sf::Vector2f(24.f, 23.f));
        GameScene::enemyManager.addEnemy<CannonBullet>(m_transform.getCurrentPosition(), sf::Vector2f(2.75f, 2.75f), true);
    }
}
void CannonLauncherDownRight::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) return;
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
}

sf::Vector2f CannonLauncherDownRight::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f CannonLauncherDownRight::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect CannonLauncherDownRight::getHitbox() {
    return m_hitbox;
}

bool CannonLauncherDownRight::isDestroyed() {
    return m_transform.isDestroyed();
}

void CannonLauncherDownRight::animationUpdate(float deltaTime) {}
