#include "Object/Projectile/MarioFireball.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/ProjectileManager.hpp"
#include "Core/Object/Projectile/Behavior/FireballBehavior.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Effect/FireballExplosion.hpp"
#include "Effect/ScoreEffect.hpp"

MarioFireball::MarioFireball(ProjectileManager &manager, const bool direction, const sf::Vector2f &position)
    : Projectile(manager),
    m_direction(direction),
    m_transform(position, sf::Vector2f(7.f, 16.f), sf::degrees(0.f)){
    m_animation.setTexture("Fireball", true);
    m_hitbox = sf::FloatRect({0.f, 0.f}, {15.f, 16.f});
    m_velocity = {8.125f, 0.f};
    setDrawingPriority(2);
}

void MarioFireball::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void MarioFireball::FireballEffect() {
    AddFireballExplosion(m_transform.getCurrentPosition().x, m_transform.getCurrentPosition().y - 7.f);
}

void MarioFireball::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    //Spin
    if (m_direction) m_transform.rotate(- sf::degrees(11.5f * deltaTime));
    else m_transform.rotate(sf::degrees(11.5f * deltaTime));
    //Status
    if (Scroll::isOutOfScreenYBottom(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0.f) ||
        Scroll::isOutOfScreenXLeft(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0.f) ||
        Scroll::isOutOfScreenXRight(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0.f)) {
        Destroy();
        return;
    }
    //Movement
    auto data = FireballBehavior::FireballX(m_transform.getCurrentPosition(), m_velocity, m_direction, deltaTime, getHitbox(), getOrigin());
    if (data.remove) {
        FireballEffect();
        Destroy();
        return;
    }
    m_transform.setCurrentPosition(data.position);
    m_velocity = data.velocity;
    bool bounce = false;
    data = FireballBehavior::FireballY(m_transform.getCurrentPosition(), bounce, m_velocity, deltaTime, getHitbox(), getOrigin());
    if (data.remove) {
        FireballEffect();
        Destroy();
        return;
    }
    if (bounce)
        m_velocity.y = -5.f;
    m_transform.setCurrentPosition(data.position);
    if (!bounce) m_velocity.y = data.velocity.y;
}

void MarioFireball::CollisionUpdate() {
    const sf::FloatRect playerHitbox = getGlobalHitbox(getHitbox(), m_transform.getCurrentPosition(), getOrigin());
    //GoombaAI
    auto& list = GameScene::enemyManager.getGoombaAIList();
    for (auto & jt : list) {
        if (sf::FloatRect EnemyGoombaAICollide = getGlobalHitbox(jt.getHitbox(), jt.getPosition(), jt.getOrigin()); isCollide(EnemyGoombaAICollide, playerHitbox)) {
            if (!jt.isDeath()) {
                jt.ShellHit();
                AddScoreEffect(ScoreID::SCORE_100, jt.getPosition().x, jt.getPosition().y - jt.getOrigin().y);
                SoundManager::PlaySound(SoundID::GAME_KICK2);
                FireballEffect();
                Destroy();
                return;
            }
        }
    }
    //BroAI
    auto& BroList = GameScene::enemyManager.getBroAIList();
    for (auto & jt : BroList) {
        if (sf::FloatRect loopHitbox = getGlobalHitbox(jt.getHitbox(), jt.getPosition(), jt.getOrigin()); isCollide(loopHitbox, playerHitbox)) {
            if (!jt.isDeath()) {
                jt.ShellHit();
                AddScoreEffect(ScoreID::SCORE_200, jt.getPosition().x, jt.getPosition().y - jt.getOrigin().y);
                SoundManager::PlaySound(SoundID::GAME_KICK2);
                FireballEffect();
                Destroy();
                return;
            }
        }
    }
    //Piranha
    auto& PiranhaList = GameScene::enemyManager.getPiranhaAIList();
    for (auto & jt : PiranhaList) {
        if (sf::FloatRect loopPiranhaHitbox = getGlobalHitbox(jt.getHitbox(), jt.getPosition(), jt.getOrigin()); isCollide(loopPiranhaHitbox, playerHitbox)) {
            if (!jt.isDeath()) {
                jt.Death(0);
                AddScoreEffect(ScoreID::SCORE_100, jt.getPosition().x, jt.getPosition().y - jt.getOrigin().y);
                SoundManager::PlaySound(SoundID::GAME_KICK2);
                FireballEffect();
                Destroy();
                return;
            }
        }
    }
}

void MarioFireball::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 8.f)) return;
    m_animation.setAnimationDirection(m_direction);
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha) - sf::Vector2f(0.f, 7.f), getOrigin() - sf::Vector2f(0.f, 9.f));
    m_animation.setRotation(m_transform.getInterpolatedAngle(alpha));
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), m_transform.getCurrentPosition() - getOrigin(), sf::Color::Red));
}

void MarioFireball::Destroy() {
    m_transform.destroy();
    m_manager.setDeletionFlag(true);
}

void MarioFireball::LevelEndCleanup() {
    AddScoreEffect(ScoreID::SCORE_100, m_transform.getCurrentPosition().x, m_transform.getCurrentPosition().y - getOrigin().y);
    Destroy();
}

void MarioFireball::animationUpdate(float deltaTime) {}

sf::Vector2f MarioFireball::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f MarioFireball::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect MarioFireball::getHitbox() {
    return m_hitbox;
}

bool MarioFireball::isDestroyed() {
    return m_transform.isDestroyed();
}
