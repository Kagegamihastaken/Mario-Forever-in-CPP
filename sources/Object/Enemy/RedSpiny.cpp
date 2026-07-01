#include "Object/Enemy/RedSpiny.hpp"
#include "Core/Object/EnemyManager.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/Enemy/Behavior/GoombaAIBehavior.hpp"
#include "Core/HitboxUtils.hpp"
#include "Core/Utility.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Object/Mario.hpp"

RedSpiny::RedSpiny(EnemyManager &manager, const sf::Vector2f& position)
    : Enemy(manager),
    m_transform(position, sf::Vector2f(16.f, 31.f), sf::degrees(0.f)){
    m_animation.setAnimation(0, 1, 14, true);
    m_animation.setAnimationSequence("RedSpinyAnimName");
    m_hitbox = sf::FloatRect({0.f, 0.f}, {32.f, 32.f});
    m_wall_hitbox = sf::FloatRect(getHitbox().position, getHitbox().size - sf::Vector2f(0.f, 6.f));
    m_velocity = sf::Vector2f(1.f, 0.f);
    setDirection(false);
    setDisabled(true);
    setCollideEachOther(true);
    m_state = 0;
    setShellKicking(true);
    setShellBlocker(false);
    setDrawingPriority(0);
}
void RedSpiny::updatePreviousData() {
    if (isDestroyed() || isDisabled()) return;
    m_transform.Update();
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
    if (Utility::f_abs(Mario::getCurrentPosition().x - m_transform.getCurrentPosition().x) >= 80.0f) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(Mario::getHitbox(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (const sf::FloatRect GoombaAIHitbox = getGlobalHitbox(getHitbox(), m_transform.getCurrentPosition(), getOrigin()); isCollide(GoombaAIHitbox, hitbox_mario))
        Mario::PowerDown();
}
void RedSpiny::statusUpdate(const float deltaTime) {
    if (isDestroyed()) return;

    if (m_state == 0) {
        if (Scroll::isOutOfScreenYBottom(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0))
            Destroy();
        if (!Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0)) {
            if (isDisabled()) setDisabled(false);
        }
    }
    if (m_state == 1)
        if (Scroll::isOutOfScreenYBottom(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0)) Destroy();
}
void RedSpiny::XUpdate(const float deltaTime) {
    if (isDestroyed() || isDisabled() || m_state > 0) return;
    auto data = GoombaAIBehavior::GoombaAIXMove(GoombaAIBehavior::GoombaAIData(m_transform.getCurrentPosition(), m_velocity, getDirection()), deltaTime);
    data = GoombaAIBehavior::GoombaAIXCollision(data, getHitbox(), m_wall_hitbox, getOrigin());
    m_transform.setCurrentPosition(data.position);
    m_velocity = data.velocity;
    setDirection(data.direction);
}
void RedSpiny::YUpdate(const float deltaTime) {
    if (isDestroyed() || isDisabled()) return;
    if (m_state < 1) {
        auto data = GoombaAIBehavior::GoombaAIYMove(GoombaAIBehavior::GoombaAIData(m_transform.getCurrentPosition(), m_velocity, getDirection()), deltaTime);
        data = GoombaAIBehavior::GoombaAIYCollision(data, getHitbox(), getOrigin());
        m_transform.setCurrentPosition(data.position);
        m_velocity = data.velocity;
        setDirection(data.direction);
    }
    else {
        const auto data = GoombaAIBehavior::GoombaAIEffectYMove(GoombaAIBehavior::GoombaAIData(m_transform.getCurrentPosition(), m_velocity, getDirection()), deltaTime);
        m_velocity = data.velocity;
        m_transform.setCurrentPosition(data.position);
    }
}

void RedSpiny::BlockHit() {
    if (m_state > 0) return;
    AddScoreEffect(ScoreID::SCORE_100, m_transform.getCurrentPosition().x, m_transform.getCurrentPosition().y - getOrigin().y);
    SoundManager::PlaySound(SoundID::GAME_KICK2);
    Death(1);
}

void RedSpiny::ShellHit() {
    if (m_state > 0) return;
    Death(1);
}

void RedSpiny::Destroy() {
    if (!isDestroyed()) {
        m_transform.destroy();
        m_enemyManager.setDeletionFlag(true);
    }
}

void RedSpiny::draw(float alpha) {
    m_animation.setAnimationDirection(static_cast<AnimationDirection>(!getDirection()));
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) {
        m_animation.frameUpdate();
        return;
    }
    m_animation.setColor(sf::Color(255, 255, 255));
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), m_transform.getCurrentPosition() - getOrigin(), sf::Color::Red));
}
void RedSpiny::Death(unsigned int state) {
    setCollideEachOther(false);
    m_state = state;
    switch (m_state) {
        case 1:
            setDisabled(false);
            m_velocity = sf::Vector2f(0.f, -3.f);
            m_animation.setAnimationSequence("RedSpinyDeathEffect");
            m_animation.setAnimation(0,0,100, true);
            setShellKicking(false);
            setShellBlocker(false);
            setDrawingPriority(3);
            break;
        default:;
    }
}

bool RedSpiny::isDeath() {
    return m_state > 0;
}

void RedSpiny::animationUpdate(float deltaTime) {
    m_animation.frameTimeAccumulate(deltaTime);
}

sf::Vector2f RedSpiny::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f RedSpiny::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect RedSpiny::getHitbox() {
    return m_hitbox;
}

bool RedSpiny::isDestroyed() {
    return m_transform.isDestroyed();
}
