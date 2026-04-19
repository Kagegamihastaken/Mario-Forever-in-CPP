#include "Object/Effect/Score1UPEffect.hpp"

#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Object/EffectManager.hpp"
#include "Object/Mario.hpp"

Score1UPEffect::Score1UPEffect(EffectManager &manager, const sf::Vector2f &position) : Effect(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    setOrigin(sf::Vector2f(16.f, 15.f));
    setHitbox(sf::FloatRect({0.f, 0.f}, {32.f, 16.f}));
    m_animation.setTexture("Score_1UP");
    m_alpha = 255.f;
    m_velocity = {0.f, -1.5f};
    Mario::setLives(Mario::getLives() + 1);
    SoundManager::PlaySound("1UP");
}

void Score1UPEffect::setPreviousData() {
    if (isDestroyed()) return;

    setPreviousPosition(getCurrentPosition());
}

void Score1UPEffect::interpolateData(float alpha) {
    if (isDestroyed()) return;

    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}

void Score1UPEffect::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;

    move(sf::Vector2f(0.f, m_velocity.y * deltaTime));
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
        setDestroyed(true);
        m_effectManager.setDeletionFlag(true);
    }
}

void Score1UPEffect::draw() {
    if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 32.f)) return;
    m_animation.setColor(sf::Color(255, 255, 255, m_alpha));
    m_animation.animationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.animationDraw();
}
