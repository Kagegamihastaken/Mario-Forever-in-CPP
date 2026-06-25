#include "Object/Enemy/GreenPiranhaGround.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Scroll.hpp"
#include "Core/Utility.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/EnemyManager.hpp"
#include "Object/Mario.hpp"

GreenPiranhaGround::GreenPiranhaGround(EnemyManager &manager, const sf::Vector2f &position)
    : Enemy(manager),
    m_transform(position, sf::Vector2f(0.f, 0.f), sf::degrees(0.f)){
    m_animation.setAnimationSequence("PiranhaGroundAnimName");
    m_animation.setAnimation(0, 3, 24, true);
    m_hitbox = sf::FloatRect({9.f, 7.f}, {13.f, 25.f});

    setDirection(false);
    setDisabled(true);
    setCollideEachOther(false);

    setShellKicking(false);
    setShellBlocker(false);
    setDrawingPriority(0);
}

void GreenPiranhaGround::updatePreviousData() {
    if (isDestroyed() || isDisabled()) return;
    m_transform.Update();
}

void GreenPiranhaGround::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;

    if (!Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0))
        if (isDisabled()) setDisabled(false);
}

void GreenPiranhaGround::MarioCollision(float MarioYVelocity) {
    if (isDestroyed() || isDisabled()) return;
    if (Utility::f_abs(Mario::getCurrentPosition().x - m_transform.getCurrentPosition().x) >= 80.f) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(Mario::getHitbox(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (const sf::FloatRect PiranhaAIHitbox = getGlobalHitbox(getHitbox(), m_transform.getCurrentPosition(), getOrigin()); isCollide(PiranhaAIHitbox, hitbox_mario)) {
        Mario::PowerDown();
    }
}

void GreenPiranhaGround::XUpdate(float deltaTime) {}
void GreenPiranhaGround::YUpdate(float deltaTime) {}
void GreenPiranhaGround::EnemyCollision() {}

void GreenPiranhaGround::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) {
        m_animation.frameUpdate();
        return;
    }
    m_animation.setColor(sf::Color(255, 255, 255));
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), m_transform.getCurrentPosition(), sf::Color::Red));
}
void GreenPiranhaGround::Destroy() {
    if (!isDestroyed()) {
        m_transform.destroy();
        m_enemyManager.setDeletionFlag(true);
    }
}
void GreenPiranhaGround::Death(unsigned int state) {
    Destroy();
}
void GreenPiranhaGround::BlockHit() {}
void GreenPiranhaGround::ShellHit() {}
bool GreenPiranhaGround::isDeath() {
    return false;
}

void GreenPiranhaGround::animationUpdate(float deltaTime) {
    m_animation.frameTimeAccumulate(deltaTime);
}

sf::Vector2f GreenPiranhaGround::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f GreenPiranhaGround::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect GreenPiranhaGround::getHitbox() {
    return m_hitbox;
}

bool GreenPiranhaGround::isDestroyed() {
    return m_transform.isDestroyed();
}
