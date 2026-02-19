#include "Object/Enemy/PinkPiranhaUp.hpp"

#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/EnemyManager.hpp"
#include "Core/Object/Enemy/Behavior/PiranhaAIBehavior.hpp"
#include "Object/Mario.hpp"
#include "Object/PiranhaAI.hpp"
#include "Projectiles/PiranhaProjectile.hpp"

PinkPiranhaUp::PinkPiranhaUp(EnemyManager &manager, const sf::Vector2f &position) : Enemy(manager) {
    setCurrentPosition(PiranhaAIBehavior::PiranhaPositionAdjust(PIRANHA_UP, position));
    setPreviousPosition(getCurrentPosition());
    setInterpolatedPosition(getCurrentPosition());
    m_animation.setAnimationSequence(PinkPiranhaUpAnimName);
    m_animation.setAnimation(0, 1, 24);
    setHitbox(sf::FloatRect({16.f, 17.f}, {31.f, 47.f}));
    setOrigin(sf::Vector2f(32.f, 63.f));

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

void PinkPiranhaUp::setPreviousData() {
    if (isDestroyed() || isDisabled()) return;
    setPreviousPosition(getCurrentPosition());
}

void PinkPiranhaUp::interpolateData(float alpha) {
    if (isDestroyed() || isDisabled()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}

void PinkPiranhaUp::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;

    if (!isOutScreen(getCurrentPosition().x, getCurrentPosition().y, 64.f, 64.f))
        if (isDisabled()) setDisabled(false);

    //Movement
    PiranhaAIBehavior::PiranhaAIData data = PiranhaAIBehavior::PiranhaMovementUpdate(PiranhaAIBehavior::PiranhaAIData(
        getCurrentPosition(), m_moving_stop, m_moving_state, m_speed, m_position_moving, m_position_limit,
        m_stop_clock, m_stop_time, m_distance_appear, m_fire_counting, m_fire_count, m_fire_ticking, m_fire_interval), PIRANHA_UP, deltaTime
    );
    bool fire = false;
    data = PiranhaAIBehavior::PiranhaFireUpdate(fire, data, deltaTime);
    setCurrentPosition(data.pos);
    m_position_moving = data.pos_temp;
    m_moving_stop = data.stop;
    m_moving_state = data.state;
    m_stop_clock = data.stop_clock;

    m_fire_counting = data.fire_counting;
    m_fire_ticking = data.fire_ticking;
    if (fire) {
        SoundManager::PlaySound("Fireball");
        AddPiranhaAIProjectile(static_cast<bool>(m_animation.getAnimationDirection()), PIRANHA_UP, PIRANHA_FIREBALL, getCurrentPosition().x, getCurrentPosition().y - 51.f);
    }
}

void PinkPiranhaUp::MarioCollision(float MarioYVelocity) {
    if (isDestroyed() || isDisabled()) return;
    if (f_abs(player.curr.x - getCurrentPosition().x) >= 80.f) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(player.hitboxMain, player.curr, player.property.getOrigin());
    if (const sf::FloatRect PiranhaAIHitbox = getGlobalHitbox(getHitbox(), getCurrentPosition(), getOrigin()); isCollide(PiranhaAIHitbox, hitbox_mario)) {
        PowerDown();
    }
}

void PinkPiranhaUp::XUpdate(float deltaTime) {}
void PinkPiranhaUp::YUpdate(float deltaTime) {}
void PinkPiranhaUp::EnemyCollision() {}

void PinkPiranhaUp::draw() {
    m_animation.setAnimationDirection(static_cast<AnimationDirection>(!getDirection()));
    if (isOutScreen(getInterpolatedPosition().x - getOrigin().x, getInterpolatedPosition().y, 64, 64)) return;
    m_animation.setColor(sf::Color(255, 255, 255));
    m_animation.AnimationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.AnimationDraw();
}
void PinkPiranhaUp::Destroy() {
    if (!isDestroyed()) {
        setDestroyed(true);
        m_enemyManager.setDeletionFlag(true);
    }
}
void PinkPiranhaUp::Death(unsigned int state) {
    Destroy();
}
void PinkPiranhaUp::BlockHit() {}
void PinkPiranhaUp::ShellHit() {}
bool PinkPiranhaUp::isDeath() {
    return false;
}
