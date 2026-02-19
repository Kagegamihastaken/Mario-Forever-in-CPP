#include "Object/Enemy/GreenKoopa.hpp"
#include "Core/Object/EnemyManager.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "../../../headers/Core/Object/Enemy/Behavior/GoombaAIBehavior.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Object/GoombaAI.hpp"
#include "Effect/GoombaAIEffect.hpp"
#include "Object/Mario.hpp"

GreenKoopa::GreenKoopa(EnemyManager &manager, const sf::Vector2f& position, bool isShell) : Enemy(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    setDirection(false);
    setDisabled(true);
    setCollideEachOther(true);
    setDrawingPriority(1);
    if (!isShell) {
        setHitbox(sf::FloatRect({0.f, 0.f}, {32.f, 47.f}));
        m_wall_hitbox = sf::FloatRect(getHitbox().position, getHitbox().size - sf::Vector2f(0.f, 6.f));
        setOrigin(sf::Vector2f(16.f, 46.f));
        m_velocity = sf::Vector2f(1.f, 0.f);
        m_animation.setAnimation(0, 1, 11, true);
        m_animation.setAnimationSequence(GreenKoopaAnimName);
        m_state = 0;
    }
    else {
        setHitbox(sf::FloatRect({0.f, 0.f}, {32.f, 28.f}));
        m_wall_hitbox = sf::FloatRect(getHitbox().position, getHitbox().size - sf::Vector2f(0.f, 6.f));
        setOrigin(sf::Vector2f(16,27));
        m_velocity = sf::Vector2f(0.f, 0.f);
        m_animation.setAnimationSequence(GreenKoopaShellAnimName);
        m_animation.setAnimation(3,3,100);
        m_state = 1;
    }
    m_invincibleTimer = 0.f;
    m_hit_count = 0;
    setShellKicking(true);
    setShellBlocker(false);
}
void GreenKoopa::setPreviousData() {
    if (isDestroyed() || isDisabled()) return;
    setPreviousPosition(getCurrentPosition());
}
void GreenKoopa::interpolateData(const float alpha) {
    if (isDestroyed() || isDisabled()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}
void GreenKoopa::EnemyCollision() {
    if (isDestroyed() || isDisabled() || m_state == 3) return;
    if (m_state != 2) {
        auto& list = m_enemyManager.getGoombaAIList();
        auto it = list.iterator_to(*this);
        ++it;
        for (; it != list.end(); ++it) {
            Enemy* other = &*it;
            if (other->isDestroyed() || other->isDisabled()) continue;
            GoombaAIBehavior::GoombaAICollision(this, other);
        }
    }
    else {
        for (auto &enemy: m_enemyManager.getGoombaAIList()) {
            Enemy* other = &enemy;
            if (other == this) continue;
            if (other->isDestroyed() || other->isDisabled()) continue;
            GoombaAIBehavior::ShellMovingCollision(this, other, m_hit_count);
        }
        for (auto &enemy: m_enemyManager.getBroAIList()) {
            Enemy* other = &enemy;
            if (other == this) continue;
            if (other->isDestroyed() || other->isDisabled()) continue;
            GoombaAIBehavior::ShellMovingCollision(this, other, m_hit_count);
        }
    }
}
void GreenKoopa::MarioCollision(const float MarioYVelocity) {
    if (isDestroyed() || isDisabled() || m_state == 3) return;
    if (m_invincibleTimer < 0.12f * 50 && m_state == 1) return;
    if (m_invincibleTimer < 0.6f * 50 && m_state == 2) return;
    if (f_abs(player.curr.x - getCurrentPosition().x) >= 80.0f) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(player.hitboxMain, player.curr, player.property.getOrigin());
    if (const sf::FloatRect GoombaAIHitbox = getGlobalHitbox(getHitbox(), getCurrentPosition(), getOrigin()); isCollide(GoombaAIHitbox, hitbox_mario)) {
        m_invincibleTimer = 0.f;
        if (m_state != 1) {
            if (getCurrentPosition().y - 16.f > player.curr.y && MarioYVelocity > 0.0f) {
                GoombaAIBehavior::GoombaAIStomping();
                AddScoreEffect(SCORE_100, getCurrentPosition().x, getCurrentPosition().y - getOrigin().y);
                Death(1);
                return;
            }
            if (getCurrentPosition().y - 16.f < player.curr.y)
                PowerDown();
        }
        else if (m_state == 1) {
            SoundManager::PlaySound("Kick2");
            if (getCurrentPosition().x >= player.curr.x) setDirection(true);
            else setDirection(false);
            m_state = 2;
            ChangeState();
        }
    }
}
void GreenKoopa::statusUpdate(const float deltaTime) {
    if (isDestroyed()) return;

    if (m_invincibleTimer < 0.12f * 50 && m_state == 1) m_invincibleTimer += deltaTime;
    if (m_invincibleTimer < 0.6f * 50 && m_state == 2) m_invincibleTimer += deltaTime;


    if (m_state != 3) {
        if (isOutScreenYBottom(getCurrentPosition().y, 80))
            Destroy();
        if (!isOutScreen(getCurrentPosition().x - getOrigin().x, getCurrentPosition().y, 32, 80)) {
            if (isDisabled()) setDisabled(false);
        }
    }
    else
        if (isOutScreenYBottom(getCurrentPosition().y, 64)) Destroy();
}
void GreenKoopa::XUpdate(const float deltaTime) {
    if (isDestroyed() || isDisabled() || m_state == 3) return;
    auto data = GoombaAIBehavior::GoombaAIXMove(GoombaAIBehavior::GoombaAIData(getCurrentPosition(), m_velocity, getDirection()), deltaTime);
    if (m_state != 2) {
        data = GoombaAIBehavior::GoombaAIXCollision(data, getHitbox(), m_wall_hitbox, getOrigin());
        setCurrentPosition(data.position);
        m_velocity = data.velocity;
        setDirection(data.direction);
    }
    else {
        data = GoombaAIBehavior::ShellXCollision(data, getHitbox(), m_wall_hitbox, getOrigin());
        setCurrentPosition(data.position);
        m_velocity = data.velocity;
        setDirection(data.direction);
    }
}
void GreenKoopa::YUpdate(const float deltaTime) {
    if (isDestroyed() || isDisabled()) return;
    if (m_state != 3) {
        auto data = GoombaAIBehavior::GoombaAIYMove(GoombaAIBehavior::GoombaAIData(getCurrentPosition(), m_velocity, getDirection()), deltaTime);
        data = GoombaAIBehavior::GoombaAIYCollision(data, getHitbox(), getOrigin()); // true, getPreviousPosition().x
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

void GreenKoopa::BlockHit() {
    if (m_state > 0) return;
    AddScoreEffect(SCORE_100, getCurrentPosition().x, getCurrentPosition().y - getOrigin().y);
    SoundManager::PlaySound("Kick2");
    Death(3);
}

void GreenKoopa::Destroy() {
    if (!isDestroyed()) {
        setDestroyed(true);
        m_enemyManager.setDeletionFlag(true);
    }
}

void GreenKoopa::draw() {
    m_animation.setAnimationDirection(static_cast<AnimationDirection>(!getDirection()));
    if (isOutScreen(getInterpolatedPosition().x - getOrigin().x, getInterpolatedPosition().y, 32, 80)) return;
    m_animation.setColor(sf::Color(255, 255, 255));
    m_animation.AnimationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.AnimationDraw();
}

void GreenKoopa::ChangeState() {
    switch (m_state) {
        case 1:
            setHitbox(sf::FloatRect({0.f, 0.f}, {32.f, 28.f}));
            m_wall_hitbox = sf::FloatRect(getHitbox().position, getHitbox().size - sf::Vector2f(0.f, 6.f));
            setOrigin(sf::Vector2f(16,27));
            m_animation.setAnimationSequence(GreenKoopaShellAnimName);
            m_animation.setAnimation(3,3,100);
            setCollideEachOther(true);
            m_velocity.x = 0.f;
            m_hit_count = 0;
            setShellBlocker(false);
            setShellKicking(true);
            setDrawingPriority(1);
            break;
        case 2:
            setHitbox(sf::FloatRect({0.f, 0.f}, {32.f, 28.f}));
            m_wall_hitbox = sf::FloatRect(getHitbox().position, getHitbox().size - sf::Vector2f(0.f, 6.f));
            setOrigin(sf::Vector2f(16,27));
            m_animation.setAnimationSequence(GreenKoopaShellAnimName);
            m_animation.setAnimation(0,3,54);
            setCollideEachOther(false);
            m_velocity.x = 5.f;
            m_hit_count = 0;
            setShellBlocker(true);
            setShellKicking(true);
            setDrawingPriority(1);
            break;
        case 3:
            setHitbox(sf::FloatRect({0.f, 0.f}, {32.f, 28.f}));
            m_wall_hitbox = sf::FloatRect(getHitbox().position, getHitbox().size - sf::Vector2f(0.f, 6.f));
            setOrigin(sf::Vector2f(16,27));
            m_velocity = sf::Vector2f(0.f, -3.f);
            m_hit_count = 0;
            m_animation.setAnimationSequence(GreenKoopaDeathEffect);
            m_animation.setAnimation(0,0,100);
            setCollideEachOther(false);
            setShellBlocker(false);
            setShellKicking(false);
            setDrawingPriority(2);
            break;
        default:;
    }
}

void GreenKoopa::Death(unsigned int state) {
    m_state = state;
    ChangeState();
}
void GreenKoopa::ShellHit() {
    if (m_state == 3) return;
    Death(3);
}

bool GreenKoopa::isDeath() {
    return m_state == 3;
}
