#include "Object/Effect/NormalBrickParticle.hpp"

#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/Object/EffectManager.hpp"

NormalBrickParticle::NormalBrickParticle(EffectManager &manager, const sf::Vector2f &velocity, const sf::Vector2f &position) : Effect(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_velocity = velocity;
    setOrigin(sf::Vector2f(8.f, 8.f));
    setHitbox(sf::FloatRect({0.f, 0.f}, {16.f, 16.f}));
    m_animation.setTexture("NormalBrickParticle");
}

void NormalBrickParticle::setPreviousData() {
    if (isDestroyed()) return;

    setPreviousPosition(getCurrentPosition());
    setPreviousAngle(getCurrentAngle());
}

void NormalBrickParticle::interpolateData(float alpha) {
    if (isDestroyed()) return;

    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
    setInterpolatedAngle(linearInterpolation(getPreviousAngle(), getCurrentAngle(), alpha));
}

void NormalBrickParticle::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;

    if (isOutOfScreenYBottom(MFCPP::CollisionObject(getCurrentPosition(), getOrigin(), getHitbox()), 32.f))
        Destroy();
    move(m_velocity * deltaTime);
    if (m_velocity.x > 0.f) {
        setCurrentAngle(getCurrentAngle() + sf::degrees(10.f * deltaTime));
        m_direction = true;
    }
    else if (m_velocity.x < 0.f) {
        setCurrentAngle(getCurrentAngle() - sf::degrees(10.f * deltaTime));
        m_direction = false;
    }
    m_velocity.y += deltaTime * 0.3f;
}

void NormalBrickParticle::Destroy() {
    if (!isDestroyed()) {
        setDestroyed(true);
        m_effectManager.setDeletionFlag(true);
    }
}

void NormalBrickParticle::draw() {
    m_animation.setAnimationDirection(!m_direction);
    if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 32.f)) return;
    m_animation.setRotation(getInterpolatedAngle());
    m_animation.setColor(sf::Color(255, 255, 255));
    m_animation.animationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.animationDraw();
}
