#include "Object/Enemy/Thwomp.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Utility.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/Enemy/Behavior/ThwompAIBehavior.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Mario.hpp"

Thwomp::Thwomp(EnemyManager &manager, const sf::Vector2f &position)
    : Enemy(manager),
    m_transform(position, sf::Vector2f(32.f, 65.f), sf::degrees(0.f)) {
    m_animation.setAnimation(0, 0, 100, true);
    m_animation.setAnimationSequence("THWOMP");
    m_hitbox = sf::FloatRect({6.f, 0.f}, {53.f, 66.f});
    m_velocity = sf::Vector2f(0.f, 0.f);

    m_triggerRange = 96.f;
    m_rise = 75.f;
    m_y_velocity_rise = 1.f;

    m_yStore = position.y;
    m_decending = false;
    m_hitGround = false;
    m_countingBeforeRise = 0.f;

    m_AnimationSmile = false;
    m_AnimationBlink = false;
    m_AnimationBlinkTime = 0.f;

    setDirection(false);
    setDisabled(true);
    setCollideEachOther(false);
    setShellKicking(false);
    setShellBlocker(false);
    setDrawingPriority(0);
}

void Thwomp::ChangeAnimation() {
    if (m_AnimationSmile)
        m_animation.setAnimation(4, 15, 10, false);
    else if (m_AnimationBlink)
        m_animation.setAnimation(1, 3, 50, false);
    else
        m_animation.setAnimation(0, 0, 100, true);
}

void Thwomp::updatePreviousData() {
    if (isDestroyed() || isDisabled()) return;
    m_transform.Update();
}

void Thwomp::EnemyCollision() {}

void Thwomp::MarioCollision(const float MarioYVelocity) {
    if (isDestroyed() || isDisabled()) return;
    if (Utility::f_abs(Mario::getCurrentPosition().x - m_transform.getCurrentPosition().x) >= 80.0f) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(Mario::getHitbox(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (const sf::FloatRect GoombaAIHitbox = getGlobalHitbox(m_hitbox, m_transform.getCurrentPosition(), getOrigin()); isCollide(GoombaAIHitbox, hitbox_mario)) {
        if (!m_AnimationSmile) {
            SoundManager::PlaySound(SoundID::GAME_THWOMP);
            m_AnimationSmile = true;
            ChangeAnimation();
        }
        Mario::PowerDown();
    }
}

void Thwomp::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;

    if (m_transform.getCurrentPosition().x < Mario::getCurrentPosition().x + m_triggerRange &&
    m_transform.getCurrentPosition().x > Mario::getCurrentPosition().x - m_triggerRange &&
    !Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 64) &&
    !m_hitGround)
        m_decending = true;

    //Animation
    //Blinking
    m_AnimationBlinkTime += deltaTime;
    if (m_AnimationBlinkTime > 5.f) {
        m_AnimationBlinkTime = 0.f;
        if (Utility::RandomIntNumberGenerator(1, 20) == 1 && !m_AnimationBlink && !m_AnimationSmile) {
            m_AnimationBlink = true;
            ChangeAnimation();
        }
    }
    //Restore Animation
    if (m_AnimationSmile && m_animation.isAnimationAtTheEnd()) {
        m_AnimationSmile = false;
        ChangeAnimation();
    }
    if (m_AnimationBlink && m_animation.isAnimationAtTheEnd()) {
        m_AnimationBlink = false;
        ChangeAnimation();
    }

    if (!Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 96)) {
        if (isDisabled()) setDisabled(false);
    }
}

void Thwomp::XUpdate(float deltaTime) {}

void Thwomp::YUpdate(float deltaTime) {
    auto data = ThwompYStatusUpdate(ThwompAIBehavior::ThwompData(m_transform.getCurrentPosition(), m_velocity, m_decending, m_hitGround, m_countingBeforeRise, m_rise, m_y_velocity_rise, m_yStore), m_hitbox, getOrigin(), deltaTime);
    m_transform.setCurrentPosition(data.position);
    m_velocity = data.velocity;
    m_decending = data.decending;
    m_hitGround = data.hitGround;
    m_countingBeforeRise = data.countingBeforeRise;
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

void Thwomp::Death(unsigned int state) {}

bool Thwomp::isDeath() {
    return true;
}

void Thwomp::animationUpdate(float deltaTime) {
    m_animation.frameTimeAccumulate(deltaTime);
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
