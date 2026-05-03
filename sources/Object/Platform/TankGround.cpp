#include "Object/Platform/TankGround.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Logging.hpp"
#include "Core/Scroll.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/MovingBlockManager.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Mario.hpp"

TankGround::TankGround(MovingBlockManager &manager, const sf::Vector2f &pos) : MovingBlock(manager) {
    setCurrentPosition(pos);
    setInterpolatedPosition(pos);
    setPreviousPosition(pos);

    setOrigin(sf::Vector2f(0.f, 0.f));
    setHitbox(sf::FloatRect({0.f, 0.f}, {1920.f, 64.f}));
    m_animation.setTexture("TankGround");
    m_prev_ViewX = ViewX;
    m_step = false;
    setCanCollision(true);
}

void TankGround::setPreviousData() {
    if (isDestroyed()) return;

    setPreviousPosition(getCurrentPosition());
}

void TankGround::interpolateData(float alpha) {
    if (isDestroyed()) return;

    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}

void TankGround::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    //Mario
    const sf::FloatRect CollideHitbox = getGlobalHitbox(Mario::getHitboxFloor(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (const sf::FloatRect PlatformHitbox = getGlobalHitbox(getHitbox(), getPreviousPosition(), getOrigin()); isCollide(CollideHitbox, PlatformHitbox)) {
        if (!Mario::getCurrentFalling()) m_step = true;
    }
    else {
        if (!Mario::getCurrentFalling()) m_step = false;
    }

    if (m_step || Mario::getCurrentFalling()) {
        float temp;
        const auto [fst, snd] = QuickCheckSideCollision(
            MFCPP::CollisionObject(Mario::getCurrentPosition(), Mario::getOrigin(), extendHitboxX(Mario::getHitboxWall(), 2.f)), Mario::getDirection(), temp, temp);
        if (!snd)
            Mario::setCurrentPosition(Mario::getCurrentPosition() + sf::Vector2f(sf::Vector2f(ViewX - m_prev_ViewX, 0.f)));
    }
    //ScoreEffect
    for (auto &score: GameScene::effectManager.getScrollBasedList()) {
        score.setCurrentPosition(score.getCurrentPosition() + sf::Vector2f(ViewX - m_prev_ViewX, 0.f));
    }

    setCurrentPosition(getCurrentPosition() + sf::Vector2f(ViewX - m_prev_ViewX, 0.f));
    setPreviousPosition(getCurrentPosition());
    m_prev_ViewX = ViewX;
}

void TankGround::Destroy() {
    if (!isDestroyed()) {
        setDestroyed(true);
        m_movingBlockManager.setDeletionFlag(true);
    }
}

void TankGround::activate() {}

void TankGround::draw() {
    if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 32.f)) return;
    m_animation.setColor(sf::Color(255, 255, 255));
    m_animation.animationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), getCurrentPosition() - getOrigin(), sf::Color::Red));
}
