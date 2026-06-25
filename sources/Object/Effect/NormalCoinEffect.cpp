#include "Object/Effect/NormalCoinEffect.hpp"

#include "Core/Scroll.hpp"
#include "Core/Class/CollisionObjectClass.hpp"
#include "Core/Object/EffectManager.hpp"
#include "Effect/CoinEffect.hpp"
#include "Effect/ScoreEffect.hpp"

NormalCoinEffect::NormalCoinEffect(EffectManager &manager, const sf::Vector2f &position)
    : Effect(manager),
    m_transform(position, sf::Vector2f(18.f, 31.f), sf::degrees(0.f)){
    m_hitbox = sf::FloatRect({0.f, 0.f}, {37.f, 32.f});
    m_velocity = sf::Vector2f(0.f, -3.5f);
    m_animation.setAnimationSequence("CoinEffectAnimName");
    m_animation.setAnimation(0, 20, 70, false);
}

void NormalCoinEffect::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void NormalCoinEffect::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    if (m_animation.isAnimationAtTheEnd()) {
        Destroy();
        return;
    }
    m_transform.move(sf::Vector2f(0.f, m_velocity.y * deltaTime));
    if (m_velocity.y < 0.0f) m_velocity.y += deltaTime * 0.125f;
    else m_velocity.y += 0.f;
}

void NormalCoinEffect::Destroy() {
    if (!isDestroyed()) {
        AddScoreEffect(ScoreID::SCORE_200, m_transform.getCurrentPosition().x, m_transform.getCurrentPosition().y);
        m_transform.destroy();
        m_effectManager.setDeletionFlag(true);
    }
}

void NormalCoinEffect::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) {
        m_animation.frameUpdate();
        return;
    }
    m_animation.setColor(sf::Color(255, 255, 255));
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
}

void NormalCoinEffect::animationUpdate(float deltaTime) {
    m_animation.frameTimeAccumulate(deltaTime);
}

sf::Vector2f NormalCoinEffect::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f NormalCoinEffect::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect NormalCoinEffect::getHitbox() {
    return m_hitbox;
}

bool NormalCoinEffect::isDestroyed() {
    return m_transform.isDestroyed();
}

void NormalCoinEffect::teleport(sf::Vector2f val) {}
