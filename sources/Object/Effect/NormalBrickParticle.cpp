#include "Object/Effect/NormalBrickParticle.hpp"

#include "Core/Scroll.hpp"
#include "Core/Object/EffectManager.hpp"

NormalBrickParticle::NormalBrickParticle(EffectManager &manager, const sf::Vector2f &velocity, const sf::Vector2f &position)
    : Effect(manager),
    m_transform(position, sf::Vector2f(8.f, 8.f), sf::degrees(0.f)){
    m_velocity = velocity;
    m_hitbox = sf::FloatRect({0.f, 0.f}, {16.f, 16.f});
    m_animation.setTexture("NormalBrickParticle");
    m_direction = true;
}

void NormalBrickParticle::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void NormalBrickParticle::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;

    if (Scroll::isOutOfScreenYBottom(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 32.f))
        Destroy();
    m_transform.move(m_velocity * deltaTime);
    if (m_velocity.x > 0.f) {
        m_transform.rotate(sf::degrees(10.f * deltaTime));
        m_direction = true;
    }
    else if (m_velocity.x < 0.f) {
        m_transform.rotate(-sf::degrees(10.f * deltaTime));
        m_direction = false;
    }
    m_velocity.y += deltaTime * 0.3f;
}

void NormalBrickParticle::Destroy() {
    if (!isDestroyed()) {
        m_transform.destroy();
        m_effectManager.setDeletionFlag(true);
    }
}

void NormalBrickParticle::draw(float alpha) {
    m_animation.setAnimationDirection(!m_direction);
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 16.f)) return;
    m_animation.setRotation(m_transform.getInterpolatedAngle(alpha));
    m_animation.setColor(sf::Color(255, 255, 255));
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
}

sf::Vector2f NormalBrickParticle::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f NormalBrickParticle::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect NormalBrickParticle::getHitbox() {
    return m_hitbox;
}

bool NormalBrickParticle::isDestroyed() {
    return m_transform.isDestroyed();
}

void NormalBrickParticle::animationUpdate(float deltaTime) {}

void NormalBrickParticle::teleport(sf::Vector2f val) {}
