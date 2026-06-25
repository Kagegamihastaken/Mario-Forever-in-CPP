#include "Object/Projectile/GearProjectileEffect.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Scroll.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/ProjectileManager.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Mario.hpp"

GearProjectileEffect::GearProjectileEffect(ProjectileManager &manager, const sf::Vector2f &position, const sf::Angle& angle, bool dir)
    : Projectile(manager),
    m_transform(position, sf::Vector2f(20.f, 20.f), angle){
    m_alpha = 255.f;
    m_animation.setTexture("GearProjectile", true);
    m_hitbox = sf::FloatRect({0.f, 0.f}, {40.f, 40.f});
    setDrawingPriority(1);
}

void GearProjectileEffect::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void GearProjectileEffect::FireballEffect() const {}

void GearProjectileEffect::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    //Status
    m_alpha -= 15.f * deltaTime;
    if (m_alpha <= 0.f) Destroy();
}

void GearProjectileEffect::CollisionUpdate() {}

void GearProjectileEffect::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 8.f)) return;
    m_animation.setColor(sf::Color(255.f, 255.f, 255.f, m_alpha));
    m_animation.setAnimationDirection(m_dir);
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha) - sf::Vector2f(0.f, 0.f), getOrigin() - sf::Vector2f(0.f, 0.f));
    m_animation.setRotation(m_transform.getInterpolatedAngle(alpha));
    m_animation.animationDraw();
}

void GearProjectileEffect::Destroy() {
    m_transform.destroy();
    m_manager.setDeletionFlag(true);
}

void GearProjectileEffect::LevelEndCleanup() {
    Destroy();
}

void GearProjectileEffect::animationUpdate(float deltaTime) {}

sf::Vector2f GearProjectileEffect::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f GearProjectileEffect::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect GearProjectileEffect::getHitbox() {
    return m_hitbox;
}

bool GearProjectileEffect::isDestroyed() {
    return m_transform.isDestroyed();
}
