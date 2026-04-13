#include "Object/Platform/RedPlatform.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Logging.hpp"
#include "Core/Scroll.hpp"
#include "Core/Object/MovingBlockManager.hpp"
#include "Core/Object/MovingBlock/Behavior/PlatformBehavior.hpp"

RedPlatform::RedPlatform(MovingBlockManager &manager, const sf::Vector2f &start, const sf::Vector2f &end, float speed, bool smooth, bool fall, bool wait, bool small) : MovingBlock(manager) {
    setCurrentPosition(start);
    setInterpolatedPosition(start);
    setPreviousPosition(start);

    setOrigin(sf::Vector2f(0.f, 0.f));
    if (!small) {
        setHitbox(sf::FloatRect({0.f, 0.f}, {95.f, 8.f}));
        m_animation.setTexture("RedPlatform");
    }
    else {
        setHitbox(sf::FloatRect({0.f, 0.f}, {31.f, 8.f}));
        m_animation.setTexture("RedSmallPlatform");
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
}

void RedPlatform::setPreviousData() {
    if (isDestroyed()) return;

    setPreviousPosition(getCurrentPosition());
}

void RedPlatform::interpolateData(float alpha) {
    if (isDestroyed()) return;

    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}

void RedPlatform::statusUpdate(float deltaTime) {
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

void RedPlatform::Destroy() {
    if (!isDestroyed()) {
        setDestroyed(true);
        m_movingBlockManager.setDeletionFlag(true);
    }
}

void RedPlatform::activate() {
    if (!m_willFall) m_willFall = true;
    if (m_wait && m_waitState == 0) m_waitState = 1;
}

void RedPlatform::draw() {
    if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 32.f)) return;
    m_animation.setColor(sf::Color(255, 255, 255));
    m_animation.AnimationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.AnimationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), getCurrentPosition() - getOrigin(), sf::Color::Red));
}
