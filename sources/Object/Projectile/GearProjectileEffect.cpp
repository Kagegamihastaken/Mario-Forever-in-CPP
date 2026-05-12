#include "Object/Projectile/GearProjectileEffect.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/ProjectileManager.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Effect/FireballExplosion.hpp"
#include "Effect/MarioEffect.hpp"
#include "Object/Mario.hpp"

GearProjectileEffect::GearProjectileEffect(ProjectileManager &manager, const sf::Vector2f &position, const sf::Angle& angle, bool dir) : Projectile(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    setCurrentAngle(angle);
    setPreviousAngle(angle);
    setCurrentAngle(angle);
    m_alpha = 255.f;
    m_animation.setTexture("GearProjectile", true);
    setOrigin(sf::Vector2f(20.f, 20.f));
    setHitbox(sf::FloatRect({0.f, 0.f}, {40.f, 40.f}));
    setDrawingPriority(1);
}

void GearProjectileEffect::setPreviousData() {
    if (isDestroyed()) return;
    setPreviousPosition(getCurrentPosition());
    setPreviousAngle(getCurrentAngle());
}
void GearProjectileEffect::interpolateData(const float alpha) {
    if (isDestroyed()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
    setInterpolatedAngle(linearInterpolation(getPreviousAngle(), getCurrentAngle(), alpha));
}

void GearProjectileEffect::FireballEffect() const {}

void GearProjectileEffect::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    //Status
    m_alpha -= 15.f * deltaTime;
    if (m_alpha <= 0.f) Destroy();
}

void GearProjectileEffect::CollisionUpdate() {}

void GearProjectileEffect::draw() {
    if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 32.f)) return;
    m_animation.setColor(sf::Color(255.f, 255.f, 255.f, m_alpha));
    m_animation.setAnimationDirection(m_dir);
    m_animation.animationUpdate(getInterpolatedPosition() - sf::Vector2f(0.f, 0.f), getOrigin() - sf::Vector2f(0.f, 0.f));
    m_animation.setRotation(getInterpolatedAngle());
    m_animation.animationDraw();
}

void GearProjectileEffect::Destroy() {
    setDestroyed(true);
    m_manager.setDeletionFlag(true);
}

void GearProjectileEffect::LevelEndCleanup() {
    Destroy();
}
