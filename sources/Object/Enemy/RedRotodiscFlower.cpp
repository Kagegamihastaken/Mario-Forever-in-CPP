#include "Object/Enemy/RedRotodiscFlower.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/EnemyManager.hpp"
#include "Core/Object/Enemy/RotodiscAIBehavior.hpp"
#include "Object/Mario.hpp"
#include "Object/RotodiscAI.hpp"

RedRotodiscFlower::RedRotodiscFlower(EnemyManager &manager, const sf::Vector2f &position, unsigned int arrange_mode, float radius, float speed, float speed_x_change) : Enemy(manager) {
    setCurrentPosition(RotodiscAIBehavior::RotodiscPositionAdjust(position, arrange_mode));
    setPreviousPosition(getCurrentPosition());
    setInterpolatedPosition(getCurrentPosition());
    m_position_center = getCurrentPosition();
    m_animation.setAnimation(0, 25, 100);
    m_animation.setAnimationSequence(RotodiscAnimName);
    m_animation_base.setTexture("RotodiscBase");
    setHitbox(sf::FloatRect({0.f, 0.f}, {34.f, 32.f}));
    setOrigin(sf::Vector2f(17.f, 16.f));
    setDirection(false);
    setDisabled(false);
    setCollideEachOther(false);
    m_radius = 0.f;
    m_max_radius = radius;
    m_change_state = false;
    m_speed = speed;
    m_speed_x_change = speed_x_change;
    setShellKicking(false);
    setShellBlocker(false);
    setDrawingLowerPriority(false);
    m_angle = 0.f;
}
void RedRotodiscFlower::setPreviousData() {
    if (isDestroyed() || isDisabled()) return;
    setPreviousPosition(getCurrentPosition());
}
void RedRotodiscFlower::interpolateData(const float alpha) {
    if (isDestroyed() || isDisabled()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}
void RedRotodiscFlower::EnemyCollision() {}
void RedRotodiscFlower::MarioCollision(float MarioYVelocity) {
    if (isDestroyed() || isDisabled()) return;
    if (f_abs(player.curr.x - getCurrentPosition().x) >= 80.f) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(player.hitboxMain, player.curr, player.property.getOrigin());
    if (const sf::FloatRect RotodiscHitbox = getGlobalHitbox(getHitbox(), getCurrentPosition(), getOrigin()); isCollide(RotodiscHitbox, hitbox_mario)) {
        PowerDown();
    }
}
void RedRotodiscFlower::statusUpdate(const float deltaTime) {
    if (isDestroyed() || isDisabled()) return;

    m_angle = RotodiscAIBehavior::RotodiscAngleUpdate(m_angle, m_speed, deltaTime);
    RotodiscAIBehavior::RotodiscRadiusChange(m_radius, m_change_state, m_max_radius, m_speed_x_change, deltaTime);
}
void RedRotodiscFlower::XUpdate(const float deltaTime) {
    if (isDestroyed() || isDisabled()) return;
    setCurrentPosition(RotodiscAIBehavior::RotodiscRoundXUpdate(getCurrentPosition(), m_position_center, m_angle, m_radius));
}
void RedRotodiscFlower::YUpdate(const float deltaTime) {
    if (isDestroyed() || isDisabled()) return;
    setCurrentPosition(RotodiscAIBehavior::RotodiscRoundYUpdate(getCurrentPosition(), m_position_center, m_angle, m_radius));
}
void RedRotodiscFlower::BlockHit() {}
void RedRotodiscFlower::ShellHit() {}
void RedRotodiscFlower::draw() {
    if (!isOutScreen(m_position_center.x, m_position_center.y, 64, 64)) {
        m_animation_base.AnimationUpdate(m_position_center, sf::Vector2f(16.f, 16.f));
        m_animation_base.AnimationDraw();
    }
    if (!isOutScreen(getInterpolatedPosition().x, getInterpolatedPosition().y, 64, 64)) {
        m_animation.setColor(sf::Color(255, 255, 255));
        m_animation.AnimationUpdate(getInterpolatedPosition(), getOrigin());
        m_animation.AnimationDraw();
    }
}
void RedRotodiscFlower::Destroy() {
    if (!isDestroyed()) {
        setDestroyed(true);
        m_enemyManager.setDeletionFlag(true);
    }
}
void RedRotodiscFlower::Death(unsigned int state) {
    Destroy();
}
bool RedRotodiscFlower::isDeath() {
    return true;
}
