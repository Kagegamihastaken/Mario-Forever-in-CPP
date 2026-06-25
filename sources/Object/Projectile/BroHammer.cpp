#include "Object/Projectile/BroHammer.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Scroll.hpp"
#include "Core/Utility.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/ProjectileManager.hpp"
#include "Effect/MarioEffect.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Object/Mario.hpp"

BroHammer::BroHammer(ProjectileManager &manager, const bool direction, const sf::Vector2f &position)
    : Projectile(manager),
    m_direction(direction),
    m_transform(position, sf::Vector2f(13.f, 18.f), sf::degrees(0.f)){
    m_animation.setTexture("Hammer", true);
    m_hitbox = sf::FloatRect({0.f, 0.f}, {24.f, 24.f});
    m_velocity = {1.f + static_cast<float>(Utility::RandomIntNumberGenerator(0, 4)), (6.f + static_cast<float>(Utility::RandomIntNumberGenerator(0, 4))) * -1.f};
    setDrawingPriority(2);
}

void BroHammer::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void BroHammer::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    //Spin
    if (m_direction) m_transform.rotate(- sf::degrees(5.f * deltaTime));
    else m_transform.rotate(sf::degrees(5.f * deltaTime));
    //Status
    if (Scroll::isOutOfScreenYBottom(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0.f)) {
        Destroy();
        return;
    }
    //Movement
    m_transform.setCurrentPosition(sf::Vector2f(m_transform.getCurrentPosition().x - m_velocity.x * (m_direction ? 1.f : -1.f) * deltaTime, m_transform.getCurrentPosition().y + m_velocity.y * deltaTime));
    m_velocity.y += (m_velocity.y >= 10.0f ? 0.0f : deltaTime * 0.215f);
}

void BroHammer::CollisionUpdate() {
    if (isDestroyed()) return;
    if (EffectActive) return;
    const sf::FloatRect playerHitbox = getGlobalHitbox(Mario::getHitbox(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (sf::FloatRect loopHitbox = getGlobalHitbox(getHitbox(), m_transform.getCurrentPosition(), getOrigin()); isCollide(loopHitbox, playerHitbox)) {
        Mario::PowerDown();
    }
}

void BroHammer::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 16.f)) return;
    m_animation.setAnimationDirection(m_direction);
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha) - sf::Vector2f(0.f, 7.f), getOrigin() - sf::Vector2f(0.f, 9.f));
    m_animation.setRotation(m_transform.getInterpolatedAngle(alpha));
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), m_transform.getCurrentPosition() - getOrigin(), sf::Color::Red));
}

void BroHammer::Destroy() {
    m_transform.destroy();
    m_manager.setDeletionFlag(true);
}

void BroHammer::LevelEndCleanup() {
    AddScoreEffect(ScoreID::SCORE_100, m_transform.getCurrentPosition().x, m_transform.getCurrentPosition().y - getOrigin().y);
    Destroy();
}

void BroHammer::animationUpdate(float deltaTime) {}

sf::Vector2f BroHammer::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f BroHammer::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect BroHammer::getHitbox() {
    return m_hitbox;
}

bool BroHammer::isDestroyed() {
    return m_transform.isDestroyed();
}
