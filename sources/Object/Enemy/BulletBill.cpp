#include "Object/Enemy/BulletBill.hpp"

#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/EnemyManager.hpp"
#include "Core/Object/Enemy/GoombaAIBehavior.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Object/Mario.hpp"

BulletBill::BulletBill(EnemyManager &manager, const sf::Vector2f &position, const float speed, const bool direction) : Enemy(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_animation.setTexture("BulletBillNormal_0");
    setHitbox(sf::FloatRect({0.f, 0.f}, {34.f, 28.f}));
    setOrigin(sf::Vector2f(16.f, 28.f));
    m_velocity = sf::Vector2f(speed,0.f);
    setDirection(direction);
    setDisabled(true);
    setCollideEachOther(false);
    m_state = 0;
    setShellKicking(false);
    setShellBlocker(false);
    setDrawingLowerPriority(false);
}

void BulletBill::setPreviousData() {
    if (isDestroyed() || isDisabled()) return;
    setPreviousPosition(getCurrentPosition());
}

void BulletBill::interpolateData(const float alpha) {
    if (isDestroyed() || isDisabled()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}

void BulletBill::EnemyCollision() {}
void BulletBill::MarioCollision(float MarioYVelocity) {
    if (isDestroyed() || isDisabled() || m_state > 0) return;
    if (f_abs(player.curr.x - getCurrentPosition().x) >= 80.f) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(player.hitboxMain, player.curr, player.property.getOrigin());
    if (const sf::FloatRect other = getGlobalHitbox(getHitbox(), getCurrentPosition(), getOrigin()); isCollide(other, hitbox_mario)) {
        if (getCurrentPosition().y - 16.f > player.curr.y && MarioYVelocity > 0.f) {
            GoombaAIBehavior::GoombaAIStomping();
            AddScoreEffect(SCORE_100, getCurrentPosition().x, getCurrentPosition().y - getOrigin().y);
            Death(1);
            return;
        }
        if (getCurrentPosition().y - 16.f < player.curr.y)
            PowerDown();
    }
}

void BulletBill::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    if (m_state == 0) {
        if (isOutScreen(getCurrentPosition().x, getCurrentPosition().y, 512, 512))
            Destroy();
        if (!isOutScreen(getCurrentPosition().x - getOrigin().x, getCurrentPosition().y, 32, 80)) {
            if (isDisabled()) setDisabled(false);
        }
    }
    else if (m_state == 1)
        if (isOutScreenYBottom(getCurrentPosition().y, 64)) Destroy();
}
void BulletBill::XUpdate(const float deltaTime) {
    if (isDestroyed() || isDisabled()) return;
    const auto data = GoombaAIBehavior::GoombaAIXMove(GoombaAIBehavior::GoombaAIData(getCurrentPosition(), m_velocity, getDirection()), deltaTime);
    setCurrentPosition(data.position);
    m_velocity = data.velocity;
    setDirection(data.direction);
}
void BulletBill::YUpdate(const float deltaTime) {
    if (isDestroyed() || isDisabled() || m_state < 1) return;
    const auto data = GoombaAIBehavior::BulletBillEffectYMove(GoombaAIBehavior::GoombaAIData(getCurrentPosition(), m_velocity, getDirection()), deltaTime);
    m_velocity = data.velocity;
    setCurrentPosition(data.position);
    setDirection(data.direction);
}
void BulletBill::BlockHit() {}
void BulletBill::ShellHit() {}
void BulletBill::Destroy() {
    if (!isDestroyed()) {
        setDestroyed(true);
        m_enemyManager.setDeletionFlag(true);
    }
}
void BulletBill::draw() {
    m_animation.setAnimationDirection(!getDirection());
    if (isOutScreen(getInterpolatedPosition().x - getOrigin().x, getInterpolatedPosition().y, 32, 80)) return;
    m_animation.setColor(sf::Color(255, 255, 255));
    m_animation.AnimationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.AnimationDraw();
}

void BulletBill::Death(unsigned int state) {
    m_state = state;
}

bool BulletBill::isDeath() {
    return true;
}
