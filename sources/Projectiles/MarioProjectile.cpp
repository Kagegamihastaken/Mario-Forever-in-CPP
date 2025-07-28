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
    if (!it->isDestroyed() && !out) AddFireballExplosion(it->getCurrentPosition().x, it->getCurrentPosition().y);
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
        const sf::FloatRect playerHitbox = getGlobalHitbox(it->getHitbox(), it->getCurrentPosition(), it->getOrigin());
        //GoombaAI
        if (!it->isDestroyed()) {
            for (auto jt = GoombaAIList.begin(); jt != GoombaAIList.end(); ++jt) {
                if (sf::FloatRect loopHitbox = getGlobalHitbox(jt->GetHitboxMain(), jt->getCurrentPosition(), jt->getOrigin()); isCollide(loopHitbox, playerHitbox)) {
                    if (jt->IsCanDeath()) {
                        DeleteMarioProjectile(it);
                        jt->DeathBehaviour(SCORE_100);
                        DeleteGoombaAIIndex(jt);
                        break;
                    }
                }
            }
        }
        //BroAI
        if (!it->isDestroyed()) {
            for (auto jt = BroAIList.begin(); jt != BroAIList.end(); ++jt) {
                if (sf::FloatRect loopHitbox = getGlobalHitbox(jt->getHitbox(), jt->getCurrentPosition(), jt->getOrigin()); isCollide(loopHitbox, playerHitbox)) {
                    DeleteMarioProjectile(it);
                    jt->DeathBehaviour(SCORE_200);
                    SoundManager::PlaySound("Kick2");
                    DeleteBroAIIndex(jt);
                    break;
                }
            }
        }
        //Piranha
        if (!it->isDestroyed()) {
            for (int j = 0; j < PiranhaAIList.size(); ++j) {
                if (sf::FloatRect loopHitbox = getGlobalHitbox(PiranhaAIList[j].getHitbox(), PiranhaAIList[j].getCurrentPosition(), PiranhaAIList[j].getOrigin()); isCollide(loopHitbox, playerHitbox)) {
                    DeleteMarioProjectile(it);
                    AddScoreEffect(SCORE_100, PiranhaAIList[j].getCurrentPosition().x, PiranhaAIList[j].getCurrentPosition().y - PiranhaAIList[j].getOrigin().y);
                    SoundManager::PlaySound("Kick2");
                    DeletePiranhaAIIndex(j);
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
            it = MarioProjectileList.emplace(direction, type, FIREBALL_BEHAVIOUR, sf::FloatRect({0.f, 0.f}, {15.f, 16.f}), sf::Vector2f(x, y), sf::Vector2f(7.f, 8.f));
            it->setTexture("Fireball", direction);
            break;
        default: ;
    }
}
//Behaviour
static void FireballYUpdate(const plf::colony<MFCPP::MarioProjectile>::colony_iterator<false>& it, const float deltaTime) {

    it->move(sf::Vector2f(0.f, it->getYVelo() * deltaTime));
    it->setYVelo(it->getYVelo() + (it->getYVelo() >= 10.0f ? 0.0f : 1.f * deltaTime * 0.3f));
    if (it->getYVelo() > 10.0f) it->setYVelo(10.0f);

    if (float PlatPosY; PlatformYCollision(MFCPP::CollisionObject(it->getCurrentPosition(), it->getOrigin(), it->getHitbox()), PlatPosY, it->getYVelo(), false)) {
        it->setCurrentPosition(sf::Vector2f(it->getCurrentPosition().x, PlatPosY));
        it->setYVelo(-5.f);
    }

    float CurrPosYCollide, CurrPosXCollide;
    //recolide
    if (QuickCheckBotCollision(MFCPP::CollisionObject(it->getCurrentPosition(), it->getOrigin(), it->getHitbox()), CurrPosXCollide, CurrPosYCollide)) {
        if (const float offset = it->getXVelo() + 1.f; it->getYVelo() >= -it->getXVelo()) {
            const float floorY = GetCurrFloorY(it->getCurrentPosition(), CurrPosXCollide, CurrPosYCollide);
            if (it->getCurrentPosition().y < CurrPosYCollide + floorY - offset) return;
            it->setCurrentPosition(sf::Vector2f(it->getCurrentPosition().x, CurrPosYCollide + floorY - (it->getHitbox().size.y - it->getOrigin().y)));
            it->setYVelo(-5.f);
            return;
        }
    }
    if (QuickCheckTopCollision(MFCPP::CollisionObject(it->getCurrentPosition(), it->getOrigin(), it->getHitbox()), CurrPosXCollide, CurrPosYCollide))
        DeleteMarioProjectile(it);
}
static void FireballXUpdate(const plf::colony<MFCPP::MarioProjectile>::colony_iterator<false>& it, const float deltaTime) {
    if (it->getDirection()) it->move(sf::Vector2f( - it->getXVelo() * deltaTime, 0.f));
    else it->move(sf::Vector2f( it->getXVelo() * deltaTime, 0.f));

    if (float PlatDistance; PlatformXCollision(MFCPP::CollisionObject(it->getCurrentPosition(), it->getOrigin(), it->getHitbox()), PlatDistance, it->getYVelo()))
        it->move(sf::Vector2f(PlatDistance, 0.f));

    float CurrPosXCollide = 0, CurrPosYCollide = 0;
    const auto [fst, snd] = QuickCheckSideCollision(
        MFCPP::CollisionObject(it->getCurrentPosition(), it->getOrigin(), it->getHitbox()), MarioDirection,
        CurrPosXCollide, CurrPosYCollide);
    if (fst || snd)
        DeleteMarioProjectile(it);
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

        i.AnimationUpdate(i.getInterpolatedPosition(), i.getOrigin());
        i.setRotation(i.getInterpolatedAngle());
        i.AnimationDraw(window);
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