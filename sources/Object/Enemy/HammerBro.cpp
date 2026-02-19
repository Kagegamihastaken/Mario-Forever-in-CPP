#include "Object/Enemy/HammerBro.hpp"

#include "Core/Interpolation.hpp"
#include "Core/Logging.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/EnemyManager.hpp"
#include "../../../headers/Core/Object/Enemy/Behavior/BroAIBehavior.hpp"
#include "../../../headers/Core/Object/Enemy/Behavior/GoombaAIBehavior.hpp"
#include "Effect/BroAIEffect.hpp"
#include "Effect/MarioEffect.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Object/BroAI.hpp"
#include "Object/Mario.hpp"
#include "Projectiles/BroAIProjectile.hpp"

HammerBro::HammerBro(EnemyManager &manager, const sf::Vector2f &position) : Enemy(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_animation.setAnimation(0, 1, 14, true);
    m_animation.setAnimationSequence(HammerBroAnimName);
    setHitbox(sf::FloatRect({7.f, 16.f}, {32.f, 48.f}));
    m_wall_hitbox = sf::FloatRect(getHitbox().position, getHitbox().size - sf::Vector2f(0.f, 6.f));
    setOrigin(sf::Vector2f(24.f, 63.f));
    m_velocity = sf::Vector2f(2.f, 0.f);
    setDirection(false);
    setDisabled(true);
    setCollideEachOther(false);
    m_state = 0;
    setShellKicking(true);
    setShellBlocker(false);
    setDrawingPriority(1);
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

void HammerBro::setPreviousData() {
    if (isDestroyed() || isDisabled()) return;
    setPreviousPosition(getCurrentPosition());
}
void HammerBro::interpolateData(float alpha) {
    if (isDestroyed() || isDisabled()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}

void HammerBro::EnemyCollision() {}
void HammerBro::MarioCollision(const float MarioYVelocity) {
    if (isDestroyed() || isDisabled() || m_state != 0) return;
    if (f_abs(player.curr.x - getCurrentPosition().x) >= 80.f) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(player.hitboxMain, player.curr, player.property.getOrigin());
    if (const sf::FloatRect BroAIHitbox = getGlobalHitbox(getHitbox(), getCurrentPosition(), getOrigin()); isCollide(BroAIHitbox, hitbox_mario)) {
        if (getCurrentPosition().y - 16.f >= player.curr.y && MarioYVelocity > 0.f) {
            GoombaAIBehavior::GoombaAIStomping();
            AddScoreEffect(SCORE_200, getCurrentPosition().x, getCurrentPosition().y - getOrigin().y);
            Death(1);
            return;
        }
        if (getCurrentPosition().y - 16.f < player.curr.y)
            PowerDown();
    }
}

void HammerBro::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    // Status Update
    if (m_state == 0) {
        if (isOutScreenYBottom(getCurrentPosition().y, 80))
            Destroy();
        if (!isOutScreen(getCurrentPosition().x, getCurrentPosition().y, 64.f, 64.f))
            if (isDisabled()) {
                setDisabled(false);
                return;
            }
        if (!EffectActive) {
            if (getCurrentPosition().x > player.curr.x) m_animation.setAnimationDirection(ANIM_LEFT);
            else m_animation.setAnimationDirection(ANIM_RIGHT);
        }
        else m_animation.setAnimationDirection(ANIM_LEFT);
    }
    else {
        if (isOutScreenYBottom(getCurrentPosition().y, 64.f)) Destroy();
    }

    // Shooting Update
    if (isDisabled() || m_state != 0) return;
    BroAIBehavior::BroAIShootData data(m_launchTickingTime, m_LaunchInterval, m_launchIntervalTicking, m_launchRNG, m_launchWaitTime, m_launchCount, m_launchCounting, m_launchIntervalBetween, m_launchIntervalBetweenTicking);
    const BroAIBehavior::BroAIShootingData ShootingData = BroAIBehavior::ShootUpdate(GoombaAIBehavior::GoombaAIData(getCurrentPosition(), m_velocity, getDirection()), data, m_IsLaunching, deltaTime);

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
            AddBroAIProjectile(static_cast<bool>(m_animation.getAnimationDirection()), BROAI_HAMMER, getCurrentPosition().x + 5.f, getCurrentPosition().y - 31.f);
        else
            AddBroAIProjectile(static_cast<bool>(m_animation.getAnimationDirection()), BROAI_HAMMER, getCurrentPosition().x - 5.f, getCurrentPosition().y - 31.f);
        SoundManager::PlaySound("Hammer");
    }
}

