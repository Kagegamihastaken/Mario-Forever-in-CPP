#include "Object/Projectile/MarioBeetroot.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Utility.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/ProjectileManager.hpp"
#include "Core/Object/Projectile/Behavior/BeetrootBehavior.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Effect/FireballExplosion.hpp"
#include "Effect/ScoreEffect.hpp"

MarioBeetroot::MarioBeetroot(ProjectileManager &manager, const bool direction, const sf::Vector2f &position)
    : Projectile(manager),
    m_transform(position, sf::Vector2f(11.f, 31.f), sf::degrees(0.f)),
    m_direction(direction) {
    m_animation.setTexture("Beetroot_Projectile");
    m_hitbox = sf::FloatRect({0.f, 0.f}, {23.f, 32.f});
    m_velocity = {2.125f, -5.f};
    m_hit_count = 0.f;
    setDrawingPriority(2);
}

void MarioBeetroot::spin(const bool flip, const bool sound = true) {
    //Collision With CustomTile
    for (auto& bonus : GameScene::customTileManager.getBonusList()) {
        if (const sf::FloatRect BonusHitbox = getGlobalHitbox(bonus.getHitbox(), bonus.getPosition(), bonus.getOrigin()); isCollide(BonusHitbox, getGlobalHitbox(getHitbox(), m_transform.getCurrentPosition(), getOrigin())))
            bonus.KickEvent();
    }
    //spin & change
    if (flip) {
        AddFireballExplosion(m_transform.getCurrentPosition().x, m_transform.getCurrentPosition().y - getOrigin().y);
        m_velocity.y = 4.f;
    }
    else {
        AddFireballExplosion(m_transform.getCurrentPosition().x, m_transform.getCurrentPosition().y);
        m_velocity.x = (10 + Utility::RandomIntNumberGenerator(0, 31)) / 8.f;
        m_direction = !m_direction;
        m_velocity.y = -8.f;
    }
    if (sound)
        SoundManager::PlaySound(SoundID::GAME_STUN);
}

void MarioBeetroot::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}
void MarioBeetroot::statusUpdate(float deltaTime) {
    if (isDestroyed()) return;
    //Status
    if (Scroll::isOutOfScreenYBottom(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0.f) ||
        Scroll::isOutOfScreenXLeft(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0.f) ||
        Scroll::isOutOfScreenXRight(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()), 0.f)) {
        Destroy();
        return;
    }
    //Movement
    auto data = BeetrootBehavior::BeetrootXUpdate(m_transform.getCurrentPosition(), m_hit_count, m_velocity, m_direction, deltaTime);
    m_transform.setCurrentPosition(data.position);
    m_velocity = data.velocity;

    int bounce = 0;
    data = BeetrootBehavior::BeetrootXCollision(m_transform.getCurrentPosition(), m_hit_count, bounce, m_velocity, m_direction, deltaTime, getHitbox(), getOrigin());
    if (bounce == 1)
        spin(false);
    m_transform.setCurrentPosition(data.position);
    m_hit_count = data.count;

    data = BeetrootBehavior::BeetrootYUpdate(m_transform.getCurrentPosition(), m_hit_count, m_velocity, deltaTime);
    m_transform.setCurrentPosition(data.position);
    m_velocity = data.velocity;

    data = BeetrootBehavior::BeetrootYCollision(m_transform.getCurrentPosition(), m_hit_count, bounce, m_velocity, deltaTime, getHitbox(), getOrigin());
    if (bounce == 1)
        spin(false);
    else if (bounce == 2)
        spin(true);
    m_transform.setCurrentPosition(data.position);
    if (!bounce) m_velocity.y = data.velocity.y;
    m_hit_count = data.count;
}

void MarioBeetroot::CollisionUpdate() {
    if (m_hit_count > 2) return;
    const sf::FloatRect playerHitbox = getGlobalHitbox(getHitbox(), m_transform.getCurrentPosition(), getOrigin());
    //GoombaAI
    auto& list = GameScene::enemyManager.getGoombaAIList();
    for (auto & jt : list) {
        if (sf::FloatRect EnemyGoombaAICollide = getGlobalHitbox(jt.getHitbox(), jt.getPosition(), jt.getOrigin()); isCollide(EnemyGoombaAICollide, playerHitbox)) {
            if (!jt.isDeath()) {
                jt.ShellHit();
                AddScoreEffect(ScoreID::SCORE_100, jt.getPosition().x, jt.getPosition().y - jt.getOrigin().y);
                SoundManager::PlaySound(SoundID::GAME_KICK2);
                spin(false, false);
                ++m_hit_count;
                break;
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
                spin(false, false);
                ++m_hit_count;
                break;
            }
        }
    }
    //Piranha
    auto& PiranhaList = GameScene::enemyManager.getPiranhaAIList();
    for (auto & jt : PiranhaList) {
        if (sf::FloatRect loopHitbox = getGlobalHitbox(jt.getHitbox(), jt.getPosition(), jt.getOrigin()); isCollide(loopHitbox, playerHitbox)) {
            if (!jt.isDeath()) {
                jt.Death(0);
                AddScoreEffect(ScoreID::SCORE_100, jt.getPosition().x, jt.getPosition().y - jt.getOrigin().y);
                SoundManager::PlaySound(SoundID::GAME_KICK2);
                spin(false, false);
                ++m_hit_count;
                break;
            }
        }
    }
}

void MarioBeetroot::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 8.f)) return;
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha) - sf::Vector2f(0.f, 7.f), getOrigin() - sf::Vector2f(0.f, 9.f));
    m_animation.animationDraw();
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(getHitbox(), m_transform.getCurrentPosition() - getOrigin(), sf::Color::Red));
}

void MarioBeetroot::Destroy() {
    m_transform.destroy();
    m_manager.setDeletionFlag(true);
}

void MarioBeetroot::LevelEndCleanup() {
    AddScoreEffect(ScoreID::SCORE_100, m_transform.getCurrentPosition().x, m_transform.getCurrentPosition().y - getOrigin().y);
    Destroy();
}

void MarioBeetroot::animationUpdate(float deltaTime) {}

sf::Vector2f MarioBeetroot::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f MarioBeetroot::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect MarioBeetroot::getHitbox() {
    return m_hitbox;
}

bool MarioBeetroot::isDestroyed() {
    return m_transform.isDestroyed();
}
