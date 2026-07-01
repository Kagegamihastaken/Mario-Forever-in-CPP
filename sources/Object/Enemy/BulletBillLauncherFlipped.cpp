#include "Object/Enemy/BulletBillLauncherFlipped.hpp"

#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Tilemap.hpp"
#include "Core/Utility.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Object/CustomTile/Behavior/BulletBillLauncherBehavior.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Effect/FireballExplosion.hpp"
#include "Effect/MarioEffect.hpp"
#include "Object/Mario.hpp"
#include "Object/Enemy/BulletBill.hpp"

BulletBillLauncherFlipped::BulletBillLauncherFlipped(CustomTileManager &manager, const sf::Vector2f &position)
    : CustomTile(manager),
    m_transform(position, sf::Vector2f(16.f, 31.f), sf::degrees(0.f)){
    m_animation.setTexture("BulletLauncherFlipped");
    m_hitbox = sf::FloatRect({0.f, 0.f}, {32.f, 32.f});
    MFCPP::Tilemap::setIndexTilemapCollision(position.x - getOrigin().x, position.y - getOrigin().y, true);
    MFCPP::Tilemap::setIndexTilemapID(position.x - getOrigin().x, position.y - getOrigin().y, 0);
    MFCPP::Tilemap::setIndexTilemapFloorY(position.x - getOrigin().x, position.y - getOrigin().y, {0, 32});
    setDrawingPriority(1);

    m_launch_interval = 75.f;

    m_random_fire_interval = 149;
    m_first_shot_time = 25.f;
    m_timing = 0.f;
    m_disabled = false;
    m_state = false;
}

void BulletBillLauncherFlipped::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void BulletBillLauncherFlipped::KickEvent() {}
void BulletBillLauncherFlipped::HitEvent() {}

void BulletBillLauncherFlipped::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0)) return;

    bool shoot = false;
    BulletBillLauncherBehavior::BulletBillLauncherData data = BulletBillLauncherBehavior::BulletBillLauncherUpdate(BulletBillLauncherBehavior::BulletBillLauncherData(
    m_transform.getCurrentPosition(), m_disabled, m_state, m_timing, m_launch_interval, m_first_shot_time, m_random_fire_interval), shoot, deltaTime);

    m_disabled = data.disabled;
    m_state = data.state;
    m_timing = data.timing;

    if (shoot) {
        switch (Utility::RandomIntNumberGenerator(1, 3)) {
            case 1:
                SoundManager::PlaySound(SoundID::GAME_BULLET1);
                break;
            case 2:
                SoundManager::PlaySound(SoundID::GAME_BULLET2);
                break;
            case 3:
                SoundManager::PlaySound(SoundID::GAME_BULLET3);
                break;
            default: ;
        }
        const auto dir = (m_transform.getCurrentPosition().x > Mario::getCurrentPosition().x ? false : !EffectActive);
        AddFireballExplosion(m_transform.getCurrentPosition().x - getOrigin().x * (dir ? -1.f : 1.f), m_transform.getCurrentPosition().y - 32.f / 2.f + 1.f);
        GameScene::enemyManager.addEnemy<BulletBill>(sf::Vector2f(m_transform.getCurrentPosition().x, m_transform.getCurrentPosition().y), 3.75f, dir);
    }
}
void BulletBillLauncherFlipped::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) return;
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
}

sf::Vector2f BulletBillLauncherFlipped::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f BulletBillLauncherFlipped::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect BulletBillLauncherFlipped::getHitbox() {
    return m_hitbox;
}

bool BulletBillLauncherFlipped::isDestroyed() {
    return m_transform.isDestroyed();
}

void BulletBillLauncherFlipped::animationUpdate(float deltaTime) {}

