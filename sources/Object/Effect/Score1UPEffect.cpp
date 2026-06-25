#include "Object/Effect/Score1UPEffect.hpp"

#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Object/EffectManager.hpp"
#include "Object/Mario.hpp"

Score1UPEffect::Score1UPEffect(EffectManager &manager, const sf::Vector2f &position)
    : Effect(manager),
    m_transform(position, sf::Vector2f(16.f, 15.f), sf::degrees(0.f)){
    m_hitbox = sf::FloatRect({0.f, 0.f}, {32.f, 16.f});
    m_animation.setTexture("Score_1UP");
    m_alpha = 255.f;
    m_velocity = {0.f, -1.5f};
    Mario::setLives(Mario::getLives() + 1);
    SoundManager::PlaySound("1UP");
}

void Score1UPEffect::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void Score1UPEffect::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;

    m_transform.move(sf::Vector2f(0.f, m_velocity.y * deltaTime));
    if (m_velocity.y < 0.f) m_velocity.y += 0.025f * deltaTime;
    else {
        if (m_velocity.y < 0.f) m_velocity.y = 0.f;
        if (m_alpha > 0.f) m_alpha -= 7.5f * deltaTime;
        else {
            m_alpha = 0.f;
            Destroy();
        }
    }
}

void Score1UPEffect::Destroy() {
    if (!isDestroyed()) {
        m_transform.destroy();
        m_effectManager.setDeletionFlag(true);
    }
}

void Score1UPEffect::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) return;
    m_animation.setColor(sf::Color(255, 255, 255, m_alpha));
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
}

sf::Vector2f Score1UPEffect::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f Score1UPEffect::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect Score1UPEffect::getHitbox() {
    return m_hitbox;
}

bool Score1UPEffect::isDestroyed() {
    return m_transform.isDestroyed();
}

void Score1UPEffect::animationUpdate(float deltaTime) {}

void Score1UPEffect::teleport(sf::Vector2f val) {
    m_transform.moveTeleport(val);
}
