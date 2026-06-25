#include "Object/Effect/FireballExplosionEffect.hpp"
#include <SFML/System/Vector2.hpp>

#include "Core/Scroll.hpp"
#include "Core/Class/CollisionObjectClass.hpp"
#include "Core/Object/EffectManager.hpp"

FireballExplosionEffect::FireballExplosionEffect(EffectManager &manager, const sf::Vector2f &position)
    : Effect(manager),
    m_transform(position, sf::Vector2f(16.f, 15.f), sf::degrees(0.f)){
    m_animation.setAnimationSequence("FireballExplosionAnimName");
    m_animation.setAnimation(0, 2, 24, false);
    m_hitbox = sf::FloatRect({0.f, 0.f}, {33.f, 30.f});
}

void FireballExplosionEffect::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void FireballExplosionEffect::statusUpdate(const float deltaTime) {
    if (isDestroyed()) return;

    if (m_animation.isAnimationAtTheEnd())
        Destroy();
}

void FireballExplosionEffect::Destroy() {
    if (!isDestroyed()) {
        m_transform.destroy();
        m_effectManager.setDeletionFlag(true);
    }
}

void FireballExplosionEffect::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) {
        m_animation.frameUpdate();
        return;
    }
    m_animation.setColor(sf::Color(255, 255, 255));
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
}

void FireballExplosionEffect::animationUpdate(float deltaTime) {
    m_animation.frameTimeAccumulate(deltaTime);
}

sf::Vector2f FireballExplosionEffect::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f FireballExplosionEffect::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect FireballExplosionEffect::getHitbox() {
    return m_hitbox;
}

bool FireballExplosionEffect::isDestroyed() {
    return m_transform.isDestroyed();
}

void FireballExplosionEffect::teleport(sf::Vector2f val) {}
