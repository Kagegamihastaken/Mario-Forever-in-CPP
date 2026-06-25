#include "Object/Platform/TankGround.hpp"

#include "Core/AutoScroll.hpp"
#include "Core/HitboxUtils.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/MovingBlockManager.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Effect/MarioEffect.hpp"
#include "Object/Mario.hpp"

TankGround::TankGround(MovingBlockManager &manager, const sf::Vector2f &pos)
    : MovingBlock(manager),
    m_transform(pos, sf::Vector2f(0.f, 0.f), sf::degrees(0.f)){
    m_hitbox = sf::FloatRect({0.f, 0.f}, {1920.f, 64.f});
    m_animation.setTexture("TankGround");
    m_prev_ViewX = MFCPP::AutoScroll::getPosition().x;
    m_step = false;
    setCanCollision(true);
}

void TankGround::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void TankGround::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    //Mario
    const sf::FloatRect CollideHitbox = getGlobalHitbox(Mario::getHitboxFloor(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (const sf::FloatRect PlatformHitbox = getGlobalHitbox(getHitbox(), m_transform.getPreviousPosition(), getOrigin()); isCollide(CollideHitbox, PlatformHitbox)) {
        if (!Mario::getCurrentFalling()) m_step = true;
    }
    else {
        if (!Mario::getCurrentFalling()) m_step = false;
    }
    if (EffectActive) {
        MoveMarioEffect(sf::Vector2f(sf::Vector2f(MFCPP::AutoScroll::getPosition().x - m_prev_ViewX, 0.f)));
    }

    if (m_step || Mario::getCurrentFalling()) {
        float temp;
        const auto [fst, snd] = QuickCheckSideCollision(
            MFCPP::CollisionObject(Mario::getCurrentPosition(), Mario::getOrigin(), extendHitboxX(Mario::getHitboxWall(), 2.f)), Mario::getDirection(), temp, temp);
        if (!snd)
            Mario::forceSetPosition(Mario::getCurrentPosition() + sf::Vector2f(sf::Vector2f(MFCPP::AutoScroll::getPosition().x - m_prev_ViewX, 0.f)));
    }
    //ScoreEffect
    GameScene::effectManager.teleportWithScroll(sf::Vector2f(MFCPP::AutoScroll::getPosition().x - m_prev_ViewX, 0.f));
    //setCurrentPosition(sf::Vector2f(MFCPP::AutoScroll::getPosition().x - Width / 2, getCurrentPosition().y));
    m_transform.teleport(m_transform.getCurrentPosition() + sf::Vector2f(MFCPP::AutoScroll::getPosition().x - m_prev_ViewX, 0.f));
    m_prev_ViewX = MFCPP::AutoScroll::getPosition().x;
}

void TankGround::Destroy() {
    if (!isDestroyed()) {
        m_transform.destroy();
        m_movingBlockManager.setDeletionFlag(true);
    }
}

void TankGround::activate() {}

void TankGround::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) return;
    m_animation.setColor(sf::Color(255, 255, 255));
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), m_transform.getCurrentPosition() - getOrigin(), sf::Color::Red));
}

void TankGround::animationUpdate(float deltaTime) {}

sf::Vector2f TankGround::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f TankGround::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect TankGround::getHitbox() {
    return m_hitbox;
}

bool TankGround::isDestroyed() {
    return m_transform.isDestroyed();
}

sf::Vector2f TankGround::getPreviousPosition() {
    return m_transform.getPreviousPosition();
}
