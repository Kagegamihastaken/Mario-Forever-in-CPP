#include "Object/Effect/RainEffect.hpp"

#include "Core/Scroll.hpp"
#include "Core/Utility.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/EffectManager.hpp"

RainEffect::RainEffect(EffectManager &manager, const sf::Vector2f &position)
    : Effect(manager),
    m_transform(position, sf::Vector2f(0.f, 15.f), sf::degrees(0.f)){
    m_velocity = sf::Vector2f(-8.f, 8.f);
    m_hitbox = sf::FloatRect({0.f, 0.f}, {15.f, 16.f});
    m_animation.setAnimationSequence("RAIN_EFFECT");
    state = false;
    m_platform_collided = false;
    if (Utility::RandomIntNumberGenerator(1, 2) == 1)
        m_animation.setAnimation(0, 0, 0, true);
    else
        m_animation.setAnimation(1, 1, 0, true);
}

void RainEffect::updateState() {
    if (state)
        m_animation.setAnimation(2, 4, 20, false);
}

void RainEffect::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void RainEffect::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;

    if (Scroll::isOutOfScreenYBottom(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0.f))
        Destroy();

    if (!state) {
        float CurrPosYCollide, CurrPosXCollide;
        m_transform.move(m_velocity * deltaTime);

        auto sideCollision = QuickCheckSideCollision(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), true, CurrPosXCollide, CurrPosYCollide);
        if (sideCollision.first || sideCollision.second) {
            state = true;
            updateState();
        }
        // Bottom check
        if (float PlatPosY; PlatformYCollision(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), PlatPosY, m_velocity.y, false)) {
            state = true;
            m_platform_collided = true;
            updateState();
        }
        const float offset = std::min(m_velocity.x + 1.f, 3.f);
        if (QuickCheckBotCollision(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), offset, CurrPosXCollide, CurrPosYCollide)) {
            if (m_velocity.y >= -m_velocity.x) {
                const float floorY = GetCurrFloorY(m_transform.getCurrentPosition(), CurrPosXCollide, CurrPosYCollide);
                if (m_transform.getCurrentPosition().y >= CurrPosYCollide + floorY - offset) {
                    state = true;
                    updateState();
                }
            }
        }
    }
    else
        if (m_animation.isAnimationAtTheEnd())
            Destroy();
}

void RainEffect::Destroy() {
    if (!isDestroyed()) {
        m_transform.destroy();
        m_effectManager.setDeletionFlag(true);
    }
}

void RainEffect::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) {
        m_animation.frameUpdate();
        return;
    }
    m_animation.setRotation(m_transform.getInterpolatedAngle(alpha));
    m_animation.setColor(sf::Color(255, 255, 255));
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
}

sf::Vector2f RainEffect::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f RainEffect::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect RainEffect::getHitbox() {
    return m_hitbox;
}

bool RainEffect::isDestroyed() {
    return m_transform.isDestroyed();
}

void RainEffect::animationUpdate(float deltaTime) {
    m_animation.frameTimeAccumulate(deltaTime);
}

void RainEffect::teleport(sf::Vector2f val) {
    if (!state || m_platform_collided)
        m_transform.moveTeleport(val);
}
