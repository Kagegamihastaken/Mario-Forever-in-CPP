#include "Object/Projectile/FireLauncherProjectile.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/ProjectileManager.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Effect/FireballExplosion.hpp"
#include "Effect/MarioEffect.hpp"
#include "Object/Mario.hpp"

FireLauncherProjectile::FireLauncherProjectile(ProjectileManager &manager, const sf::Vector2f &position, const sf::Vector2f& velocity) : Projectile(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_animation.setTexture("FireLauncherProjectile", true);
    setOrigin(sf::Vector2f(12.f, 12.f));
    setHitbox(sf::FloatRect({0.f, 0.f}, {25.f, 26.f}));
    setDrawingPriority(0);
    m_velocity = velocity;
    m_timePass = 0.f;
    m_timePassLimit = 3.25f;
}

void FireLauncherProjectile::setPreviousData() {
    if (isDestroyed()) return;
    setPreviousPosition(getCurrentPosition());
    setPreviousAngle(getCurrentAngle());
}
void FireLauncherProjectile::interpolateData(const float alpha) {
    if (isDestroyed()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
    setInterpolatedAngle(linearInterpolation(getPreviousAngle(), getCurrentAngle(), alpha));
}

void FireLauncherProjectile::FireballEffect() const {
    AddFireballExplosion(getCurrentPosition().x, getCurrentPosition().y - 7.f);
}

void FireLauncherProjectile::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    m_timePass += deltaTime;
    if (m_timePass >= m_timePassLimit) setDrawingPriority(2);
    //Spin
    if (m_velocity.x < 0.f) setCurrentAngle(getCurrentAngle() - sf::degrees(45.f * deltaTime));
    else setCurrentAngle(getCurrentAngle() + sf::degrees(45.f * deltaTime));
    //Status
    if (isOutOfScreenYBottom(MFCPP::CollisionObject(getCurrentPosition(), getOrigin(), getHitbox()), 0.f)) {
        Destroy();
        return;
    }
    //Movement
    setCurrentPosition(sf::Vector2f(getCurrentPosition().x + m_velocity.x * deltaTime, getCurrentPosition().y + m_velocity.y * deltaTime));
    m_velocity.y += (m_velocity.y >= 10.0f ? 0.0f : deltaTime * 0.35f);
}

void FireLauncherProjectile::CollisionUpdate() {
    if (isDestroyed()) return;
    if (EffectActive) return;
    const sf::FloatRect playerHitbox = getGlobalHitbox(Mario::getHitbox(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (sf::FloatRect loopHitbox = getGlobalHitbox(getHitbox(), getCurrentPosition(), getOrigin()); isCollide(loopHitbox, playerHitbox)) {
        Mario::PowerDown();
        FireballEffect();
        Destroy();
    }
}

void FireLauncherProjectile::draw() {
    if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 32.f)) return;
    m_animation.setAnimationDirection(m_velocity.x > 0.f);
    m_animation.animationUpdate(getInterpolatedPosition() - sf::Vector2f(0.f, 0.f), getOrigin() - sf::Vector2f(0.f, 0.f));
    m_animation.setRotation(getInterpolatedAngle());
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), getCurrentPosition() - getOrigin(), sf::Color::Red));
}

void FireLauncherProjectile::Destroy() {
    setDestroyed(true);
    m_manager.setDeletionFlag(true);
}

void FireLauncherProjectile::LevelEndCleanup() {}
