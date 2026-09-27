#include "Object/Enemy/TankSpikeHorizon.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Scroll.hpp"
#include "Core/Utility.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/EnemyManager.hpp"
#include "Object/Mario.hpp"
#include "Core/Loading/PiranhaAILoading.hpp"

TankSpikeHorizon::TankSpikeHorizon(EnemyManager &manager, const sf::Vector2f &position, bool direction)
    : Enemy(manager),
    m_transform(position, sf::Vector2f(0.f, 0.f), sf::degrees(0.f)){
    //dir: true = right, false = left
    if (direction) {
        m_animation.setTexture("TANK_SPIKE_RIGHT");
        m_hitbox = sf::FloatRect({0.f, 9.f}, {25.f, 13.f});
    }
    else {
        m_animation.setTexture("TANK_SPIKE_LEFT");
        m_hitbox = sf::FloatRect({7.f, 9.f}, {25.f, 13.f});
    }

    setDirection(false);
    setDisabled(true);
    setCollideEachOther(false);

    setShellKicking(false);
    setShellBlocker(false);
    setDrawingPriority(0);
}

void TankSpikeHorizon::updatePreviousData() {
    if (isDestroyed() || isDisabled()) return;
    m_transform.Update();
}

void TankSpikeHorizon::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;

    if (!Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0))
        if (isDisabled()) setDisabled(false);
}

void TankSpikeHorizon::MarioCollision(float MarioYVelocity) {
    if (isDestroyed() || isDisabled()) return;
    if (Utility::f_abs(Mario::getCurrentPosition().x - m_transform.getCurrentPosition().x) >= 80.f) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(Mario::getHitbox(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (const sf::FloatRect PiranhaAIHitbox = getGlobalHitbox(getHitbox(), m_transform.getCurrentPosition(), getOrigin()); isCollide(PiranhaAIHitbox, hitbox_mario)) {
        Mario::PowerDown();
    }
}

void TankSpikeHorizon::XUpdate(float deltaTime) {}
void TankSpikeHorizon::YUpdate(float deltaTime) {}
void TankSpikeHorizon::EnemyCollision() {}

void TankSpikeHorizon::draw(float alpha) {
    m_animation.setAnimationDirection(static_cast<AnimationDirection>(getDirection()));
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) return;
    m_animation.setColor(sf::Color(255, 255, 255));
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), m_transform.getCurrentPosition(), sf::Color::Red));
}
void TankSpikeHorizon::Destroy() {
    if (!isDestroyed()) {
        m_transform.destroy();
        m_enemyManager.setDeletionFlag(true);
    }
}
void TankSpikeHorizon::Death(unsigned int state) {
    Destroy();
}
void TankSpikeHorizon::BlockHit() {}
void TankSpikeHorizon::ShellHit() {}
bool TankSpikeHorizon::isDeath() {
    return false;
}

sf::Vector2f TankSpikeHorizon::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f TankSpikeHorizon::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect TankSpikeHorizon::getHitbox() {
    return m_hitbox;
}

bool TankSpikeHorizon::isDestroyed() {
    return m_transform.isDestroyed();
}

void TankSpikeHorizon::animationUpdate(float deltaTime) {}
