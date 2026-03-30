#include "Object/Projectile/BroHammer.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/ProjectileManager.hpp"
#include "Effect/FireballExplosion.hpp"
#include "Effect/MarioEffect.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Object/Mario.hpp"

BroHammer::BroHammer(ProjectileManager &manager, const bool direction, const sf::Vector2f &position) : Projectile(manager), m_direction(direction) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_animation.setTexture("Hammer", true);
    setOrigin(sf::Vector2f(13.f, 18.f));
    setHitbox(sf::FloatRect({0.f, 0.f}, {24.f, 24.f}));
    m_velocity = {1.f + static_cast<float>(RandomIntNumberGenerator(0, 4)), (6.f + static_cast<float>(RandomIntNumberGenerator(0, 4))) * -1.f};
}

void BroHammer::setPreviousData() {
    if (isDestroyed()) return;
    setPreviousPosition(getCurrentPosition());
    setPreviousAngle(getCurrentAngle());
}
void BroHammer::interpolateData(const float alpha) {
    if (isDestroyed()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
    setInterpolatedAngle(linearInterpolation(getPreviousAngle(), getCurrentAngle(), alpha));
}

void BroHammer::FireballEffect() const {
    AddFireballExplosion(getCurrentPosition().x, getCurrentPosition().y - 7.f);
}

void BroHammer::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    //Spin
    if (m_direction) setCurrentAngle(getCurrentAngle() - sf::degrees(5.f * deltaTime));
    else setCurrentAngle(getCurrentAngle() + sf::degrees(5.f * deltaTime));
    //Status
    if (isOutOfScreenYBottom(MFCPP::CollisionObject(getCurrentPosition(), getOrigin(), getHitbox()), 32.f)) {
        Destroy();
        return;
    }
    //Movement
    setCurrentPosition(sf::Vector2f(getCurrentPosition().x - m_velocity.x * (m_direction ? 1.f : -1.f) * deltaTime, getCurrentPosition().y + m_velocity.y * deltaTime));
    m_velocity.y += (m_velocity.y >= 10.0f ? 0.0f : deltaTime * 0.215f);
}

void BroHammer::CollisionUpdate() {
    if (isDestroyed()) return;
    if (EffectActive) return;
    const sf::FloatRect playerHitbox = getGlobalHitbox(player.hitboxMain, player.curr, player.property.getOrigin());
    if (sf::FloatRect loopHitbox = getGlobalHitbox(getHitbox(), getCurrentPosition(), getOrigin()); isCollide(loopHitbox, playerHitbox)) {
        PowerDown();
    }
}

void BroHammer::draw() {
    //if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 32.f)) return;
    m_animation.setAnimationDirection(m_direction);
    m_animation.AnimationUpdate(getInterpolatedPosition() - sf::Vector2f(0.f, 7.f), getOrigin() - sf::Vector2f(0.f, 9.f));
    m_animation.setRotation(getInterpolatedAngle());
    m_animation.AnimationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), getCurrentPosition() - getOrigin(), sf::Color::Red));
}

void BroHammer::Destroy() {
    setDestroyed(true);
    m_manager.setDeletionFlag(true);
}

void BroHammer::LevelEndCleanup() {
    AddScoreEffect(SCORE_100, getCurrentPosition().x, getCurrentPosition().y - getOrigin().y);
    Destroy();
}
