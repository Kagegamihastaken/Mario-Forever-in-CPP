#include "Object/Enemy/PinkPiranhaDown.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Utility.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/EnemyManager.hpp"
#include "Core/Object/Enemy/Behavior/PiranhaAIBehavior.hpp"
#include "Object/Mario.hpp"
#include "Projectiles/PiranhaProjectile.hpp"

PinkPiranhaDown::PinkPiranhaDown(EnemyManager &manager, const sf::Vector2f &position)
    : Enemy(manager),
    m_transform(PiranhaAIBehavior::PiranhaPositionAdjust(PiranhaDirection::PIRANHA_DOWN, position), sf::Vector2f(32.f, 0.f), sf::degrees(0.f)){
    m_animation.setAnimationSequence("PinkPiranhaDownAnimName");
    m_animation.setAnimation(0, 1, 24, true);
    m_hitbox = sf::FloatRect({16.f, 0.f}, {31.f, 47.f});

    m_speed = 1.f;
    m_stop_time = 70.f;
    m_stop_clock = 0.f;
    m_distance_appear = 80.f;

    m_fire_count = 3; // 3
    m_fire_interval = 10.f; // 10.f

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

void PinkPiranhaDown::updatePreviousData() {
    if (isDestroyed() || isDisabled()) return;
    m_transform.Update();
}

void PinkPiranhaDown::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;

    if (!Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 32.f))
        if (isDisabled()) setDisabled(false);

    //Movement
    if (isDisabled()) return;
    PiranhaAIBehavior::PiranhaAIData data = PiranhaAIBehavior::PiranhaMovementUpdate(PiranhaAIBehavior::PiranhaAIData(
        m_transform.getCurrentPosition(), getOrigin(), getHitbox(), m_moving_stop, m_moving_state, m_speed, m_position_moving, m_position_limit,
        m_stop_clock, m_stop_time, m_distance_appear, m_fire_counting, m_fire_count, m_fire_ticking, m_fire_interval), PiranhaDirection::PIRANHA_DOWN, deltaTime
    );
    bool fire = false;
    data = PiranhaAIBehavior::PiranhaFireUpdate(fire, data, deltaTime);
    m_transform.setCurrentPosition(data.pos);
    m_position_moving = data.pos_temp;
    m_moving_stop = data.stop;
    m_moving_state = data.state;
    m_stop_clock = data.stop_clock;

    m_fire_counting = data.fire_counting;
    m_fire_ticking = data.fire_ticking;
    if (fire) {
        SoundManager::PlaySound(SoundID::GAME_FIREBALL);
        AddPiranhaAIProjectile(static_cast<bool>(m_animation.getAnimationDirection()), PiranhaDirection::PIRANHA_DOWN, PiranhaProjectileType::PIRANHA_FIREBALL, m_transform.getCurrentPosition().x, m_transform.getCurrentPosition().y + 51.f);
    }
}

void PinkPiranhaDown::MarioCollision(float MarioYVelocity) {
    if (isDestroyed() || isDisabled()) return;
    if (Utility::f_abs(Mario::getCurrentPosition().x - m_transform.getCurrentPosition().x) >= 80.f) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(Mario::getHitbox(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (const sf::FloatRect PiranhaAIHitbox = getGlobalHitbox(getHitbox(), m_transform.getCurrentPosition(), getOrigin()); isCollide(PiranhaAIHitbox, hitbox_mario)) {
        Mario::PowerDown();
    }
}

void PinkPiranhaDown::XUpdate(float deltaTime) {}
void PinkPiranhaDown::YUpdate(float deltaTime) {}
void PinkPiranhaDown::EnemyCollision() {}

void PinkPiranhaDown::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) {
        m_animation.frameUpdate();
        return;
    }
    m_animation.setColor(sf::Color(255, 255, 255));
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), m_transform.getCurrentPosition() - getOrigin(), sf::Color::Red));
}
void PinkPiranhaDown::Destroy() {
    if (!isDestroyed()) {
        m_transform.destroy();
        m_enemyManager.setDeletionFlag(true);
    }
}
void PinkPiranhaDown::Death(unsigned int state) {
    Destroy();
}
void PinkPiranhaDown::BlockHit() {}
void PinkPiranhaDown::ShellHit() {}
bool PinkPiranhaDown::isDeath() {
    return isDestroyed();
}

void PinkPiranhaDown::animationUpdate(float deltaTime) {
    m_animation.frameTimeAccumulate(deltaTime);
}

sf::Vector2f PinkPiranhaDown::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f PinkPiranhaDown::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect PinkPiranhaDown::getHitbox() {
    return m_hitbox;
}

bool PinkPiranhaDown::isDestroyed() {
    return m_transform.isDestroyed();
}