void HammerBro::AnimationUpdate(const bool val) {
    if (val)
        m_animation.setAnimationSequence(HammerBroLaunchAnimName);
    else
        m_animation.setAnimationSequence(HammerBroAnimName);
}

void HammerBro::XUpdate(const float deltaTime) {
    if (isDestroyed() || isDisabled() || m_state != 0) return;

    BroAIBehavior::BroAIData data = {getCurrentPosition(), m_movingValue, m_timeTicking, m_tickingValue, m_velocity.x, m_stop_duration, m_randomWalking, getDirection(), m_WalkingState};
    data = BroAIBehavior::BroAIXUpdate(data, m_RNG_walking_range, m_RNG_ticking_range, deltaTime);
    m_movingValue = data.movingValue;
    m_timeTicking = data.timeTicking;
    m_tickingValue = data.tickingValue;
    m_stop_duration = data.stop_duration;
    m_randomWalking = data.randomWalking;
    setDirection(data.directionMoving);
    m_WalkingState = data.state;
    setCurrentPosition(data.position);
    m_velocity.x = data.speed;
    const auto CollisionData = BroAIBehavior::BroAIXCollision(GoombaAIBehavior::GoombaAIData(getCurrentPosition(), sf::Vector2f(0.f, 0.f), getDirection()), getHitbox(), m_wall_hitbox, getOrigin());
    setCurrentPosition(CollisionData);
}
void HammerBro::YUpdate(float deltaTime) {
    if (isDestroyed() || isDisabled()) return;
    if (m_state == 0) {
        GoombaAIBehavior::GoombaAIData data = BroAIBehavior::BroAIYUpdateMovement(GoombaAIBehavior::GoombaAIData(getCurrentPosition(), m_velocity, getDirection()), deltaTime);
        data = BroAIBehavior::BroAIYCollision(data, getHitbox(), getOrigin(), m_timeCounter, m_isFalling, m_lastY, true, deltaTime);
        setCurrentPosition(data.position);
        m_velocity.y = data.velocity.y;
    }
    else {
        const auto data = GoombaAIBehavior::GoombaAIEffectYMove(GoombaAIBehavior::GoombaAIData(getCurrentPosition(), m_velocity, getDirection()), deltaTime);
        m_velocity = data.velocity;
        setCurrentPosition(data.position);
    }
}
void HammerBro::BlockHit() {
    if (m_state != 0) return;
    AddScoreEffect(SCORE_200, getCurrentPosition().x, getCurrentPosition().y - getOrigin().y);
    SoundManager::PlaySound("Kick2");
    Death(1);
}
void HammerBro::Death(unsigned int state) {
    m_state = state;
    m_velocity = sf::Vector2f(0.f, 0.f);
    m_animation.setAnimationSequence(HammerBroDeath);
    m_animation.setAnimation(0, 0, 100, true);
    setShellKicking(false);
    setShellBlocker(false);
    setDrawingPriority(2);
    if (getCurrentPosition().x > player.curr.x) m_animation.setAnimationDirection(ANIM_LEFT);
    else m_animation.setAnimationDirection(ANIM_RIGHT);
}

void HammerBro::Destroy() {
    if (!isDestroyed()) {
        setDestroyed(true);
        m_enemyManager.setDeletionFlag(true);
    }
}

void HammerBro::draw() {
    if (isOutScreen(getInterpolatedPosition().x - getOrigin().x, getInterpolatedPosition().y, 32, 80)) return;
    m_animation.AnimationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.AnimationDraw();
}
bool HammerBro::isDeath() {
    return m_state != 0;
}
void HammerBro::ShellHit() {
    if (m_state != 0) return;
    Death(1);
}
