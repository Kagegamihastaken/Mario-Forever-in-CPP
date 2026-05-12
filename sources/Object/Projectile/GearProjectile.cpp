#include "Object/Projectile/GearProjectile.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/ProjectileManager.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Effect/FireballExplosion.hpp"
#include "Effect/MarioEffect.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Object/Mario.hpp"
#include "Object/Projectile/GearProjectileEffect.hpp"

GearProjectile::GearProjectile(ProjectileManager &manager, const sf::Vector2f &position) : Projectile(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_animation.setTexture("GearProjectile", true);
    setOrigin(sf::Vector2f(20.f, 20.f));
    setHitbox(sf::FloatRect({0.f, 0.f}, {40.f, 40.f}));
    m_timeEffect = 0.f;
    m_timeEffectMax = 2.5f;
    m_velocity = {static_cast<float>(RandomIntNumberGenerator(0, 2)) - static_cast<float>(RandomIntNumberGenerator(0, 4)), (8.f + static_cast<float>(RandomIntNumberGenerator(0, 2))) * -1.f};
    setDrawingPriority(1);
}

void GearProjectile::setPreviousData() {
    if (isDestroyed()) return;
    setPreviousPosition(getCurrentPosition());
    setPreviousAngle(getCurrentAngle());
}
void GearProjectile::interpolateData(const float alpha) {
    if (isDestroyed()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
    setInterpolatedAngle(linearInterpolation(getPreviousAngle(), getCurrentAngle(), alpha));
}

void GearProjectile::FireballEffect() const {}

void GearProjectile::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    //Spin
    m_timeEffect += deltaTime;
    if (m_timeEffect >= m_timeEffectMax) {
        m_timeEffect = f_mod(m_timeEffect, m_timeEffectMax);
        m_manager.addProjectile<GearProjectileEffect>(getCurrentPosition(), getCurrentAngle(), m_velocity.x > 0.f);
    }
    if (m_velocity.x < 0.f) setCurrentAngle(getCurrentAngle() - sf::degrees(11.5f * deltaTime));
    else setCurrentAngle(getCurrentAngle() + sf::degrees(11.5f * deltaTime));
    //Status
    if (isOutOfScreenYBottom(MFCPP::CollisionObject(getCurrentPosition(), getOrigin(), getHitbox()), 0.f)) {
        Destroy();
        return;
    }
    //Movement
    setCurrentPosition(sf::Vector2f(getCurrentPosition().x + m_velocity.x * deltaTime, getCurrentPosition().y + m_velocity.y * deltaTime));
    m_velocity.y += (m_velocity.y >= 10.0f ? 0.0f : deltaTime * 0.175f);
}

void GearProjectile::CollisionUpdate() {
    if (isDestroyed()) return;
    if (EffectActive) return;
    const sf::FloatRect playerHitbox = getGlobalHitbox(Mario::getHitbox(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (sf::FloatRect loopHitbox = getGlobalHitbox(getHitbox(), getCurrentPosition(), getOrigin()); isCollide(loopHitbox, playerHitbox)) {
        Mario::PowerDown();
    }
}

void GearProjectile::draw() {
    if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 32.f)) return;
    m_animation.setAnimationDirection(m_velocity.x > 0.f);
    m_animation.animationUpdate(getInterpolatedPosition() - sf::Vector2f(0.f, 0.f), getOrigin() - sf::Vector2f(0.f, 0.f));
    m_animation.setRotation(getInterpolatedAngle());
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), getCurrentPosition() - getOrigin(), sf::Color::Red));
}

void GearProjectile::Destroy() {
    setDestroyed(true);
    m_manager.setDeletionFlag(true);
}

void GearProjectile::LevelEndCleanup() {
    AddScoreEffect(SCORE_100, getCurrentPosition().x, getCurrentPosition().y - getOrigin().y);
    Destroy();
}
