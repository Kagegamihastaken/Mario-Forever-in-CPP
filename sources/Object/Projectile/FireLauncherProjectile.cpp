#include "Object/Projectile/FireLauncherProjectile.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Scroll.hpp"
#include "Core/Utility.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/ProjectileManager.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Effect/FireballExplosion.hpp"
#include "Effect/MarioEffect.hpp"
#include "Object/Mario.hpp"

FireLauncherProjectile::FireLauncherProjectile(ProjectileManager &manager, const sf::Vector2f &position, const sf::Vector2f& velocity)
    : Projectile(manager),
    m_transform(position, sf::Vector2f(12.f, 13.f), sf::degrees(Utility::RandomFloatNumberGenerator(0, 359.9f))){
    m_animation.setTexture("FireLauncherProjectile", true);
    m_hitbox = sf::FloatRect({0.f, 0.f}, {25.f, 26.f});
    setDrawingPriority(0);
    m_velocity = velocity;
    m_timePass = 0.f;
    m_timePassLimit = 3.25f;
}

void FireLauncherProjectile::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void FireLauncherProjectile::FireballEffect() const {
    AddFireballExplosion(m_transform.getCurrentPosition().x, m_transform.getCurrentPosition().y - 7.f);
}

void FireLauncherProjectile::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    m_timePass += deltaTime;
    if (m_timePass >= m_timePassLimit) setDrawingPriority(2);
    //Spin
    if (m_velocity.x < 0.f) m_transform.rotate(- sf::degrees(45.f * deltaTime));
    else m_transform.rotate(sf::degrees(45.f * deltaTime));
    //Status
    if (Scroll::isOutOfScreenYBottom(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0.f)) {
        Destroy();
        return;
    }
    //Movement
    m_transform.setCurrentPosition(sf::Vector2f(m_transform.getCurrentPosition().x + m_velocity.x * deltaTime, m_transform.getCurrentPosition().y + m_velocity.y * deltaTime));
    m_velocity.y += deltaTime * 0.35f;
}

void FireLauncherProjectile::CollisionUpdate() {
    if (isDestroyed()) return;
    if (EffectActive) return;
    const sf::FloatRect playerHitbox = getGlobalHitbox(Mario::getHitbox(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (sf::FloatRect loopHitbox = getGlobalHitbox(getHitbox(), m_transform.getCurrentPosition(), getOrigin()); isCollide(loopHitbox, playerHitbox)) {
        Mario::PowerDown();
        FireballEffect();
        Destroy();
    }
}

void FireLauncherProjectile::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 8.f)) return;
    m_animation.setAnimationDirection(m_velocity.x > 0.f);
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha) - sf::Vector2f(0.f, 0.f), getOrigin() - sf::Vector2f(0.f, 0.f));
    m_animation.setRotation(m_transform.getInterpolatedAngle(alpha));
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), m_transform.getCurrentPosition() - getOrigin(), sf::Color::Red));
}

void FireLauncherProjectile::Destroy() {
    m_transform.destroy();
    m_manager.setDeletionFlag(true);
}

void FireLauncherProjectile::LevelEndCleanup() {}

void FireLauncherProjectile::animationUpdate(float deltaTime) {}

sf::Vector2f FireLauncherProjectile::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f FireLauncherProjectile::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect FireLauncherProjectile::getHitbox() {
    return m_hitbox;
}

bool FireLauncherProjectile::isDestroyed() {
    return m_transform.isDestroyed();
}
