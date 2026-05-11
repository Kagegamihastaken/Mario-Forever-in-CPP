#include "Object/Enemy/BulletBillLauncher.hpp"

#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Tilemap.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Object/CustomTile/Behavior/BulletBillLauncherBehavior.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Effect/FireballExplosion.hpp"
#include "Effect/MarioEffect.hpp"
#include "Object/Mario.hpp"
#include "Object/Enemy/BulletBill.hpp"

BulletBillLauncher::BulletBillLauncher(CustomTileManager &manager, const sf::Vector2f &position) : CustomTile(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_animation.setTexture("BulletLauncher");
    setHitbox(sf::FloatRect({0.f, 0.f}, {32.f, 32.f}));
    setOrigin(sf::Vector2f(16.f, 31.f));
    MFCPP::setIndexTilemapCollision(position.x, position.y, true);
    MFCPP::setIndexTilemapID(position.x, position.y, 0);
    MFCPP::setIndexTilemapFloorY(position.x, position.y, {0, 32});
    setDrawingPriority(1);

    m_launch_interval = 75.f;

    m_random_fire_interval = 149;
    m_first_shot_time = 25.f;
    m_timing = 0.f;
    m_disabled = false;
    m_state = false;
}

void BulletBillLauncher::setPreviousData() {
    if (isDestroyed()) return;
    setPreviousPosition(getCurrentPosition());
}

void BulletBillLauncher::interpolateData(float alpha) {
    if (isDestroyed()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}

void BulletBillLauncher::KickEvent() {}
void BulletBillLauncher::HitEvent() {}

void BulletBillLauncher::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    if (isOutScreen(getCurrentPosition().x, getCurrentPosition().y, 32.f, 32.f)) return;

    bool shoot = false;
    BulletBillLauncherBehavior::BulletBillLauncherData data = BulletBillLauncherBehavior::BulletBillLauncherUpdate(BulletBillLauncherBehavior::BulletBillLauncherData(
    getCurrentPosition(), m_disabled, m_state, m_timing, m_launch_interval, m_first_shot_time, m_random_fire_interval), shoot, deltaTime);

    m_disabled = data.disabled;
    m_state = data.state;
    m_timing = data.timing;

    if (shoot) {
        SoundManager::PlaySound(fmt::format("Bullet{}", RandomIntNumberGenerator(1, 3)));
        const auto dir = (getCurrentPosition().x > Mario::getCurrentPosition().x ? false : !EffectActive);
        AddFireballExplosion(getCurrentPosition().x - getOrigin().x * (dir ? -1.f : 1.f), getCurrentPosition().y - 32.f / 2.f + 1.f);
        GameScene::enemyManager.addEnemy<BulletBill>(sf::Vector2f(getCurrentPosition().x, getCurrentPosition().y), 3.75f, dir);
    }
}
void BulletBillLauncher::draw() {
    if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 0.f)) return;
    m_animation.animationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.animationDraw();
}

