#include "Object/Enemy/GreenPiranhaGround.hpp"

#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/EnemyManager.hpp"
#include "Object/Mario.hpp"
#include "Object/PiranhaAI.hpp"
#include "Object/Spike.hpp"

GreenPiranhaGround::GreenPiranhaGround(EnemyManager &manager, const sf::Vector2f &position) : Enemy(manager) {
    setCurrentPosition(position);
    setPreviousPosition(getCurrentPosition());
    setInterpolatedPosition(getCurrentPosition());
    m_animation.setAnimationSequence(PiranhaGroundAnimName);
    m_animation.setAnimation(0, 3, 24);
    setHitbox(sf::FloatRect({0.f, 0.f}, {32.f, 32.f}));
    setOrigin(sf::Vector2f(0.f, 0.f));

    setDirection(false);
    setDisabled(true);
    setCollideEachOther(false);

    setShellKicking(false);
    setShellBlocker(false);
    setDrawingPriority(0);
}

void GreenPiranhaGround::setPreviousData() {
    if (isDestroyed() || isDisabled()) return;
    setPreviousPosition(getCurrentPosition());
}

void GreenPiranhaGround::interpolateData(float alpha) {
    if (isDestroyed() || isDisabled()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}

void GreenPiranhaGround::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;

    if (!isOutScreen(getCurrentPosition().x, getCurrentPosition().y, 64.f, 64.f))
        if (isDisabled()) setDisabled(false);
}

void GreenPiranhaGround::MarioCollision(float MarioYVelocity) {
    if (isDestroyed() || isDisabled()) return;
    if (f_abs(player.curr.x - getCurrentPosition().x) >= 80.f) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(player.hitboxMain, player.curr, player.property.getOrigin());
    if (const sf::FloatRect PiranhaAIHitbox = getGlobalHitbox(getHitbox(), getCurrentPosition(), getOrigin()); isCollide(PiranhaAIHitbox, hitbox_mario)) {
        PowerDown();
    }
}

void GreenPiranhaGround::XUpdate(float deltaTime) {}
void GreenPiranhaGround::YUpdate(float deltaTime) {}
void GreenPiranhaGround::EnemyCollision() {}

void GreenPiranhaGround::draw() {
    m_animation.setAnimationDirection(static_cast<AnimationDirection>(!getDirection()));
    if (isOutScreen(getInterpolatedPosition().x - getOrigin().x, getInterpolatedPosition().y, 64, 64)) return;
    m_animation.setColor(sf::Color(255, 255, 255));
    m_animation.AnimationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.AnimationDraw();
}
void GreenPiranhaGround::Destroy() {
    if (!isDestroyed()) {
        setDestroyed(true);
        m_enemyManager.setDeletionFlag(true);
    }
}
void GreenPiranhaGround::Death(unsigned int state) {
    Destroy();
}
void GreenPiranhaGround::BlockHit() {}
void GreenPiranhaGround::ShellHit() {}
bool GreenPiranhaGround::isDeath() {
    return false;
}
