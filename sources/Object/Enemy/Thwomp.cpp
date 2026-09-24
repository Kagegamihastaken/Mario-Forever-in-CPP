#include "Object/Enemy/Thwomp.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Scroll.hpp"

Thwomp::Thwomp(EnemyManager &manager, const sf::Vector2f &position)
    : Enemy(manager),
    m_transform(position, sf::Vector2f(32.f, 65.f), sf::degrees(0.f)) {
    m_animation.setAnimation(0, 0, 100, true);
    m_animation.setAnimationSequence("ThwompAnimName");
    m_hitbox = sf::FloatRect({0.f, 0.f}, {64.f, 66.f});
    m_velocity = sf::Vector2f(0.f, 0.f);
    setDirection(false);
    setDisabled(true);
    setCollideEachOther(false);
    setShellKicking(false);
    setShellBlocker(false);
    setDrawingPriority(0);
}

void Thwomp::updatePreviousData() {
    if (isDestroyed() || isDisabled()) return;
    m_transform.Update();
}

void Thwomp::EnemyCollision() {

}

void Thwomp::MarioCollision(float MarioYVelocity) {

}

void Thwomp::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;

    if (!Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 96)) {
        if (isDisabled()) setDisabled(false);
    }
}

void Thwomp::XUpdate(float deltaTime) {

}

void Thwomp::YUpdate(float deltaTime) {
    // move
    
}


void Thwomp::BlockHit() {

}

void Thwomp::ShellHit() {

}

void Thwomp::Destroy() {

}

void Thwomp::draw(float alpha) {
    if (isDestroyed() || isDisabled()) return;
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) {
        m_animation.frameUpdate();
        return;
    }
    m_animation.setColor(sf::Color(255, 255, 255));
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), m_transform.getCurrentPosition() - getOrigin(), sf::Color::Red));
}

void Thwomp::Death(unsigned int state) {

}

bool Thwomp::isDeath() {
    return false;
}

void Thwomp::animationUpdate(float deltaTime) {

}

sf::Vector2f Thwomp::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f Thwomp::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect Thwomp::getHitbox() {
    return m_hitbox;
}

bool Thwomp::isDestroyed() {
    return m_transform.isDestroyed();
}
