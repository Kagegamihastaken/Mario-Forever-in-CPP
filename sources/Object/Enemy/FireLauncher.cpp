#include "Object/Enemy/FireLauncher.hpp"

#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Tilemap.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Mario.hpp"
#include "Object/Enemy/BulletBill.hpp"
#include "Object/Projectile/FireLauncherProjectile.hpp"

FireLauncher::FireLauncher(CustomTileManager &manager, const sf::Vector2f &position) : CustomTile(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_animation.setTexture("FireLauncher");
    setHitbox(sf::FloatRect({0.f, 0.f}, {32.f, 32.f}));
    setOrigin(sf::Vector2f(0.f, 0.f));
    MFCPP::setIndexTilemapCollision(position.x - getOrigin().x, position.y - getOrigin().y, true);
    MFCPP::setIndexTilemapID(position.x - getOrigin().x, position.y - getOrigin().y, 0);
    MFCPP::setIndexTilemapFloorY(position.x - getOrigin().x, position.y - getOrigin().y, {0, 32});
    setDrawingPriority(1);
    m_FireInterval = 200.f;
    m_FireIntervalCounting = 0.f;
    m_ProjectileCount = 20;
    m_ProjectileCounting = 0;;
    m_FireBetweenInterval = 2.5f;
    m_FireBetweenIntervalCounting = 0.f;
    m_playSound = false;
}

void FireLauncher::setPreviousData() {
    if (isDestroyed()) return;
    setPreviousPosition(getCurrentPosition());
}

void FireLauncher::interpolateData(float alpha) {
    if (isDestroyed()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}

void FireLauncher::KickEvent() {}
void FireLauncher::HitEvent() {}

void FireLauncher::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    if (isOutScreen(getCurrentPosition().x, getCurrentPosition().y, 0.f, 0.f)) return;

    if (m_FireIntervalCounting > 0.f) {
        m_FireIntervalCounting -= deltaTime;
    }
    else if (m_FireIntervalCounting <= 0.f) {
        m_FireIntervalCounting = m_FireInterval;
        m_ProjectileCounting = m_ProjectileCount;
    }

    if (m_ProjectileCounting > 0) {
        if (!m_playSound) {
            SoundManager::PlaySound("Volcano");
            m_playSound = true;
        }
        m_FireBetweenIntervalCounting += deltaTime;
        if (m_FireBetweenIntervalCounting >= m_FireBetweenInterval) {
            m_FireBetweenIntervalCounting = f_mod(m_FireBetweenIntervalCounting, m_FireBetweenInterval);
            GameScene::projectileManager.addProjectile<FireLauncherProjectile>(getCurrentPosition() + sf::Vector2f(16.f, 16.f), sf::Vector2f(-8.5f, 0.f));
            m_ProjectileCounting -= 1;
        }
    }
    else {
        if (m_playSound) m_playSound = false;
    }
}
void FireLauncher::draw() {
    if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 0.f)) return;
    m_animation.animationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.animationDraw();
}

