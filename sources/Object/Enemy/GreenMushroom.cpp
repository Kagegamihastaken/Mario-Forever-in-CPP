#include "Object/Enemy/GreenMushroom.hpp"
#include "Core/Object/EnemyManager.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "../../../headers/Core/Object/Enemy/Behavior/GoombaAIBehavior.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Object/Mario.hpp"

GreenMushroom::GreenMushroom(EnemyManager &manager, const sf::Vector2f& position) : Enemy(manager) {
    setCurrentPosition(position + sf::Vector2f(0.f, 31.f));
    setPreviousPosition(getCurrentPosition());
    setInterpolatedPosition(getCurrentPosition());
    m_animation.setTexture("GreenMushroom_0");
    setHitbox(sf::FloatRect({0.f, 0.f}, {31.f, 32.f}));
    m_wall_hitbox = sf::FloatRect(getHitbox().position, getHitbox().size - sf::Vector2f(0.f, 6.f));
    setOrigin(sf::Vector2f(16.f, 31.f));
    m_velocity = sf::Vector2f(2.f, 0.f);
    setDirection(true);
    setDisabled(true);
    m_appearingSpeed = 1.5f;
    m_appearingY = 0.f;
    m_appearingYMax = 32.f;
    m_isappearing = true;
    setCollideEachOther(false);
    setShellKicking(false);
    setShellBlocker(false);

    setDrawingPriority(1);
}
void GreenMushroom::setPreviousData() {
    if (isDestroyed() || isDisabled()) return;
    setPreviousPosition(getCurrentPosition());
}
void GreenMushroom::interpolateData(const float alpha) {
    if (isDestroyed() || isDisabled()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}
void GreenMushroom::EnemyCollision() {}
void GreenMushroom::MarioCollision(const float MarioYVelocity) {
    if (isDestroyed() || isDisabled() || m_isappearing) return;
    if (f_abs(player.curr.x - getCurrentPosition().x) >= 80.0f) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(player.hitboxMain, player.curr, player.property.getOrigin());
    if (const sf::FloatRect GoombaAIHitbox = getGlobalHitbox(getHitbox(), getCurrentPosition(), getOrigin()); isCollide(GoombaAIHitbox, hitbox_mario)) {
        AddScoreEffect(SCORE_1UP, getCurrentPosition().x, getCurrentPosition().y - getOrigin().y);
        if (PowerState == 0) SetPowerState(1);
        Destroy();
    }
}
void GreenMushroom::statusUpdate(const float deltaTime) {
    if (isDestroyed()) return;

    if (isOutScreenYBottom(getCurrentPosition().y, 80))
        Destroy();
    if (!isOutScreen(getCurrentPosition().x - getOrigin().x, getCurrentPosition().y, 32, 80)) {
        if (isDisabled()) setDisabled(false);
    }
}
void GreenMushroom::XUpdate(const float deltaTime) {
    if (isDestroyed() || isDisabled() || m_isappearing) return;
    auto data = GoombaAIBehavior::GoombaAIXMove(GoombaAIBehavior::GoombaAIData(getCurrentPosition(), m_velocity, getDirection()), deltaTime);
    data = GoombaAIBehavior::GoombaAIXCollision(data, getHitbox(), m_wall_hitbox, getOrigin());
    setCurrentPosition(data.position);
    m_velocity = data.velocity;
    setDirection(data.direction);
}
void GreenMushroom::YUpdate(const float deltaTime) {
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
void GreenMushroom::BlockHit() {}
void GreenMushroom::ShellHit() {}
void GreenMushroom::Destroy() {
    if (!isDestroyed()) {
        setDestroyed(true);
        m_enemyManager.setDeletionFlag(true);
    }
}
void GreenMushroom::draw() {
    if (isOutScreen(getInterpolatedPosition().x - getOrigin().x, getInterpolatedPosition().y, 32, 80)) return;
    m_animation.AnimationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.AnimationDraw();
}
void GreenMushroom::Death(unsigned int state) {}
bool GreenMushroom::isDeath() {
    return true; // Means no collision
}
