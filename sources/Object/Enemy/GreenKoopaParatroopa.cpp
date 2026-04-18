#include "Object/Enemy/GreenKoopaParatroopa.hpp"
#include "Core/Object/EnemyManager.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/Enemy/Behavior/GoombaAIBehavior.hpp"
#include "Core/HitboxUtils.hpp"
#include "Core/Object/Enemy/Behavior/RotodiscAIBehavior.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Object/GoombaAI.hpp"
#include "Effect/GoombaAIEffect.hpp"
#include "Effect/MarioEffect.hpp"
#include "Object/Mario.hpp"
#include "Object/Enemy/GreenKoopa.hpp"

GreenKoopaParatroopa::GreenKoopaParatroopa(EnemyManager &manager, const sf::Vector2f& position, const float angle) : Enemy(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_origin_pos = position;
    setDirection(false);
    setDisabled(true);
    setCollideEachOther(false);
    setDrawingPriority(1);
    setHitbox(sf::FloatRect({0.f, 0.f}, {33.f, 47.f}));
    m_wall_hitbox = sf::FloatRect(getHitbox().position, getHitbox().size - sf::Vector2f(0.f, 6.f));
    setOrigin(sf::Vector2f(16.f, 46.f));
    m_velocity = sf::Vector2f(0.f, 0.f);
    m_max_radius = sf::Vector2f(0.f, 50.f);
    m_speed = 2.f;
    m_angle = angle;
    m_animation.setAnimation(0, 1, 11, true);
    m_animation.setAnimationSequence(GreenKoopaParatroopaAnimName);
    m_state = 0;
    setShellKicking(true);
    setShellBlocker(false);
}
void GreenKoopaParatroopa::setPreviousData() {
    if (isDestroyed() || isDisabled()) return;
    setPreviousPosition(getCurrentPosition());
}
void GreenKoopaParatroopa::interpolateData(const float alpha) {
    if (isDestroyed() || isDisabled()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}
void GreenKoopaParatroopa::EnemyCollision() {}

void GreenKoopaParatroopa::MarioCollision(const float MarioYVelocity) {
    if (isDestroyed() || isDisabled() || m_state == 1) return;
    if (f_abs(Mario::getCurrentPosition().x - getCurrentPosition().x) >= 80.0f) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(Mario::getHitbox(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (const sf::FloatRect GoombaAIHitbox = getGlobalHitbox(getHitbox(), getCurrentPosition(), getOrigin()); isCollide(GoombaAIHitbox, hitbox_mario)) {
        if (m_state == 0) {
            if (getCurrentPosition().y - 16.f > Mario::getCurrentPosition().y && MarioYVelocity > 0.0f) {
                GoombaAIBehavior::GoombaAIStomping();
                AddScoreEffect(SCORE_100, getCurrentPosition().x, getCurrentPosition().y - getOrigin().y);
                m_enemyManager.addEnemy<GreenKoopa>(getCurrentPosition(), false);
                Destroy();
                return;
            }
            if (getCurrentPosition().y - 16.f < Mario::getCurrentPosition().y)
                Mario::PowerDown();
        }
    }
}
void GreenKoopaParatroopa::statusUpdate(const float deltaTime) {
    if (isDestroyed()) return;

    if (m_state != 1) {
        if (!EffectActive) {
            if (getCurrentPosition().x > Mario::getCurrentPosition().x) setDirection(false);
            else setDirection(true);
        }
        else setDirection(false);
        m_angle = RotodiscAIBehavior::RotodiscAngleUpdate(m_angle, m_speed, deltaTime);
        if (isOutScreenYBottom(getCurrentPosition().y, 80))
            Destroy();
        if (!isOutScreen(getCurrentPosition().x - getOrigin().x, getCurrentPosition().y, 32, 80)) {
            if (isDisabled()) setDisabled(false);
        }
    }
    else
        if (isOutScreenYBottom(getCurrentPosition().y, 64)) Destroy();
}
void GreenKoopaParatroopa::XUpdate(const float deltaTime) {
    if (isDestroyed() || isDisabled() || m_state == 1) return;
    if (m_state != 1) {
        setCurrentPosition(RotodiscAIBehavior::RotodiscRoundXUpdate(getCurrentPosition(), m_origin_pos, m_angle, m_max_radius.x));
    }
    else {
        const auto data = GoombaAIBehavior::ShellXCollision(GoombaAIBehavior::GoombaAIData(getCurrentPosition(), m_velocity, getDirection()), getHitbox(), m_wall_hitbox, getOrigin());
        setCurrentPosition(data.position);
        m_velocity = data.velocity;
        setDirection(data.direction);
    }
}
void GreenKoopaParatroopa::YUpdate(const float deltaTime) {
    if (isDestroyed() || isDisabled()) return;
    if (m_state != 1) {
        setCurrentPosition(RotodiscAIBehavior::RotodiscRoundYUpdate(getCurrentPosition(), m_origin_pos, m_angle, m_max_radius.y));
    }
    else {
        const auto data = GoombaAIBehavior::GoombaAIEffectYMove(GoombaAIBehavior::GoombaAIData(getCurrentPosition(), m_velocity, getDirection()), deltaTime);
        m_velocity = data.velocity;
        setCurrentPosition(data.position);
    }
}

void GreenKoopaParatroopa::BlockHit() {}

void GreenKoopaParatroopa::Destroy() {
    if (!isDestroyed()) {
        setDestroyed(true);
        m_enemyManager.setDeletionFlag(true);
    }
}

void GreenKoopaParatroopa::draw() {
    if (m_state == 0) m_animation.setAnimationDirection(static_cast<AnimationDirection>(!getDirection()));
    if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 32.f)) return;
    m_animation.setColor(sf::Color(255, 255, 255));
    m_animation.animationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), getCurrentPosition() - getOrigin(), sf::Color::Red));
}

void GreenKoopaParatroopa::ChangeState() {
    switch (m_state) {
        case 1:
            setHitbox(sf::FloatRect({0.f, 0.f}, {32.f, 28.f}));
            m_wall_hitbox = sf::FloatRect(getHitbox().position, getHitbox().size - sf::Vector2f(0.f, 6.f));
            setOrigin(sf::Vector2f(16,27));
            m_velocity = sf::Vector2f(0.f, -3.f);
            m_animation.setAnimationSequence(GreenKoopaDeathEffect);
            m_animation.setAnimation(0,0,100);
            setCollideEachOther(false);
            setShellBlocker(false);
            setShellKicking(false);
            setDrawingPriority(2);
            setDisabled(false);
            break;
        default:;
    }
}

void GreenKoopaParatroopa::Death(unsigned int state) {
    m_state = state;
    ChangeState();
}
void GreenKoopaParatroopa::ShellHit() {
    if (m_state == 1) return;
    Death(1);
}

bool GreenKoopaParatroopa::isDeath() {
    return m_state == 1;
}
