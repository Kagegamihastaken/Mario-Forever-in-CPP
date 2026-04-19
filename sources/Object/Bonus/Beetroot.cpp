#include "Object/Bonus/Beetroot.hpp"
#include "Core/Object/EnemyManager.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/Enemy/Behavior/GoombaAIBehavior.hpp"
#include "Core/HitboxUtils.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Object/GoombaAI.hpp"
#include "Object/Mario.hpp"

Beetroot::Beetroot(EnemyManager &manager, const sf::Vector2f& position) : Enemy(manager) {
    setCurrentPosition(position + sf::Vector2f(0.f, 31.f));
    setPreviousPosition(getCurrentPosition());
    setInterpolatedPosition(getCurrentPosition());
    m_animation.setAnimation(0, 3, 8);
    m_animation.setAnimationSequence(BeetrootAnimName);
    setHitbox(sf::FloatRect({0.f, 0.f}, {31.f, 32.f}));
    m_wall_hitbox = sf::FloatRect(getHitbox().position, getHitbox().size - sf::Vector2f(0.f, 6.f));
    setOrigin(sf::Vector2f(16.f, 31.f));
    m_velocity = sf::Vector2f(0.f, 0.f);
    setDirection(true);
    setDisabled(true);
    m_appearingSpeed = 0.5f;
    m_appearingY = 0.f;
    m_appearingYMax = 32.f;
    m_isappearing = true;
    setCollideEachOther(false);
    setShellKicking(false);
    setShellBlocker(false);

    setDrawingPriority(0);
}
void Beetroot::setPreviousData() {
    if (isDestroyed() || isDisabled()) return;
    setPreviousPosition(getCurrentPosition());
}
void Beetroot::interpolateData(const float alpha) {
    if (isDestroyed() || isDisabled()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}
void Beetroot::EnemyCollision() {}
void Beetroot::MarioCollision(const float MarioYVelocity) {
    if (isDestroyed() || isDisabled() || m_isappearing) return;
    if (f_abs(Mario::getCurrentPosition().x - getCurrentPosition().x) >= 80.0f) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(Mario::getHitbox(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (const sf::FloatRect GoombaAIHitbox = getGlobalHitbox(getHitbox(), getCurrentPosition(), getOrigin()); isCollide(GoombaAIHitbox, hitbox_mario)) {
        SoundManager::PlaySound("Powerup");
        AddScoreEffect(SCORE_1000, getCurrentPosition().x, getCurrentPosition().y - getOrigin().y);
        if (Mario::getPowerState() > 0) Mario::SetPowerState(3);
        else if (Mario::getPowerState() == 0) Mario::SetPowerState(1);
        Destroy();
    }
}
void Beetroot::statusUpdate(const float deltaTime) {
    if (isDestroyed()) return;

    if (isOutScreenYBottom(getCurrentPosition().y, 80))
        Destroy();
    if (!isOutScreen(getCurrentPosition().x - getOrigin().x, getCurrentPosition().y, 32, 80)) {
        if (isDisabled()) setDisabled(false);
    }
}
void Beetroot::XUpdate(const float deltaTime) {
    if (isDestroyed() || isDisabled() || m_isappearing) return;
    auto data = GoombaAIBehavior::GoombaAIXMove(GoombaAIBehavior::GoombaAIData(getCurrentPosition(), m_velocity, getDirection()), deltaTime);
    data = GoombaAIBehavior::GoombaAIXCollision(data, getHitbox(), m_wall_hitbox, getOrigin());
    setCurrentPosition(data.position);
    m_velocity = data.velocity;
    setDirection(data.direction);
}
void Beetroot::YUpdate(const float deltaTime) {
    if (isDestroyed() || isDisabled()) return;
    if (!m_isappearing) {
        auto data = GoombaAIBehavior::GoombaAIYMove(GoombaAIBehavior::GoombaAIData(getCurrentPosition(), m_velocity, getDirection()), deltaTime);
        data = GoombaAIBehavior::GoombaAIYCollision(data, getHitbox(), getOrigin());
        setCurrentPosition(data.position);
        m_velocity = data.velocity;
        setDirection(data.direction);
    }
    else {
        const auto data = GoombaAIBehavior::MushroomAppearing(GoombaAIBehavior::GoombaAIData(getCurrentPosition(), m_velocity, getDirection()), m_appearingY, m_appearingSpeed, m_appearingYMax, m_isappearing, isDisabled(), deltaTime);
        setCurrentPosition(data.position);
        m_velocity = data.velocity;
        setDirection(data.direction);
    }
}
void Beetroot::BlockHit() {}
void Beetroot::ShellHit() {}
void Beetroot::Destroy() {
    if (!isDestroyed()) {
        setDestroyed(true);
        m_enemyManager.setDeletionFlag(true);
    }
}
void Beetroot::draw() {
    if (isOutScreen(getInterpolatedPosition().x - getOrigin().x, getInterpolatedPosition().y, 32, 80)) return;
    m_animation.animationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), getCurrentPosition() - getOrigin(), sf::Color::Magenta));
}
void Beetroot::Death(unsigned int state) {}
bool Beetroot::isDeath() {
    return true; // Means no collision
}
