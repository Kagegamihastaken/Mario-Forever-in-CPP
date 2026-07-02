#include "Object/Enemy/FireBro.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/EnemyManager.hpp"
#include "Core/Object/Enemy/Behavior/BroAIBehavior.hpp"
#include "Core/Object/Enemy/Behavior/GoombaAIBehavior.hpp"
#include "Core/HitboxUtils.hpp"
#include "Core/Utility.hpp"
#include "Effect/MarioEffect.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Object/BroAI.hpp"
#include "Object/Mario.hpp"
#include "Projectiles/BroAIProjectile.hpp"

FireBro::FireBro(EnemyManager &manager, const sf::Vector2f &position)
    : Enemy(manager),
    m_transform(position, sf::Vector2f(24.f, 63.f), sf::degrees(0.f)){
    m_animation.setAnimation(0, 1, 14, true);
    m_animation.setAnimationSequence("FireBroAnimName");
    m_hitbox = sf::FloatRect({7.f, 16.f}, {32.f, 48.f});
    m_wall_hitbox = sf::FloatRect(getHitbox().position, getHitbox().size - sf::Vector2f(0.f, 6.f));
    m_velocity = sf::Vector2f(2.f, 0.f);
    setDirection(false);
    setDisabled(true);
    setCollideEachOther(false);
    m_state = 0;
    setShellKicking(true);
    setShellBlocker(false);
    setDrawingPriority(0);
    //walking value
    m_movingValue = 0.f;
    m_WalkingState = 1;
    m_randomWalking = 0.f;
    m_timeTicking = 0.f;
    m_stop_duration = 100.f;
    m_tickingValue = BroAIBehavior::m_fixed_ticking_value;
    m_RNG_walking_range = 64.f;
    m_RNG_ticking_range = 5.f;
    m_isFalling = false;
    m_timeCounter = 0.f;
    m_lastY = -1.f;

    //launching value
    m_launchTickingTime = 0.f;
    m_LaunchInterval = 3.f;
    m_launchIntervalTicking = 0.f;
    m_launchRNG = 10.f;
    m_launchWaitTime = 30.f;
    m_launchCount = 1;
    m_launchCounting = m_launchCount;
    m_launchIntervalBetween = 0.f;
    m_launchIntervalBetweenTicking = m_launchIntervalBetween;

    m_IsLaunching = false;
    m_lastIsLaunching = m_IsLaunching;
}

void FireBro::updatePreviousData() {
    if (isDestroyed() || isDisabled()) return;
    m_transform.Update();
}
void FireBro::EnemyCollision() {}
void FireBro::MarioCollision(const float MarioYVelocity) {
    if (isDestroyed() || isDisabled() || m_state != 0) return;
    if (Utility::f_abs(Mario::getCurrentPosition().x - m_transform.getCurrentPosition().x) >= 80.f) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(Mario::getHitbox(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (const sf::FloatRect BroAIHitbox = getGlobalHitbox(getHitbox(), m_transform.getCurrentPosition(), getOrigin()); isCollide(BroAIHitbox, hitbox_mario)) {
        if (m_transform.getCurrentPosition().y - 16.f >= Mario::getCurrentPosition().y && MarioYVelocity > 0.f) {
            GoombaAIBehavior::GoombaAIStomping();
            AddScoreEffect(ScoreID::SCORE_200, m_transform.getCurrentPosition().x, m_transform.getCurrentPosition().y - getOrigin().y);
            Death(1);
            return;
        }
        if (m_transform.getCurrentPosition().y - 16.f < Mario::getCurrentPosition().y)
            Mario::PowerDown();
    }
}

void FireBro::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    // Status Update
    if (m_state == 0) {
        if (Scroll::isOutOfScreenYBottom(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0))
            Destroy();
        if (!Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0))
            if (isDisabled()) {
                setDisabled(false);
                return;
            }
        if (!EffectActive) {
            if (m_transform.getCurrentPosition().x > Mario::getCurrentPosition().x) m_animation.setAnimationDirection(ANIM_LEFT);
            else m_animation.setAnimationDirection(ANIM_RIGHT);
        }
        else m_animation.setAnimationDirection(ANIM_LEFT);
    }
    else {
        if (Scroll::isOutOfScreenYBottom(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0)) Destroy();
    }

    // Shooting Update
    if (isDisabled() || m_state != 0) return;
    BroAIBehavior::BroAIShootData data(m_launchTickingTime, m_LaunchInterval, m_launchIntervalTicking, m_launchRNG, m_launchWaitTime, m_launchCount, m_launchCounting, m_launchIntervalBetween, m_launchIntervalBetweenTicking);
    const BroAIBehavior::BroAIShootingData ShootingData = BroAIBehavior::ShootUpdate(GoombaAIBehavior::GoombaAIData(m_transform.getCurrentPosition(), m_velocity, getDirection()), data, getOrigin(), getHitbox(), m_IsLaunching, deltaTime);

    m_launchTickingTime = data.launchTickingTime;
    m_launchIntervalTicking = data.launchIntervalTicking;
    m_launchCounting = data.launchCounting;
    m_launchIntervalBetweenTicking = data.launchIntervalBetweenTicking;

    m_IsLaunching = ShootingData.willFire;
    if (m_IsLaunching != m_lastIsLaunching) {
        m_lastIsLaunching = m_IsLaunching;
        AnimationUpdate(m_IsLaunching);
    }
    if (ShootingData.isFire) {
        if (m_animation.getAnimationDirection() == AnimationDirection::ANIM_RIGHT)
            AddBroAIProjectile(static_cast<bool>(m_animation.getAnimationDirection()), BroAIProjectileType::FIREBALL, m_transform.getCurrentPosition().x + 6.f, m_transform.getCurrentPosition().y - 21.f);
        else
            AddBroAIProjectile(static_cast<bool>(m_animation.getAnimationDirection()), BroAIProjectileType::FIREBALL, m_transform.getCurrentPosition().x - 6.f, m_transform.getCurrentPosition().y - 21.f);
        SoundManager::PlaySound(SoundID::GAME_FIREBALL);
    }
}

