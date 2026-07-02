#include "Object/Enemy/GreenPiranhaLeft.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Utility.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/EnemyManager.hpp"
#include "Core/Object/Enemy/Behavior/PiranhaAIBehavior.hpp"
#include "Object/Mario.hpp"

GreenPiranhaLeft::GreenPiranhaLeft(EnemyManager &manager, const sf::Vector2f &position)
    : Enemy(manager),
    m_transform(PiranhaAIBehavior::PiranhaPositionAdjust(PiranhaDirection::LEFT, position), sf::Vector2f(0.f, 32.f), sf::degrees(0.f)){
    m_animation.setAnimationSequence("GreenPiranhaLeftAnimName");
    m_animation.setAnimation(0, 3, 24, true);
    m_hitbox = sf::FloatRect({ 0.f, 17.f }, { 47.f, 31.f });

    m_speed = 1.f;
    m_stop_time = 70.f;
    m_stop_clock = 0.f;
    m_distance_appear = 80.f;

    m_fire_count = 0; // 3
    m_fire_interval = 0.f; // 10.f

    m_position_limit = 64.f;
    m_position_moving = 64.f;
    m_moving_state = true;
    m_moving_stop = false;


    m_fire_counting = 0;
    m_fire_ticking = 0.f;

    setDirection(false);
    setDisabled(true);
    setCollideEachOther(false);

    setShellKicking(false);
    setShellBlocker(false);
    setDrawingPriority(0);
}

void GreenPiranhaLeft::updatePreviousData() {
    if (isDestroyed() || isDisabled()) return;
    m_transform.Update();
}

void GreenPiranhaLeft::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;

    if (!Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 32.f))
        if (isDisabled()) setDisabled(false);

    //Movement
    if (isDisabled()) return;
    PiranhaAIBehavior::PiranhaAIData data = PiranhaAIBehavior::PiranhaMovementUpdate(PiranhaAIBehavior::PiranhaAIData(
        m_transform.getCurrentPosition(), getOrigin(), getHitbox(), m_moving_stop, m_moving_state, m_speed, m_position_moving, m_position_limit,
        m_stop_clock, m_stop_time, m_distance_appear, m_fire_counting, m_fire_count, m_fire_ticking, m_fire_interval), PiranhaDirection::LEFT, deltaTime
    );
    m_transform.setCurrentPosition(data.pos);
    m_position_moving = data.pos_temp;
    m_moving_stop = data.stop;
    m_moving_state = data.state;
    m_stop_clock = data.stop_clock;
}

void GreenPiranhaLeft::MarioCollision(float MarioYVelocity) {
    if (isDestroyed() || isDisabled()) return;
    if (Utility::f_abs(Mario::getCurrentPosition().x - m_transform.getCurrentPosition().x) >= 80.f) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(Mario::getHitbox(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (const sf::FloatRect PiranhaAIHitbox = getGlobalHitbox(getHitbox(), m_transform.getCurrentPosition(), getOrigin()); isCollide(PiranhaAIHitbox, hitbox_mario)) {
        Mario::PowerDown();
    }
}

void GreenPiranhaLeft::XUpdate(float deltaTime) {}
void GreenPiranhaLeft::YUpdate(float deltaTime) {}
void GreenPiranhaLeft::EnemyCollision() {}

void GreenPiranhaLeft::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) {
        m_animation.frameUpdate();
        return;
    }
    m_animation.setColor(sf::Color(255, 255, 255));
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), m_transform.getCurrentPosition() - getOrigin(), sf::Color::Red));
}
void GreenPiranhaLeft::Destroy() {
    if (!isDestroyed()) {
        m_transform.destroy();
        m_enemyManager.setDeletionFlag(true);
    }
}
void GreenPiranhaLeft::Death(unsigned int state) {
    Destroy();
}
void GreenPiranhaLeft::BlockHit() {}
void GreenPiranhaLeft::ShellHit() {}
bool GreenPiranhaLeft::isDeath() {
    return isDestroyed();
}

void GreenPiranhaLeft::animationUpdate(float deltaTime) {
    m_animation.frameTimeAccumulate(deltaTime);
}

sf::Vector2f GreenPiranhaLeft::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f GreenPiranhaLeft::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect GreenPiranhaLeft::getHitbox() {
    return m_hitbox;
}

bool GreenPiranhaLeft::isDestroyed() {
    return m_transform.isDestroyed();
}
