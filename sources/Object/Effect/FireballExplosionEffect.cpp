#include "Object/Effect/FireballExplosionEffect.hpp"
#include <SFML/System/Vector2.hpp>

#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/Class/CollisionObjectClass.hpp"
#include "Core/Object/EffectManager.hpp"
#include "Effect/FireballExplosion.hpp"

FireballExplosionEffect::FireballExplosionEffect(EffectManager &manager, const sf::Vector2f &position) : Effect(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_animation.setAnimationSequence(FireballExplosionAnimName);
    m_animation.setAnimation(0, 2, 24, false);
    setOrigin(sf::Vector2f(16.f, 15.f));
    setHitbox(sf::FloatRect({0.f, 0.f}, {33.f, 30.f}));
}

void FireballExplosionEffect::setPreviousData() {
    if (isDestroyed()) return;

    setPreviousPosition(getCurrentPosition());
}

void FireballExplosionEffect::interpolateData(const float alpha) {
    if (isDestroyed()) return;

    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}

void FireballExplosionEffect::statusUpdate(const float deltaTime) {
    if (isDestroyed()) return;

    if (m_animation.isAnimationAtTheEnd())
        Destroy();
}

void FireballExplosionEffect::Destroy() {
    if (!isDestroyed()) {
        setDestroyed(true);
        m_effectManager.setDeletionFlag(true);
    }
}

void FireballExplosionEffect::draw() {
    if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 32.f)) return;
    m_animation.setColor(sf::Color(255, 255, 255));
    m_animation.animationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.animationDraw();
}
