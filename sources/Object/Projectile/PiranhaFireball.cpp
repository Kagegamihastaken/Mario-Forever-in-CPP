#include "Object/Projectile/PiranhaFireball.hpp"

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

PiranhaFireball::PiranhaFireball(ProjectileManager &manager, const PiranhaDirection direction, const sf::Vector2f &position) : Projectile(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_animation.setTexture("Fireball", true);
    setOrigin(sf::Vector2f(7.f, 16.f));
    setHitbox(sf::FloatRect({0.f, 0.f}, {15.f, 16.f}));
    setDrawingPriority(2);
    switch (direction) {
        case PIRANHA_UP:
            m_velocity = {static_cast<float>(RandomIntNumberGenerator(0, 4)) - static_cast<float>(RandomIntNumberGenerator(0, 4)),
    (3.f + static_cast<float>(RandomIntNumberGenerator(0, 8))) * -1.f
            };
            break;
        case PIRANHA_DOWN:
            m_velocity = {
            static_cast<float>(RandomIntNumberGenerator(0, 4)) - static_cast<float>(RandomIntNumberGenerator(0, 4)),
            (3.f + static_cast<float>(RandomIntNumberGenerator(0, 3)))
        };
            break;
        case PIRANHA_RIGHT:
            m_velocity = {
            (3.f + static_cast<float>(RandomIntNumberGenerator(0, 3))) * -1.f,
            static_cast<float>(RandomIntNumberGenerator(0, 9)) - static_cast<float>(RandomIntNumberGenerator(0, 9))
        };
            break;
        case PIRANHA_LEFT:
            m_velocity = {
            (3.f + static_cast<float>(RandomIntNumberGenerator(0, 3))),
            static_cast<float>(RandomIntNumberGenerator(0, 9)) - static_cast<float>(RandomIntNumberGenerator(0, 9))
        };
            break;
        default:
            m_velocity = {0.f, 0.f};
            break;
    }
}

void PiranhaFireball::setPreviousData() {
    if (isDestroyed()) return;
    setPreviousPosition(getCurrentPosition());
    setPreviousAngle(getCurrentAngle());
}
void PiranhaFireball::interpolateData(const float alpha) {
    if (isDestroyed()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
    setInterpolatedAngle(linearInterpolation(getPreviousAngle(), getCurrentAngle(), alpha));
}

void PiranhaFireball::FireballEffect() const {
    AddFireballExplosion(getCurrentPosition().x, getCurrentPosition().y - 7.f);
}

void PiranhaFireball::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    //Spin
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

void PiranhaFireball::CollisionUpdate() {
    if (isDestroyed()) return;
    if (EffectActive) return;
    const sf::FloatRect playerHitbox = getGlobalHitbox(Mario::getHitbox(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (sf::FloatRect loopHitbox = getGlobalHitbox(getHitbox(), getCurrentPosition(), getOrigin()); isCollide(loopHitbox, playerHitbox)) {
        Mario::PowerDown();
        FireballEffect();
        Destroy();
    }
}

void PiranhaFireball::draw() {
    if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 32.f)) return;
    m_animation.setAnimationDirection(m_velocity.x > 0.f);
    m_animation.animationUpdate(getInterpolatedPosition() - sf::Vector2f(0.f, 7.f), getOrigin() - sf::Vector2f(0.f, 9.f));
    m_animation.setRotation(getInterpolatedAngle());
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), getCurrentPosition() - getOrigin(), sf::Color::Red));
}

void PiranhaFireball::Destroy() {
    setDestroyed(true);
    m_manager.setDeletionFlag(true);
}

void PiranhaFireball::LevelEndCleanup() {
    AddScoreEffect(SCORE_100, getCurrentPosition().x, getCurrentPosition().y - getOrigin().y);
    Destroy();
}
