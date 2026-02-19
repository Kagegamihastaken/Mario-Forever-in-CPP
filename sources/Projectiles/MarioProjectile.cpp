#include "Core/Class/MarioProjectileClass.hpp"
#include "Core/Interpolation.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Scroll.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/SoundManager.hpp"
#include "Effect/FireballExplosion.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Object/BroAI.hpp"
#include "Object/GoombaAI.hpp"
#include "Object/PiranhaAI.hpp"
#include "Object/Mario.hpp"
#include "Projectiles/MarioProjectile.hpp"

#include "Core/ProjectileBehaviour/Fireball.hpp"
#include "Core/ProjectileBehaviour/Fireball.hpp"
#include "Core/Scene/GameScene.hpp"

plf::colony<MFCPP::MarioProjectile> MarioProjectileList;
static bool MarioProjectileDeleteGate = false;

int getAmountProjectile() {
    return static_cast<int>(MarioProjectileList.size());
}
void SetPrevMarioProjectilePos() {
    for (auto & i : MarioProjectileList) {
        if (i.isDestroyed()) continue;

        i.setPreviousPosition(i.getCurrentPosition());
        i.setPreviousAngle(i.getCurrentAngle());
    }
}
void InterpolateMarioProjectilePos(const float alpha) {
    for (auto & i : MarioProjectileList) {
        if (i.isDestroyed()) continue;

        i.setInterpolatedPosition(linearInterpolation(i.getPreviousPosition(), i.getCurrentPosition(), alpha));
        i.setInterpolatedAngle(linearInterpolation(i.getPreviousAngle(), i.getCurrentAngle(), alpha));
    }
}
void DeleteMarioProjectile(const plf::colony<MFCPP::MarioProjectile>::colony_iterator<false>& it, const bool out = false) {
    if (!it->isDestroyed() && !out) AddFireballExplosion(it->getCurrentPosition().x, it->getCurrentPosition().y - 7.f);
    it->setDestroyed(true);
    MarioProjectileDeleteGate = true;
}
void DeleteMarioProjectile(const float x, const float y) {
    for (auto it = MarioProjectileList.begin(); it != MarioProjectileList.end(); ++it) {
        if (it->getCurrentPosition().x == x && it->getCurrentPosition().y == y) {
            DeleteMarioProjectile(it);
        }
    }
}
void DeleteAllMarioProjectile() {
    MarioProjectileList.clear();
}
void MarioProjectileCollision() {
    if (MarioProjectileList.empty()) return;
    for (auto it = MarioProjectileList.begin(); it != MarioProjectileList.end(); ++it) {
        if (it->isDestroyed()) continue;
        const sf::FloatRect playerHitbox = getGlobalHitbox(it->getHitbox(), it->getCurrentPosition(), it->getOrigin());
        //GoombaAI
        auto& list = GameScene::enemyManager.getGoombaAIList();
        for (auto jt = list.begin(); jt != list.end(); ++jt) {
            if (sf::FloatRect EnemyGoombaAICollide = getGlobalHitbox(jt->getHitbox(), jt->getCurrentPosition(), jt->getOrigin()); isCollide(EnemyGoombaAICollide, playerHitbox)) {
                if (!jt->isDeath()) {
                    jt->ShellHit();
                    AddScoreEffect(SCORE_100, jt->getCurrentPosition().x, jt->getCurrentPosition().y - jt->getOrigin().y);
                    SoundManager::PlaySound("Kick2");
                    DeleteMarioProjectile(it);
                    break;
                }
            }
        }
        //BroAI
        auto& BroList = GameScene::enemyManager.getBroAIList();
        for (auto jt = BroList.begin(); jt != BroList.end(); ++jt) {
            if (sf::FloatRect loopHitbox = getGlobalHitbox(jt->getHitbox(), jt->getCurrentPosition(), jt->getOrigin()); isCollide(loopHitbox, playerHitbox)) {
                if (!jt->isDeath()) {
                    jt->ShellHit();
                    AddScoreEffect(SCORE_200, jt->getCurrentPosition().x, jt->getCurrentPosition().y - jt->getOrigin().y);
                    SoundManager::PlaySound("Kick2");
                    DeleteMarioProjectile(it);
                    break;
                }
            }
        }
        //Piranha
        auto& PiranhaList = GameScene::enemyManager.getPiranhaAIList();
        for (auto jt = PiranhaList.begin(); jt != PiranhaList.end(); ++jt) {
            if (sf::FloatRect loopHitbox = getGlobalHitbox(jt->getHitbox(), jt->getCurrentPosition(), jt->getOrigin()); isCollide(loopHitbox, playerHitbox)) {
                if (!jt->isDeath()) {
                    jt->Death(0);
                    AddScoreEffect(SCORE_100, jt->getCurrentPosition().x, jt->getCurrentPosition().y - jt->getOrigin().y);
                    SoundManager::PlaySound("Kick2");
                    DeleteMarioProjectile(it);
                    break;
                }
            }
        }
    }
}
void MarioProjectileStatusUpdate() {
    for (auto it = MarioProjectileList.begin(); it != MarioProjectileList.end(); ++it) {
        if (isOutScreen(it->getInterpolatedPosition().x, it->getInterpolatedPosition().y, 32.f, 32.f)) {
            DeleteMarioProjectile(it, true);
            //DeleteMarioProjectile(MarioProjectileList[i].getCurrentPosition().x, MarioProjectileList[i].getCurrentPosition().y);
        }
    }
}
void MarioProjectileSpin(const float deltaTime) {
    for (auto it = MarioProjectileList.begin(); it != MarioProjectileList.end(); ++it) {
        if (it->isDestroyed()) return;

        if (it->getDirection()) it->setCurrentAngle(it->getCurrentAngle() - sf::degrees(11.5f * deltaTime));
        else it->setCurrentAngle(it->getCurrentAngle() + sf::degrees(11.5f * deltaTime));
    }
}
void LevelEndMarioProjectileCleanup() {
    for (auto it = MarioProjectileList.begin(); it != MarioProjectileList.end(); ++it) {
        if (it->isDestroyed()) continue;

        AddScoreEffect(SCORE_100, it->getCurrentPosition().x, it->getCurrentPosition().y - it->getOrigin().y);
        it->setDestroyed(true);
    }
}
void AddMarioProjectile(const bool direction, const MarioProjectileType type, const float x, const float y) {
    plf::colony<MFCPP::MarioProjectile>::colony_iterator<false> it;
    switch (type) {
        case MarioProjectileType::FIREBALL:
            it = MarioProjectileList.emplace(direction, type, FIREBALL_BEHAVIOUR, sf::FloatRect({0.f, 0.f}, {15.f, 16.f}), sf::Vector2f(x, y), sf::Vector2f(7.f, 16.f));
            it->setTexture("Fireball", direction);
            break;
        default: ;
    }
}
//Behaviour
static void FireballYUpdate(const plf::colony<MFCPP::MarioProjectile>::colony_iterator<false>& it, const float deltaTime) {
    const auto [Xvel, Yvel, X, Y, remove] = FireballY(it->getCurrentPosition(), it->getXVelo(), it->getYVelo(), deltaTime, it->getHitbox(), it->getOrigin());
    if (remove) {
        DeleteMarioProjectile(it);
        return;
    }
    it->setCurrentPosition(sf::Vector2f(X, Y));
    it->setXVelo(Xvel);
    it->setYVelo(Yvel);
}
static void FireballXUpdate(const plf::colony<MFCPP::MarioProjectile>::colony_iterator<false>& it, const float deltaTime) {
    const auto [Xvel, Yvel, X, Y, remove] = FireballX(it->getCurrentPosition(), it->getXVelo(), it->getYVelo(), it->getDirection(), deltaTime, it->getHitbox(), it->getOrigin());
    if (remove) {
        DeleteMarioProjectile(it);
        return;
    }
    it->setCurrentPosition(sf::Vector2f(X, Y));
    it->setXVelo(Xvel);
    it->setYVelo(Yvel);
}

void MarioProjectileMovementUpdate(const float deltaTime) {
    for (auto it = MarioProjectileList.begin(); it != MarioProjectileList.end(); ++it) {
        if (it->isDestroyed()) return;

        switch (it->getBehaviour()) {
            case FIREBALL_BEHAVIOUR:
                FireballXUpdate(it, deltaTime);
                FireballYUpdate(it, deltaTime);
                break;
            default: ;
        }
    }
}
void MarioProjectileDraw() {
    for (auto& i : MarioProjectileList) {
        if (i.isDestroyed()) return;

        i.AnimationUpdate(i.getInterpolatedPosition() - sf::Vector2f(0.f, 7.f), i.getOrigin() - sf::Vector2f(0.f, 9.f));
        i.setRotation(i.getInterpolatedAngle());
        i.AnimationDraw();
    }
}
void MarioProjectileCleanup() {
    if (!MarioProjectileDeleteGate) return;
    auto it = MarioProjectileList.begin();
    while (it != MarioProjectileList.end()) {
        if (!it->isDestroyed()) ++it;
        else it = MarioProjectileList.erase(it);
    }
    MarioProjectileDeleteGate = false;
}