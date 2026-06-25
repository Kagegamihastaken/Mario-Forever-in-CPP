#include "Object/Enemy/BulletBill.hpp"

#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/EnemyManager.hpp"
#include "Core/Object/Enemy/Behavior/GoombaAIBehavior.hpp"
#include "Core/HitboxUtils.hpp"
#include "Core/Utility.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Object/Mario.hpp"

BulletBill::BulletBill(EnemyManager &manager, const sf::Vector2f &position, const float speed, const bool direction)
    : Enemy(manager),
    m_transform(position, sf::Vector2f(16.f, 28.f), sf::degrees(0.f)){
    m_animation.setTexture("BulletBillNormal_0");
    m_hitbox = sf::FloatRect({0.f, 0.f}, {34.f, 28.f});
    m_velocity = sf::Vector2f(speed,0.f);
    setDirection(direction);
    setDisabled(true);
    setCollideEachOther(false);
    m_state = 0;
    m_timePass = 0.f;
    setShellKicking(false);
    setShellBlocker(false);
    setDrawingPriority(2);
}

void BulletBill::updatePreviousData() {
    if (isDestroyed() || isDisabled()) return;
    m_transform.Update();
}

void BulletBill::EnemyCollision() {}
void BulletBill::MarioCollision(float MarioYVelocity) {
    if (isDestroyed() || isDisabled() || m_state > 0) return;
    if (Utility::f_abs(Mario::getCurrentPosition().x - m_transform.getCurrentPosition().x) >= 80.f) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(Mario::getHitbox(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (const sf::FloatRect other = getGlobalHitbox(getHitbox(), m_transform.getCurrentPosition(), getOrigin()); isCollide(hitbox_mario, other)) {
        if (m_transform.getCurrentPosition().y - 16.f > Mario::getCurrentPosition().y && MarioYVelocity > 0.f) {
            GoombaAIBehavior::GoombaAIStomping();
            AddScoreEffect(ScoreID::SCORE_100, m_transform.getCurrentPosition().x, m_transform.getCurrentPosition().y - getOrigin().y);
            Death(1);
            return;
        }
        if (m_transform.getCurrentPosition().y - 16.f < Mario::getCurrentPosition().y)
            Mario::PowerDown();
    }
}

void BulletBill::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    if (m_state == 0) {
        if (m_timePass >= 33.f) {
            setDrawingPriority(3);
            m_timePass = 33.f;
        }
        else m_timePass += deltaTime;

        if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 480))
            Destroy();
        if (!Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0)) {
            if (isDisabled()) setDisabled(false);
        }
    }
    else if (m_state == 1)
        if (Scroll::isOutOfScreenYBottom(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0)) Destroy();
}
void BulletBill::XUpdate(const float deltaTime) {
    if (isDestroyed() || isDisabled()) return;
    const auto data = GoombaAIBehavior::GoombaAIXMove(GoombaAIBehavior::GoombaAIData(m_transform.getCurrentPosition(), m_velocity, getDirection()), deltaTime);
    m_transform.setCurrentPosition(data.position);
    m_velocity = data.velocity;
    setDirection(data.direction);
}
void BulletBill::YUpdate(const float deltaTime) {
    if (isDestroyed() || isDisabled() || m_state < 1) return;
    const auto data = GoombaAIBehavior::BulletBillEffectYMove(GoombaAIBehavior::GoombaAIData(m_transform.getCurrentPosition(), m_velocity, getDirection()), deltaTime);
    m_velocity = data.velocity;
    m_transform.setCurrentPosition(data.position);
    setDirection(data.direction);
}
void BulletBill::BlockHit() {}
void BulletBill::ShellHit() {}
void BulletBill::Destroy() {
    if (!isDestroyed()) {
        m_transform.destroy();
        m_enemyManager.setDeletionFlag(true);
    }
}
void BulletBill::draw(float alpha) {
    m_animation.setAnimationDirection(!getDirection());
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) return;
    m_animation.setColor(sf::Color(255, 255, 255));
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), m_transform.getCurrentPosition() - getOrigin(), sf::Color::Red));
}

void BulletBill::Death(unsigned int state) {
    m_state = state;
    if (m_state == 1)
        setDrawingPriority(3);
}

bool BulletBill::isDeath() {
    return true;
}

sf::Vector2f BulletBill::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f BulletBill::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect BulletBill::getHitbox() {
    return m_hitbox;
}

bool BulletBill::isDestroyed() {
    return m_transform.isDestroyed();
}

void BulletBill::animationUpdate(float deltaTime) {}
