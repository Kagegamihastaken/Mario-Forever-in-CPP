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

BroFireball::BroFireball(ProjectileManager &manager, const bool direction, const sf::Vector2f &position)
    : Projectile(manager),
    m_direction(direction),
    m_transform(position, sf::Vector2f(7.f, 16.f), sf::degrees(0.f)){
    m_animation.setTexture("Fireball", true);
    m_hitbox = sf::FloatRect({0.f, 0.f}, {15.f, 16.f});
    m_velocity = {8.125f, 0.f};
    setDrawingPriority(2);
}

void BroFireball::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void BroFireball::FireballEffect() const {
    AddFireballExplosion(m_transform.getCurrentPosition().x, m_transform.getCurrentPosition().y - 7.f);
}

void BroFireball::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    //Spin
    if (m_direction) m_transform.rotate(- sf::degrees(11.5f * deltaTime));
    else m_transform.rotate(sf::degrees(11.5f * deltaTime));
    //Status
    if (Scroll::isOutOfScreenYBottom(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0.f) ||
        Scroll::isOutOfScreenXLeft(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0.f) ||
        Scroll::isOutOfScreenXRight(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0.f)) {
        Destroy();
        return;
    }
    //Movement
    auto data = FireballBehavior::FireballX(m_transform.getCurrentPosition(), m_velocity, m_direction, deltaTime, getHitbox(), getOrigin());
    if (data.remove) {
        FireballEffect();
        Destroy();
        return;
    }
    m_transform.setCurrentPosition(data.position);
    m_velocity = data.velocity;
    bool bounce = false;
    data = FireballBehavior::FireballY(m_transform.getCurrentPosition(), bounce, m_velocity, deltaTime, getHitbox(), getOrigin());
    if (data.remove) {
        FireballEffect();
        Destroy();
        return;
    }
    if (bounce)
        m_velocity.y = -5.f;
    m_transform.setCurrentPosition(data.position);
    if (!bounce) m_velocity.y = data.velocity.y;
}

void BroFireball::CollisionUpdate() {
    if (isDestroyed()) return;
    if (EffectActive) return;
    const sf::FloatRect playerHitbox = getGlobalHitbox(Mario::getHitbox(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (sf::FloatRect loopHitbox = getGlobalHitbox(getHitbox(), m_transform.getCurrentPosition(), getOrigin()); isCollide(loopHitbox, playerHitbox)) {
        Mario::PowerDown();
        FireballEffect();
        Destroy();
    }
}

void BroFireball::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 8.f)) return;
    m_animation.setAnimationDirection(m_direction);
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha) - sf::Vector2f(0.f, 7.f), getOrigin() - sf::Vector2f(0.f, 9.f));
    m_animation.setRotation(m_transform.getInterpolatedAngle(alpha));
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), m_transform.getCurrentPosition() - getOrigin(), sf::Color::Red));
}

void BroFireball::Destroy() {
    m_transform.destroy();
    m_manager.setDeletionFlag(true);
}

void BroFireball::LevelEndCleanup() {
    AddScoreEffect(ScoreID::SCORE_100, m_transform.getCurrentPosition().x, m_transform.getCurrentPosition().y - getOrigin().y);
    Destroy();
}

void BroFireball::animationUpdate(float deltaTime) {}
sf::Vector2f BroFireball::getPosition() {
    return m_transform.getCurrentPosition();
}
sf::Vector2f BroFireball::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect BroFireball::getHitbox() {
    return m_hitbox;
}

bool BroFireball::isDestroyed() {
    return m_transform.isDestroyed();
}

