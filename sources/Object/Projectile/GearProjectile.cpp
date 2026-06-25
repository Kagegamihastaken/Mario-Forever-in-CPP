#include "Object/Projectile/GearProjectile.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/Utility.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/ProjectileManager.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Effect/MarioEffect.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Object/Mario.hpp"
#include "Object/Projectile/GearProjectileEffect.hpp"

GearProjectile::GearProjectile(ProjectileManager &manager, const sf::Vector2f &position, const sf::Vector2f& velocity)
    : Projectile(manager),
    m_transform(position, sf::Vector2f(20.f, 20.f), sf::degrees(0.f)){
    m_animation.setTexture("GearProjectile", true);
    m_hitbox = sf::FloatRect({0.f, 0.f}, {40.f, 40.f});
    m_timeEffect = 0.f;
    m_timeEffectMax = 2.5f;
    m_velocity = velocity;
    setDrawingPriority(2);
}

void GearProjectile::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void GearProjectile::FireballEffect() const {}

void GearProjectile::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    //Spin
    m_timeEffect += deltaTime;
    if (m_timeEffect >= m_timeEffectMax) {
        m_timeEffect = Utility::f_mod(m_timeEffect, m_timeEffectMax);
        m_manager.addProjectile<GearProjectileEffect>(m_transform.getCurrentPosition(), m_transform.getCurrentAngle(), m_velocity.x > 0.f);
    }
    if (m_velocity.x < 0.f) m_transform.rotate(- sf::degrees(11.5f * deltaTime));
    else m_transform.rotate(sf::degrees(11.5f * deltaTime));
    //Status
    if (Scroll::isOutOfScreenYBottom(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0.f)) {
        Destroy();
        return;
    }
    //Movement
    m_transform.setCurrentPosition(sf::Vector2f(m_transform.getCurrentPosition().x + m_velocity.x * deltaTime, m_transform.getCurrentPosition().y + m_velocity.y * deltaTime));
    m_velocity.y += (m_velocity.y >= 10.0f ? 0.0f : deltaTime * 0.175f);
}

void GearProjectile::CollisionUpdate() {
    if (isDestroyed()) return;
    if (EffectActive) return;
    const sf::FloatRect playerHitbox = getGlobalHitbox(Mario::getHitbox(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (sf::FloatRect loopHitbox = getGlobalHitbox(getHitbox(), m_transform.getCurrentPosition(), getOrigin()); isCollide(loopHitbox, playerHitbox)) {
        Mario::PowerDown();
    }
}

void GearProjectile::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 8.f)) return;
    m_animation.setAnimationDirection(m_velocity.x > 0.f);
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha) - sf::Vector2f(0.f, 0.f), getOrigin() - sf::Vector2f(0.f, 0.f));
    m_animation.setRotation(m_transform.getInterpolatedAngle(alpha));
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), m_transform.getCurrentPosition() - getOrigin(), sf::Color::Red));
}

void GearProjectile::Destroy() {
    m_transform.destroy();
    m_manager.setDeletionFlag(true);
}

void GearProjectile::LevelEndCleanup() {
    AddScoreEffect(ScoreID::SCORE_100, m_transform.getCurrentPosition().x, m_transform.getCurrentPosition().y - getOrigin().y);
    Destroy();
}

void GearProjectile::animationUpdate(float deltaTime) {}

sf::Vector2f GearProjectile::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f GearProjectile::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect GearProjectile::getHitbox() {
    return m_hitbox;
}

bool GearProjectile::isDestroyed() {
    return m_transform.isDestroyed();
}
