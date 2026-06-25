#include "Object/Projectile/PiranhaFireball.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/Utility.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/ProjectileManager.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Effect/FireballExplosion.hpp"
#include "Effect/MarioEffect.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Object/Mario.hpp"

PiranhaFireball::PiranhaFireball(ProjectileManager &manager, const PiranhaDirection direction, const sf::Vector2f &position)
    : Projectile(manager),
    m_transform(position, sf::Vector2f(7.f, 16.f), sf::degrees(0.f)){
    m_animation.setTexture("Fireball", true);
    m_hitbox = sf::FloatRect({0.f, 0.f}, {15.f, 16.f});
    setDrawingPriority(2);
    switch (direction) {
        case PiranhaDirection::PIRANHA_UP:
            m_velocity = {static_cast<float>(Utility::RandomIntNumberGenerator(0, 4)) - static_cast<float>(Utility::RandomIntNumberGenerator(0, 4)),
    (3.f + static_cast<float>(Utility::RandomIntNumberGenerator(0, 8))) * -1.f
            };
            break;
        case PiranhaDirection::PIRANHA_DOWN:
            m_velocity = {
            static_cast<float>(Utility::RandomIntNumberGenerator(0, 4)) - static_cast<float>(Utility::RandomIntNumberGenerator(0, 4)),
            (3.f + static_cast<float>(Utility::RandomIntNumberGenerator(0, 3)))
        };
            break;
        case PiranhaDirection::PIRANHA_RIGHT:
            m_velocity = {
            (3.f + static_cast<float>(Utility::RandomIntNumberGenerator(0, 3))) * -1.f,
            static_cast<float>(Utility::RandomIntNumberGenerator(0, 9)) - static_cast<float>(Utility::RandomIntNumberGenerator(0, 9))
        };
            break;
        case PiranhaDirection::PIRANHA_LEFT:
            m_velocity = {
            (3.f + static_cast<float>(Utility::RandomIntNumberGenerator(0, 3))),
            static_cast<float>(Utility::RandomIntNumberGenerator(0, 9)) - static_cast<float>(Utility::RandomIntNumberGenerator(0, 9))
        };
            break;
        default:
            m_velocity = {0.f, 0.f};
            break;
    }
}

void PiranhaFireball::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void PiranhaFireball::FireballEffect() const {
    AddFireballExplosion(m_transform.getCurrentPosition().x, m_transform.getCurrentPosition().y - 7.f);
}

void PiranhaFireball::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    //Spin
    if (m_velocity.x < 0.f) m_transform.rotate(- sf::degrees(11.5f * deltaTime));
    else m_transform.rotate(sf::degrees(11.5f * deltaTime));
    //Status
    if (Scroll::isOutOfScreenYBottom(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0.f)) {
        Destroy();
        return;
    }
    //Movement
    m_transform.setCurrentPosition(sf::Vector2f(m_transform.getCurrentPosition().x + m_velocity.x * deltaTime, m_transform.getCurrentPosition().y + m_velocity.y * deltaTime));
    m_velocity.y += deltaTime * 0.175f;
}

void PiranhaFireball::CollisionUpdate() {
    if (isDestroyed()) return;
    if (EffectActive) return;
    const sf::FloatRect playerHitbox = getGlobalHitbox(Mario::getHitbox(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (sf::FloatRect loopHitbox = getGlobalHitbox(getHitbox(), m_transform.getCurrentPosition(), getOrigin()); isCollide(loopHitbox, playerHitbox)) {
        Mario::PowerDown();
        FireballEffect();
        Destroy();
    }
}

void PiranhaFireball::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 8.f)) return;
    m_animation.setAnimationDirection(m_velocity.x > 0.f);
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha) - sf::Vector2f(0.f, 7.f), getOrigin() - sf::Vector2f(0.f, 9.f));
    m_animation.setRotation(m_transform.getInterpolatedAngle(alpha));
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), m_transform.getCurrentPosition() - getOrigin(), sf::Color::Red));
}

void PiranhaFireball::Destroy() {
    m_transform.destroy();
    m_manager.setDeletionFlag(true);
}

void PiranhaFireball::LevelEndCleanup() {
    AddScoreEffect(ScoreID::SCORE_100, m_transform.getCurrentPosition().x, m_transform.getCurrentPosition().y - getOrigin().y);
    Destroy();
}

void PiranhaFireball::animationUpdate(float deltaTime) {}

sf::Vector2f PiranhaFireball::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f PiranhaFireball::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect PiranhaFireball::getHitbox() {
    return m_hitbox;
}

bool PiranhaFireball::isDestroyed() {
    return m_transform.isDestroyed();
}