void FireBro::AnimationUpdate(const bool val) {
    if (val)
        m_animation.setAnimationSequence("FireBroLaunchAnimName");
    else
        m_animation.setAnimationSequence("FireBroAnimName");
}

void FireBro::XUpdate(const float deltaTime) {
    if (isDestroyed() || isDisabled() || m_state != 0) return;

    BroAIBehavior::BroAIData data = {m_transform.getCurrentPosition(), m_movingValue, m_timeTicking, m_tickingValue, m_velocity.x, m_stop_duration, m_randomWalking, getDirection(), m_WalkingState};
    data = BroAIBehavior::BroAIXUpdate(data, m_RNG_walking_range, m_RNG_ticking_range, deltaTime);
    m_movingValue = data.movingValue;
    m_timeTicking = data.timeTicking;
    m_tickingValue = data.tickingValue;
    m_stop_duration = data.stop_duration;
    m_randomWalking = data.randomWalking;
    setDirection(data.directionMoving);
    m_WalkingState = data.state;
    m_transform.setCurrentPosition(data.position);
    m_velocity.x = data.speed;
    const auto CollisionData = BroAIBehavior::BroAIXCollision(GoombaAIBehavior::GoombaAIData(m_transform.getCurrentPosition(), sf::Vector2f(0.f, 0.f), getDirection()), getHitbox(), m_wall_hitbox, getOrigin());
    m_transform.setCurrentPosition(CollisionData);
}
void FireBro::YUpdate(float deltaTime) {
    if (isDestroyed() || isDisabled()) return;
    if (m_state == 0) {
        GoombaAIBehavior::GoombaAIData data = BroAIBehavior::BroAIYUpdateMovement(GoombaAIBehavior::GoombaAIData(m_transform.getCurrentPosition(), m_velocity, getDirection()), deltaTime);
        data = BroAIBehavior::BroAIYCollision(data, getHitbox(), getOrigin(), m_timeCounter, m_isFalling, m_lastY, true, deltaTime);
        m_transform.setCurrentPosition(data.position);
        m_velocity.y = data.velocity.y;
    }
    else {
        const auto data = GoombaAIBehavior::GoombaAIEffectYMove(GoombaAIBehavior::GoombaAIData(m_transform.getCurrentPosition(), m_velocity, getDirection()), deltaTime);
        m_velocity = data.velocity;
        m_transform.setCurrentPosition(data.position);
    }
}
void FireBro::BlockHit() {
    if (m_state != 0) return;
    AddScoreEffect(ScoreID::SCORE_200, m_transform.getCurrentPosition().x, m_transform.getCurrentPosition().y - getOrigin().y);
    SoundManager::PlaySound(SoundID::GAME_KICK2);
    Death(1);
}
void FireBro::Death(unsigned int state) {
    m_state = state;
    m_velocity = sf::Vector2f(0.f, 0.f);
    m_animation.setAnimationSequence("FireBroDeath");
    m_animation.setAnimation(0, 0, 100, true);
    setShellKicking(false);
    setShellBlocker(false);
    setDrawingPriority(3);
    if (m_transform.getCurrentPosition().x > Mario::getCurrentPosition().x) m_animation.setAnimationDirection(ANIM_LEFT);
    else m_animation.setAnimationDirection(ANIM_RIGHT);
    setDisabled(false);
}

void FireBro::Destroy() {
    if (!isDestroyed()) {
        m_transform.destroy();
        m_enemyManager.setDeletionFlag(true);
    }
}

void FireBro::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) {
        m_animation.frameUpdate();
        return;
    }
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), m_transform.getCurrentPosition() - getOrigin(), sf::Color::Red));
}
bool FireBro::isDeath() {
    return m_state != 0;
}
void FireBro::ShellHit() {
    if (m_state != 0) return;
    Death(1);
}

sf::Vector2f FireBro::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f FireBro::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect FireBro::getHitbox() {
    return m_hitbox;
}

bool FireBro::isDestroyed() {
    return m_transform.isDestroyed();
}

void FireBro::animationUpdate(float deltaTime) {
    m_animation.frameTimeAccumulate(deltaTime);
}
