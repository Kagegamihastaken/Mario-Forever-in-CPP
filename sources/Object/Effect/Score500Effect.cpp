#include "Object/Effect/Score500Effect.hpp"

#include "Core/Scroll.hpp"
#include "Core/Object/EffectManager.hpp"
#include "Object/Mario.hpp"

Score500Effect::Score500Effect(EffectManager &manager, const sf::Vector2f &position)
    : Effect(manager),
    m_transform(position, sf::Vector2f(14.f, 15.f), sf::degrees(0.f)){
    m_hitbox = sf::FloatRect({0.f, 0.f}, {28.f, 16.f});
    m_animation.setTexture("Score_500");
    m_alpha = 255.f;
    m_velocity = {0.f, -1.5f};
    Mario::setScore(Mario::getScore() + 500);
}

void Score500Effect::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void Score500Effect::statusUpdate(float deltaTime) {
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

void Score500Effect::Destroy() {
    if (!isDestroyed()) {
        m_transform.destroy();
        m_effectManager.setDeletionFlag(true);
    }
}

sf::Vector2f Score500Effect::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f Score500Effect::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect Score500Effect::getHitbox() {
    return m_hitbox;
}

bool Score500Effect::isDestroyed() {
    return m_transform.isDestroyed();
}

void Score500Effect::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) return;
    m_animation.setColor(sf::Color(255, 255, 255, m_alpha));
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
}

void Score500Effect::animationUpdate(float deltaTime) {}

void Score500Effect::teleport(sf::Vector2f val) {
    m_transform.moveTeleport(val);
}
