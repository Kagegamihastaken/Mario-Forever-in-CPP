#include "Object/Projectile/MarioFireball.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/ProjectileManager.hpp"
#include "Core/Object/Projectile/Behavior/FireballBehavior.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Effect/FireballExplosion.hpp"
#include "Effect/ScoreEffect.hpp"

MarioFireball::MarioFireball(ProjectileManager &manager, const bool direction, const sf::Vector2f &position) : Projectile(manager), m_direction(direction) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_animation.setTexture("Fireball", true);
    setOrigin(sf::Vector2f(7.f, 16.f));
    setHitbox(sf::FloatRect({0.f, 0.f}, {15.f, 16.f}));
    m_velocity = {8.125f, 0.f};
}

void MarioFireball::setPreviousData() {
    if (isDestroyed()) return;
    setPreviousPosition(getCurrentPosition());
    setPreviousAngle(getCurrentAngle());
}
void MarioFireball::interpolateData(const float alpha) {
    if (isDestroyed()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
    setInterpolatedAngle(linearInterpolation(getPreviousAngle(), getCurrentAngle(), alpha));
}

void MarioFireball::FireballEffect() {
    AddFireballExplosion(getCurrentPosition().x, getCurrentPosition().y - 7.f);
}

void MarioFireball::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    //Spin
    if (m_direction) setCurrentAngle(getCurrentAngle() - sf::degrees(11.5f * deltaTime));
    else setCurrentAngle(getCurrentAngle() + sf::degrees(11.5f * deltaTime));
    //Status
    if (isOutOfScreen(MFCPP::CollisionObject(getCurrentPosition(), getOrigin(), getHitbox()), 32.f)) {
        Destroy();
        return;
    }
    //Movement
    auto data = FireballBehavior::FireballX(getCurrentPosition(), m_velocity, m_direction, deltaTime, getHitbox(), getOrigin());
    if (data.remove) {
        FireballEffect();
        Destroy();
        return;
    }
    setCurrentPosition(data.position);
    m_velocity = data.velocity;
    bool bounce = false;
    data = FireballBehavior::FireballY(getCurrentPosition(), bounce, m_velocity, deltaTime, getHitbox(), getOrigin());
    if (data.remove) {
        FireballEffect();
        Destroy();
        return;
    }
    if (bounce)
        m_velocity.y = -5.f;
    setCurrentPosition(data.position);
    if (!bounce) m_velocity.y = data.velocity.y;
}

void MarioFireball::CollisionUpdate() {
    const sf::FloatRect playerHitbox = getGlobalHitbox(getHitbox(), getCurrentPosition(), getOrigin());
    //GoombaAI
    auto& list = GameScene::enemyManager.getGoombaAIList();
    for (auto & jt : list) {
        if (sf::FloatRect EnemyGoombaAICollide = getGlobalHitbox(jt.getHitbox(), jt.getCurrentPosition(), jt.getOrigin()); isCollide(EnemyGoombaAICollide, playerHitbox)) {
            if (!jt.isDeath()) {
                jt.ShellHit();
                AddScoreEffect(SCORE_100, jt.getCurrentPosition().x, jt.getCurrentPosition().y - jt.getOrigin().y);
                SoundManager::PlaySound("Kick2");
                FireballEffect();
                Destroy();
                break;
            }
        }
    }
    //BroAI
    auto& BroList = GameScene::enemyManager.getBroAIList();
    for (auto & jt : BroList) {
        if (sf::FloatRect loopHitbox = getGlobalHitbox(jt.getHitbox(), jt.getCurrentPosition(), jt.getOrigin()); isCollide(loopHitbox, playerHitbox)) {
            if (!jt.isDeath()) {
                jt.ShellHit();
                AddScoreEffect(SCORE_200, jt.getCurrentPosition().x, jt.getCurrentPosition().y - jt.getOrigin().y);
                SoundManager::PlaySound("Kick2");
                FireballEffect();
                Destroy();
                break;
            }
        }
    }
    //Piranha
    auto& PiranhaList = GameScene::enemyManager.getPiranhaAIList();
    for (auto & jt : PiranhaList) {
        if (sf::FloatRect loopHitbox = getGlobalHitbox(jt.getHitbox(), jt.getCurrentPosition(), jt.getOrigin()); isCollide(loopHitbox, playerHitbox)) {
            if (!jt.isDeath()) {
                jt.Death(0);
                AddScoreEffect(SCORE_100, jt.getCurrentPosition().x, jt.getCurrentPosition().y - jt.getOrigin().y);
                SoundManager::PlaySound("Kick2");
                FireballEffect();
                Destroy();
                break;
            }
        }
    }
}

void MarioFireball::draw() {
    //if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 32.f)) return;
    m_animation.setAnimationDirection(m_direction);
    m_animation.AnimationUpdate(getInterpolatedPosition() - sf::Vector2f(0.f, 7.f), getOrigin() - sf::Vector2f(0.f, 9.f));
    m_animation.setRotation(getInterpolatedAngle());
    m_animation.AnimationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), getCurrentPosition() - getOrigin(), sf::Color::Red));
}

void MarioFireball::Destroy() {
    setDestroyed(true);
    m_manager.setDeletionFlag(true);
}

void MarioFireball::LevelEndCleanup() {
    AddScoreEffect(SCORE_100, getCurrentPosition().x, getCurrentPosition().y - getOrigin().y);
    Destroy();
}
