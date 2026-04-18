#include "Object/Enemy/CastleSpikeUp.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/EnemyManager.hpp"
#include "Object/Mario.hpp"
#include "Object/PiranhaAI.hpp"
#include "Object/Spike.hpp"

CastleSpikeUp::CastleSpikeUp(EnemyManager &manager, const sf::Vector2f &position) : Enemy(manager) {
    setCurrentPosition(position);
    setPreviousPosition(getCurrentPosition());
    setInterpolatedPosition(getCurrentPosition());
    m_animation.setTexture("CastleSpikeUp");
    setHitbox(sf::FloatRect({9.f, 7.f}, {13.f, 25.f}));
    setOrigin(sf::Vector2f(0.f, 0.f));

    setDirection(false);
    setDisabled(true);
    setCollideEachOther(false);

    setShellKicking(false);
    setShellBlocker(false);
    setDrawingPriority(0);
}

void CastleSpikeUp::setPreviousData() {
    if (isDestroyed() || isDisabled()) return;
    setPreviousPosition(getCurrentPosition());
}

void CastleSpikeUp::interpolateData(float alpha) {
    if (isDestroyed() || isDisabled()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}

void CastleSpikeUp::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;

    if (!isOutScreen(getCurrentPosition().x, getCurrentPosition().y, 64.f, 64.f))
        if (isDisabled()) setDisabled(false);
}

void CastleSpikeUp::MarioCollision(float MarioYVelocity) {
    if (isDestroyed() || isDisabled()) return;
    if (f_abs(Mario::getCurrentPosition().x - getCurrentPosition().x) >= 80.f) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(Mario::getHitbox(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (const sf::FloatRect PiranhaAIHitbox = getGlobalHitbox(getHitbox(), getCurrentPosition(), getOrigin()); isCollide(PiranhaAIHitbox, hitbox_mario)) {
        Mario::PowerDown();
    }
}

void CastleSpikeUp::XUpdate(float deltaTime) {}
void CastleSpikeUp::YUpdate(float deltaTime) {}
void CastleSpikeUp::EnemyCollision() {}

void CastleSpikeUp::draw() {
    m_animation.setAnimationDirection(static_cast<AnimationDirection>(getDirection()));
    if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 32.f)) return;
    m_animation.setColor(sf::Color(255, 255, 255));
    m_animation.animationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), getCurrentPosition(), sf::Color::Red));
}
void CastleSpikeUp::Destroy() {
    if (!isDestroyed()) {
        setDestroyed(true);
        m_enemyManager.setDeletionFlag(true);
    }
}
void CastleSpikeUp::Death(unsigned int state) {
    Destroy();
}
void CastleSpikeUp::BlockHit() {}
void CastleSpikeUp::ShellHit() {}
bool CastleSpikeUp::isDeath() {
    return false;
}
