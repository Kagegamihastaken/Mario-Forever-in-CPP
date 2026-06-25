#include "Object/Effect/Score10000Effect.hpp"

#include "Core/Scroll.hpp"
#include "Core/Object/EffectManager.hpp"
#include "Object/Mario.hpp"

Score10000Effect::Score10000Effect(EffectManager &manager, const sf::Vector2f &position)
    : Effect(manager),
    m_transform(position, sf::Vector2f(21.f, 15.f), sf::degrees(0.f)){
    m_hitbox = sf::FloatRect({0.f, 0.f}, {42.f, 16.f});
    m_animation.setTexture("Score_10000");
    m_alpha = 255.f;
    m_velocity = {0.f, -1.5f};
    Mario::setScore(Mario::getScore() + 10000);
}

void Score10000Effect::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void Score10000Effect::statusUpdate(float deltaTime) {
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

void Score10000Effect::Destroy() {
    if (!isDestroyed()) {
        m_transform.destroy();
        m_effectManager.setDeletionFlag(true);
    }
}

sf::Vector2f Score10000Effect::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f Score10000Effect::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect Score10000Effect::getHitbox() {
    return m_hitbox;
}

bool Score10000Effect::isDestroyed() {
    return m_transform.isDestroyed();
}

void Score10000Effect::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) return;
    m_animation.setColor(sf::Color(255, 255, 255, m_alpha));
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
}

void Score10000Effect::animationUpdate(float deltaTime) {}
void Score10000Effect::teleport(sf::Vector2f val) {
    m_transform.moveTeleport(val);
}
