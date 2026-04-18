#include "Object/Effect/NormalCoinEffect.hpp"

#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/Class/CollisionObjectClass.hpp"
#include "Core/Object/EffectManager.hpp"
#include "Effect/CoinEffect.hpp"
#include "Effect/ScoreEffect.hpp"

NormalCoinEffect::NormalCoinEffect(EffectManager &manager, const sf::Vector2f &position) : Effect(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    setOrigin(sf::Vector2f(18.f, 31.f));
    setHitbox(sf::FloatRect({0.f, 0.f}, {37.f, 32.f}));
    m_velocity = sf::Vector2f(0.f, -3.5f);
    m_animation.setAnimationSequence(CoinEffectAnimName);
    m_animation.setAnimation(0, 20, 70, false);
}

void NormalCoinEffect::setPreviousData() {
    if (isDestroyed()) return;

    setPreviousPosition(getCurrentPosition());
}

void NormalCoinEffect::interpolateData(float alpha) {
    if (isDestroyed()) return;

    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}

void NormalCoinEffect::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    if (m_animation.isAnimationAtTheEnd()) {
        Destroy();
        return;
    }
    move(sf::Vector2f(0.f, m_velocity.y * deltaTime));
    if (m_velocity.y < 0.0f) m_velocity.y += deltaTime * 0.125f;
    else m_velocity.y += 0.f;
}

void NormalCoinEffect::Destroy() {
    if (!isDestroyed()) {
        AddScoreEffect(SCORE_200, getCurrentPosition().x, getCurrentPosition().y);
        setDestroyed(true);
        m_effectManager.setDeletionFlag(true);
    }
}

void NormalCoinEffect::draw() {
    if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 32.f)) return;
    m_animation.setColor(sf::Color(255, 255, 255));
    m_animation.animationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.animationDraw();
}
