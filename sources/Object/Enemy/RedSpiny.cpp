#include "Object/Enemy/RedSpiny.hpp"
#include "Core/Object/EnemyManager.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/Enemy/GoombaAIBehavior.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Object/GoombaAI.hpp"
#include "Effect/GoombaAIEffect.hpp"
#include "Object/Mario.hpp"

RedSpiny::RedSpiny(EnemyManager &manager, const sf::Vector2f& position) : Enemy(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_animation.setAnimation(0, 1, 14, true);
    m_animation.setAnimationSequence(RedSpinyAnimName);
    setHitbox(sf::FloatRect({0.f, 0.f}, {32.f, 32.f}));
    m_wall_hitbox = sf::FloatRect(getHitbox().position, getHitbox().size - sf::Vector2f(0.f, 6.f));
    setOrigin(sf::Vector2f(16.f, 31.f));
    m_velocity = sf::Vector2f(1.f, 0.f);
    setDirection(false);
    setDisabled(true);
    setCollideEachOther(true);
    m_state = 0;
    setShellKicking(true);
    setShellBlocker(false);
    setDrawingLowerPriority(true);
}
void RedSpiny::setPreviousData() {
    if (isDestroyed() || isDisabled()) return;
    setPreviousPosition(getCurrentPosition());
}
void RedSpiny::interpolateData(const float alpha) {
    if (isDestroyed() || isDisabled()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}
void RedSpiny::EnemyCollision() {
    if (isDestroyed() || isDisabled() || m_state > 0) return;
    auto& list = m_enemyManager.getGoombaAIList();
    auto it = list.iterator_to(*this);
    ++it;
    for (; it != list.end(); ++it) {
        Enemy* other = &*it;
        if (other->isDestroyed() || other->isDisabled()) continue;
        GoombaAIBehavior::GoombaAICollision(this, other);
    }
}
void RedSpiny::MarioCollision(const float MarioYVelocity) {
    if (isDestroyed() || isDisabled() || m_state > 0) return;
    if (f_abs(player.curr.x - getCurrentPosition().x) >= 80.0f) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(player.hitboxMain, player.curr, player.property.getOrigin());
    if (const sf::FloatRect GoombaAIHitbox = getGlobalHitbox(getHitbox(), getCurrentPosition(), getOrigin()); isCollide(GoombaAIHitbox, hitbox_mario))
        PowerDown();
}
void RedSpiny::statusUpdate(const float deltaTime) {
    if (isDestroyed()) return;

    if (m_state == 0) {
        if (isOutScreenYBottom(getCurrentPosition().y, 80))
            Destroy();
        if (!isOutScreen(getCurrentPosition().x - getOrigin().x, getCurrentPosition().y, 32, 80)) {
            if (isDisabled()) setDisabled(false);
        }
    }
    if (m_state == 1)
        if (isOutScreenYBottom(getCurrentPosition().y, 64)) Destroy();
}
void RedSpiny::XUpdate(const float deltaTime) {
    if (isDestroyed() || isDisabled() || m_state > 0) return;
    auto data = GoombaAIBehavior::GoombaAIXMove(GoombaAIBehavior::GoombaAIData(getCurrentPosition(), m_velocity, getDirection()), deltaTime);
    data = GoombaAIBehavior::GoombaAIXCollision(data, getHitbox(), m_wall_hitbox, getOrigin());
    setCurrentPosition(data.position);
    m_velocity = data.velocity;
    setDirection(data.direction);
}
void RedSpiny::YUpdate(const float deltaTime) {
    if (isDestroyed() || isDisabled()) return;
    if (m_state < 1) {
        auto data = GoombaAIBehavior::GoombaAIYMove(GoombaAIBehavior::GoombaAIData(getCurrentPosition(), m_velocity, getDirection()), deltaTime);
        data = GoombaAIBehavior::GoombaAIYCollision(data, getHitbox(), getOrigin());
        setCurrentPosition(data.position);
        m_velocity = data.velocity;
        setDirection(data.direction);
    }
    else {
        const auto data = GoombaAIBehavior::GoombaAIEffectYMove(GoombaAIBehavior::GoombaAIData(getCurrentPosition(), m_velocity, getDirection()), deltaTime);
        m_velocity = data.velocity;
        setCurrentPosition(data.position);
    }
}

void RedSpiny::BlockHit() {
    if (m_state > 0) return;
    AddScoreEffect(SCORE_100, getCurrentPosition().x, getCurrentPosition().y - getOrigin().y);
    SoundManager::PlaySound("Kick2");
    Death(1);
}

void RedSpiny::ShellHit() {
    if (m_state > 0) return;
    Death(1);
}

void RedSpiny::Destroy() {
    if (!isDestroyed()) {
        setDestroyed(true);
        m_enemyManager.setDeletionFlag(true);
    }
}

void RedSpiny::draw() {
    m_animation.setAnimationDirection(static_cast<AnimationDirection>(!getDirection()));
    if (isOutScreen(getInterpolatedPosition().x - getOrigin().x, getInterpolatedPosition().y, 32, 80)) return;
    m_animation.setColor(sf::Color(255, 255, 255));
    m_animation.AnimationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.AnimationDraw();
}
void RedSpiny::Death(unsigned int state) {
    setCollideEachOther(false);
    m_state = state;
    switch (m_state) {
        case 1:
            m_velocity = sf::Vector2f(0.f, -3.f);
            m_animation.setAnimationSequence(RedSpinyDeathEffect);
            m_animation.setAnimation(0,0,100, true);
            setShellKicking(false);
            setShellBlocker(false);
            setDrawingLowerPriority(false);
            break;
        default:;
    }
}

bool RedSpiny::isDeath() {
    return m_state > 0;
}
