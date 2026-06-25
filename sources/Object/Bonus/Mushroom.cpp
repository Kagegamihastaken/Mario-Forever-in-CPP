#include "Object/Bonus/Mushroom.hpp"
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

Mushroom::Mushroom(EnemyManager &manager, const sf::Vector2f& position)
    : Enemy(manager),
    m_transform(position + sf::Vector2f(0.f, 31.f), sf::Vector2f(16.f, 31.f), sf::degrees(0.f))
    {
    m_animation.setTexture("Mushroom_0");
    m_hitbox = sf::FloatRect({0.f, 0.f}, {31.f, 32.f});
    m_wall_hitbox = sf::FloatRect(m_hitbox.position, m_hitbox.size - sf::Vector2f(0.f, 6.f));
    m_velocity = sf::Vector2f(2.f, 0.f);
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
void Mushroom::updatePreviousData() {
    if (isDestroyed() || isDisabled()) return;
    m_transform.Update();
}
void Mushroom::EnemyCollision() {}
void Mushroom::MarioCollision(const float MarioYVelocity) {
    if (m_transform.isDestroyed() || isDisabled() || m_isappearing) return;
    if (Utility::f_abs(Mario::getCurrentPosition().x - m_transform.getCurrentPosition().x) >= 80.0f) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(Mario::getHitbox(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (const sf::FloatRect GoombaAIHitbox = getGlobalHitbox(m_hitbox, m_transform.getCurrentPosition(), getOrigin()); isCollide(GoombaAIHitbox, hitbox_mario)) {
        SoundManager::PlaySound("Powerup");
        AddScoreEffect(ScoreID::SCORE_1000, m_transform.getCurrentPosition().x, m_transform.getCurrentPosition().y - getOrigin().y);
        if (Mario::getPowerState() == 0) Mario::SetPowerState(1);
        Destroy();
    }
}
void Mushroom::statusUpdate(const float deltaTime) {
    if (isDestroyed()) return;

    if (Scroll::isOutOfScreenYBottom(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), m_hitbox), 0))
        Destroy();
    if (!Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), m_hitbox), 0)) {
        if (isDisabled()) setDisabled(false);
    }
}
void Mushroom::XUpdate(const float deltaTime) {
    if (isDestroyed() || isDisabled() || m_isappearing) return;
    auto data = GoombaAIBehavior::GoombaAIXMove(GoombaAIBehavior::GoombaAIData(m_transform.getCurrentPosition(), m_velocity, getDirection()), deltaTime);
    data = GoombaAIBehavior::GoombaAIXCollision(data, m_hitbox, m_wall_hitbox, getOrigin());
    m_transform.setCurrentPosition(data.position);
    m_velocity = data.velocity;
    setDirection(data.direction);
}
void Mushroom::YUpdate(const float deltaTime) {
    if (isDestroyed() || isDisabled()) return;
    if (!m_isappearing) {
        auto data = GoombaAIBehavior::GoombaAIYMove(GoombaAIBehavior::GoombaAIData(m_transform.getCurrentPosition(), m_velocity, getDirection()), deltaTime);
        data = GoombaAIBehavior::GoombaAIYCollision(data, m_hitbox, getOrigin());
        m_transform.setCurrentPosition(data.position);
        m_velocity = data.velocity;
        setDirection(data.direction);
    }
    else {
        const auto data = GoombaAIBehavior::MushroomAppearing(GoombaAIBehavior::GoombaAIData(m_transform.getCurrentPosition(), m_velocity, getDirection()), m_appearingY, m_appearingSpeed, m_appearingYMax, m_isappearing, isDisabled(), deltaTime);
        m_transform.setCurrentPosition(data.position);
        m_velocity = data.velocity;
        setDirection(data.direction);
    }
}
void Mushroom::BlockHit() {}
void Mushroom::ShellHit() {}
void Mushroom::Destroy() {
    if (!isDestroyed()) {
        m_transform.destroy();
        m_enemyManager.setDeletionFlag(true);
    }
}
void Mushroom::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), m_hitbox), 0)) return;
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(m_hitbox, m_transform.getCurrentPosition() - getOrigin(), sf::Color::Magenta));
}
void Mushroom::Death(unsigned int state) {}
bool Mushroom::isDeath() {
    return true; // Means no collision
}

void Mushroom::animationUpdate(float deltaTime) {}

sf::Vector2f Mushroom::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f Mushroom::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect Mushroom::getHitbox() {
    return m_hitbox;
}

bool Mushroom::isDestroyed() {
    return m_transform.isDestroyed();
}
