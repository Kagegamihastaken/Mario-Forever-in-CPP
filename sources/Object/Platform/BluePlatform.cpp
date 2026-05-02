#include "Object/Platform/BluePlatform.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/Object/MovingBlockManager.hpp"
#include "Core/Object/MovingBlock/Behavior/PlatformBehavior.hpp"

BluePlatform::BluePlatform(MovingBlockManager &manager, const sf::Vector2f &start, const sf::Vector2f &end, float speed, bool smooth, bool fall, bool wait, bool small) : MovingBlock(manager) {
    setCurrentPosition(start);
    setInterpolatedPosition(start);
    setPreviousPosition(start);

    setOrigin(sf::Vector2f(0.f, 0.f));
    if (!small) {
        setHitbox(sf::FloatRect({0.f, 0.f}, {95.f, 8.f}));
        m_animation.setTexture("BluePlatform");
    }
    else {
        setHitbox(sf::FloatRect({0.f, 0.f}, {31.f, 8.f}));
        m_animation.setTexture("BlueSmallPlatform");
    }

    m_start = start;
    m_end = end;
    m_maxSpeed = speed;
    m_smooth = smooth;
    m_fall = fall;
    m_wait = wait;
    m_small = small;

    m_speed = 0.f;
    m_movingToEnd = true;
    m_smoothStore = 0.f;
    m_y_velocity = 0.f;
    m_waitState = 0;
    m_willFall = false;
    setCanCollision(false);
}

void BluePlatform::setPreviousData() {
    if (isDestroyed()) return;

    setPreviousPosition(getCurrentPosition());
}

void BluePlatform::interpolateData(float alpha) {
    if (isDestroyed()) return;

    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}

void BluePlatform::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;

    if (isOutScreenYBottom(getCurrentPosition().y, 32.f) && m_willFall) {
        Destroy();
        return;
    }

    const auto data = PlatformBehavior::PlatformStatusUpdate(PlatformBehavior::PlatformData(getCurrentPosition(), m_smooth, m_wait, m_fall, m_start, m_end, m_speed, m_y_velocity, m_smoothStore, m_maxSpeed, m_movingToEnd, m_willFall, m_waitState), deltaTime);
    setCurrentPosition(data.position);
    m_speed = data.speed;
    m_smoothStore = data.smoothStore;
    m_movingToEnd = data.movingToEnd;
    m_willFall = data.willFall;
    m_y_velocity = data.y_velocity;
    m_start = data.start;
    m_end = data.end;
}

void BluePlatform::Destroy() {
    if (!isDestroyed()) {
        setDestroyed(true);
        m_movingBlockManager.setDeletionFlag(true);
    }
}

void BluePlatform::activate() {
    if (!m_willFall) m_willFall = true;
    if (m_wait && m_waitState == 0) m_waitState = 1;
}

void BluePlatform::draw() {
    if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 32.f)) return;
    m_animation.setColor(sf::Color(255, 255, 255));
    m_animation.animationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), getCurrentPosition() - getOrigin(), sf::Color::Red));
}
