#include "Object/Projectile/MarioBeetroot.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/ProjectileManager.hpp"
#include "Core/Object/Projectile/Behavior/BeetrootBehavior.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Effect/FireballExplosion.hpp"
#include "Effect/ScoreEffect.hpp"

MarioBeetroot::MarioBeetroot(ProjectileManager &manager, const bool direction, const sf::Vector2f &position) : Projectile(manager), m_direction(direction) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_animation.setTexture("Beetroot_Projectile");
    setOrigin(sf::Vector2f(11.f, 31.f));
    setHitbox(sf::FloatRect({0.f, 0.f}, {23.f, 32.f}));
    m_velocity = {2.125f, -5.f};
    m_hit_count = 0.f;
    setDrawingPriority(1);
}

void MarioBeetroot::spin(const bool flip, const bool sound = true) {
    //Collision With CustomTile
    for (auto& bonus : GameScene::customTileManager.getBonusList()) {
        if (const sf::FloatRect BonusHitbox = getGlobalHitbox(bonus.getHitbox(), bonus.getCurrentPosition(), bonus.getOrigin()); isCollide(BonusHitbox, getGlobalHitbox(getHitbox(), getCurrentPosition(), getOrigin())))
            bonus.KickEvent();
    }
    //spin & change
    if (flip) {
        AddFireballExplosion(getCurrentPosition().x, getCurrentPosition().y - getOrigin().y);
        m_velocity.y = 4.f;
    }
    else {
        AddFireballExplosion(getCurrentPosition().x, getCurrentPosition().y);
        m_velocity.x = (10 + RandomIntNumberGenerator(0, 31)) / 8.f;
        m_direction = !m_direction;
        m_velocity.y = -8.f;
    }
    if (sound)
        SoundManager::PlaySound("Stun");
}

void MarioBeetroot::setPreviousData() {
    if (isDestroyed()) return;
    setPreviousPosition(getCurrentPosition());
}
void MarioBeetroot::interpolateData(const float alpha) {
    if (isDestroyed()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}
void MarioBeetroot::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    //Status
    if (isOutOfScreenYBottom(MFCPP::CollisionObject(getCurrentPosition(), getOrigin(), getHitbox()), 0.f) ||
        isOutOfScreenXLeft(MFCPP::CollisionObject(getCurrentPosition(), getOrigin(), getHitbox()), 0.f) ||
        isOutOfScreenXRight(MFCPP::CollisionObject(getCurrentPosition(), getOrigin(), getHitbox()), 0.f)) {
        Destroy();
        return;
    }
    //Movement
    auto data = BeetrootBehavior::BeetrootXUpdate(getCurrentPosition(), m_hit_count, m_velocity, m_direction, deltaTime);
    setCurrentPosition(data.position);
    m_velocity = data.velocity;

    int bounce = 0;
    data = BeetrootBehavior::BeetrootXCollision(getCurrentPosition(), m_hit_count, bounce, m_velocity, m_direction, deltaTime, getHitbox(), getOrigin());
    if (bounce == 1)
        spin(false);
    setCurrentPosition(data.position);
    m_hit_count = data.count;

    data = BeetrootBehavior::BeetrootYUpdate(getCurrentPosition(), m_hit_count, m_velocity, deltaTime);
    setCurrentPosition(data.position);
    m_velocity = data.velocity;

    data = BeetrootBehavior::BeetrootYCollision(getCurrentPosition(), m_hit_count, bounce, m_velocity, deltaTime, getHitbox(), getOrigin());
    if (bounce == 1)
        spin(false);
    else if (bounce == 2)
        spin(true);
    setCurrentPosition(data.position);
    if (!bounce) m_velocity.y = data.velocity.y;
    m_hit_count = data.count;
}

void MarioBeetroot::CollisionUpdate() {
    if (m_hit_count > 2) return;
    const sf::FloatRect playerHitbox = getGlobalHitbox(getHitbox(), getCurrentPosition(), getOrigin());
    //GoombaAI
    auto& list = GameScene::enemyManager.getGoombaAIList();
    for (auto & jt : list) {
        if (sf::FloatRect EnemyGoombaAICollide = getGlobalHitbox(jt.getHitbox(), jt.getCurrentPosition(), jt.getOrigin()); isCollide(EnemyGoombaAICollide, playerHitbox)) {
            if (!jt.isDeath()) {
                jt.ShellHit();
                AddScoreEffect(SCORE_100, jt.getCurrentPosition().x, jt.getCurrentPosition().y - jt.getOrigin().y);
                SoundManager::PlaySound("Kick2");
                spin(false, false);
                ++m_hit_count;
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
                spin(false, false);
                ++m_hit_count;
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
                spin(false, false);
                ++m_hit_count;
                break;
            }
        }
    }
}

void MarioBeetroot::draw() {
    //if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 32.f)) return;
    m_animation.animationUpdate(getInterpolatedPosition() - sf::Vector2f(0.f, 7.f), getOrigin() - sf::Vector2f(0.f, 9.f));
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), getCurrentPosition() - getOrigin(), sf::Color::Red));
}

void MarioBeetroot::Destroy() {
    setDestroyed(true);
    m_manager.setDeletionFlag(true);
}

void MarioBeetroot::LevelEndCleanup() {
    AddScoreEffect(SCORE_100, getCurrentPosition().x, getCurrentPosition().y - getOrigin().y);
    Destroy();
}
