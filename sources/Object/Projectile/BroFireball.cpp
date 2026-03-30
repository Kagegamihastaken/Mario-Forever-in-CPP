#include "Object/Projectile/BroFireball.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/ProjectileManager.hpp"
#include "Core/Object/Projectile/Behavior/FireballBehavior.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Effect/FireballExplosion.hpp"
#include "Effect/MarioEffect.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Object/Mario.hpp"

BroFireball::BroFireball(ProjectileManager &manager, const bool direction, const sf::Vector2f &position) : Projectile(manager), m_direction(direction) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_animation.setTexture("Fireball", true);
    setOrigin(sf::Vector2f(7.f, 16.f));
    setHitbox(sf::FloatRect({0.f, 0.f}, {15.f, 16.f}));
    m_velocity = {8.125f, 0.f};
}

void BroFireball::setPreviousData() {
    if (isDestroyed()) return;
    setPreviousPosition(getCurrentPosition());
    setPreviousAngle(getCurrentAngle());
}
void BroFireball::interpolateData(const float alpha) {
    if (isDestroyed()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
    setInterpolatedAngle(linearInterpolation(getPreviousAngle(), getCurrentAngle(), alpha));
}

void BroFireball::FireballEffect() const {
    AddFireballExplosion(getCurrentPosition().x, getCurrentPosition().y - 7.f);
}

void BroFireball::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    //Spin
    if (m_direction) setCurrentAngle(getCurrentAngle() - sf::degrees(11.5f * deltaTime));
    else setCurrentAngle(getCurrentAngle() + sf::degrees(11.5f * deltaTime));
    //Status
    if (isOutOfScreen(MFCPP::CollisionObject(getCurrentPosition(), getOrigin(), getHitbox()), 32.f)) {
        Destroy();
        return;
    }
    //Movement
    auto data = FireballBehavior::FireballX(getCurrentPosition(), m_velocity, m_direction, deltaTime, getHitbox(), getOrigin());
    if (data.remove) {
        FireballEffect();
        Destroy();
        return;
    }
    setCurrentPosition(data.position);
    m_velocity = data.velocity;
    data = FireballBehavior::FireballY(getCurrentPosition(), m_velocity, deltaTime, getHitbox(), getOrigin());
    if (data.remove) {
        FireballEffect();
        Destroy();
        return;
    }
    setCurrentPosition(data.position);
    m_velocity = data.velocity;
}

void BroFireball::CollisionUpdate() {
    if (isDestroyed()) return;
    if (EffectActive) return;
    const sf::FloatRect playerHitbox = getGlobalHitbox(player.hitboxMain, player.curr, player.property.getOrigin());
    if (sf::FloatRect loopHitbox = getGlobalHitbox(getHitbox(), getCurrentPosition(), getOrigin()); isCollide(loopHitbox, playerHitbox)) {
        PowerDown();
        FireballEffect();
        Destroy();
    }
}

void BroFireball::draw() {
    //if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 32.f)) return;
    m_animation.setAnimationDirection(m_direction);
    m_animation.AnimationUpdate(getInterpolatedPosition() - sf::Vector2f(0.f, 7.f), getOrigin() - sf::Vector2f(0.f, 9.f));
    m_animation.setRotation(getInterpolatedAngle());
    m_animation.AnimationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), getCurrentPosition() - getOrigin(), sf::Color::Red));
}

void BroFireball::Destroy() {
    setDestroyed(true);
    m_manager.setDeletionFlag(true);
}

void BroFireball::LevelEndCleanup() {
    AddScoreEffect(SCORE_100, getCurrentPosition().x, getCurrentPosition().y - getOrigin().y);
    Destroy();
}
