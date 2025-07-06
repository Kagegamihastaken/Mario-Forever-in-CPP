#include "Core/Class/MarioProjectileClass.hpp"
#include "Core/ImageManager.hpp"
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

std::vector<MFCPP::MarioProjectile> MarioProjectileList;
static bool MarioProjectileDeleteGate = false;

void MarioProjectileInit() {
    ImageManager::AddTexture("Fireball", "data/resources/Projectiles/Fireball.png");
}
int getAmountProjectile() {
    return static_cast<int>(MarioProjectileList.size());
}
void SetPrevMarioProjectilePos() {
    for (auto & i : MarioProjectileList) {
        if (i.willBeDestroyed()) continue;

        i.setPreviousPosition(i.getCurrentPosition());
        i.setPreviousAngle(i.getCurrentAngle());
    }
}
void InterpolateMarioProjectilePos(const float alpha) {
    for (auto & i : MarioProjectileList) {
        if (i.willBeDestroyed()) continue;

        i.setInterpolatedPosition(linearInterpolation(i.getPreviousPosition(), i.getCurrentPosition(), alpha));
        i.setInterpolatedAngle(linearInterpolation(i.getPreviousAngle(), i.getCurrentAngle(), alpha));
    }
}
void DeleteMarioProjectile(const int i, const bool out = false) {
    if (!MarioProjectileList[i].willBeDestroyed() && !out) AddFireballExplosion(MarioProjectileList[i].getCurrentPosition().x, MarioProjectileList[i].getCurrentPosition().y);
    MarioProjectileList[i].willDestroy(true);
    MarioProjectileDeleteGate = true;
}
void DeleteMarioProjectile(const float x, const float y) {
    for (int i = 0; i < MarioProjectileList.size(); ++i) {
        if (MarioProjectileList[i].getCurrentPosition().x == x && MarioProjectileList[i].getCurrentPosition().y == y) {
            DeleteMarioProjectile(i);
        }
    }
}
void DeleteAllMarioProjectile() {
    MarioProjectileList.clear();
}
void MarioProjectileCollision() {
    if (MarioProjectileList.empty()) return;
    for (int i = 0; i < MarioProjectileList.size(); ++i) {
        const sf::FloatRect playerHitbox = getGlobalHitbox(MarioProjectileList[i].getHitbox(), MarioProjectileList[i].getCurrentPosition(), MarioProjectileList[i].getOrigin());
        //GoombaAI
        if (!MarioProjectileList[i].willBeDestroyed()) {
            for (auto jt = GoombaAIList.begin(); jt != GoombaAIList.end(); ++jt) {
                if (sf::FloatRect loopHitbox = getGlobalHitbox(jt->GetHitboxMain(), jt->getCurrentPosition(), jt->getOrigin()); isCollide(loopHitbox, playerHitbox)) {
                    if (jt->IsCanDeath()) {
                        DeleteMarioProjectile(i);
                        jt->DeathBehaviour(SCORE_100);
                        DeleteGoombaAIIndex(jt);
                        break;
                    }
                }
            }
        }
        //BroAI
        if (!MarioProjectileList[i].willBeDestroyed()) {
            for (int j = 0; j < BroAIList.size(); ++j) {
                if (sf::FloatRect loopHitbox = getGlobalHitbox(BroAIList[j].getHitbox(), BroAIList[j].getCurrentPosition(), BroAIList[j].getOrigin()); isCollide(loopHitbox, playerHitbox)) {
                    DeleteMarioProjectile(i);
                    BroAIList[j].DeathBehaviour(SCORE_200);
                    SoundManager::PlaySound("Kick2");
                    DeleteBroAIIndex(j);
                    break;
                }
            }
        }
        //Piranha
        if (!MarioProjectileList[i].willBeDestroyed()) {
            for (int j = 0; j < PiranhaAIList.size(); ++j) {
                if (sf::FloatRect loopHitbox = getGlobalHitbox(PiranhaAIList[j].getHitbox(), PiranhaAIList[j].getCurrentPosition(), PiranhaAIList[j].getOrigin()); isCollide(loopHitbox, playerHitbox)) {
                    DeleteMarioProjectile(i);
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
    for (int i = 0; i < MarioProjectileList.size(); ++i) {
        if (isOutScreen(MarioProjectileList[i].getInterpolatedPosition().x, MarioProjectileList[i].getInterpolatedPosition().y, 32.f, 32.f)) {
            DeleteMarioProjectile(i, true);
            //DeleteMarioProjectile(MarioProjectileList[i].getCurrentPosition().x, MarioProjectileList[i].getCurrentPosition().y);
        }
    }
}
void MarioProjectileSpin(const float deltaTime) {
    for (int i = 0; i < MarioProjectileList.size(); ++i) {
        if (MarioProjectileList[i].willBeDestroyed()) return;

        if (MarioProjectileList[i].getDirection()) MarioProjectileList[i].setCurrentAngle(MarioProjectileList[i].getCurrentAngle() - sf::degrees(11.5f * deltaTime));
        else MarioProjectileList[i].setCurrentAngle(MarioProjectileList[i].getCurrentAngle() + sf::degrees(11.5f * deltaTime));
    }
}
void LevelEndMarioProjectileCleanup() {
    for (int i = 0; i < MarioProjectileList.size(); ++i) {
        if (MarioProjectileList[i].willBeDestroyed()) continue;

        AddScoreEffect(SCORE_100, MarioProjectileList[i].getCurrentPosition().x, MarioProjectileList[i].getCurrentPosition().y - MarioProjectileList[i].getOrigin().y);
        MarioProjectileList[i].willDestroy(true);
    }
}
void AddMarioProjectile(const bool direction, const MarioProjectileType type, const float x, const float y) {
    switch (type) {
        case MarioProjectileType::FIREBALL:
            MarioProjectileList.emplace_back(direction, type, FIREBALL_BEHAVIOUR, sf::FloatRect({0.f, 0.f}, {15.f, 16.f}), sf::Vector2f(x, y), sf::Vector2f(7.f, 8.f));
            MarioProjectileList.back().setTexture("Fireball", direction);
            break;
        default: ;
    }
}
//Behaviour
static void FireballYUpdate(const int i, const float deltaTime) {

    MarioProjectileList[i].move(sf::Vector2f(0.f, MarioProjectileList[i].getYVelo() * deltaTime));
    MarioProjectileList[i].setYVelo(MarioProjectileList[i].getYVelo() + (MarioProjectileList[i].getYVelo() >= 10.0f ? 0.0f : 1.f * deltaTime * 0.3f));
    if (MarioProjectileList[i].getYVelo() > 10.0f) MarioProjectileList[i].setYVelo(10.0f);

    float CurrPosYCollide;
    //recolide
    if (QuickCheckBotCollision(MFCPP::CollisionObject(MarioProjectileList[i].getCurrentPosition(), MarioProjectileList[i].getOrigin(), MarioProjectileList[i].getHitbox()), CurrPosYCollide)) {
        MarioProjectileList[i].setCurrentPosition(sf::Vector2f(MarioProjectileList[i].getCurrentPosition().x, CurrPosYCollide - (MarioProjectileList[i].getHitbox().size.y - MarioProjectileList[i].getOrigin().y)));
        MarioProjectileList[i].setYVelo(-5.f);
    }
    if (QuickCheckTopCollision(MFCPP::CollisionObject(MarioProjectileList[i].getCurrentPosition(), MarioProjectileList[i].getOrigin(), MarioProjectileList[i].getHitbox()), CurrPosYCollide))
        DeleteMarioProjectile(i);
}
static void FireballXUpdate(const int i, const float deltaTime) {
    if (MarioProjectileList[i].getDirection()) MarioProjectileList[i].move(sf::Vector2f( - MarioProjectileList[i].getXVelo() * deltaTime, 0.f));
    else MarioProjectileList[i].move(sf::Vector2f( MarioProjectileList[i].getXVelo() * deltaTime, 0.f));

    float CurrPosXCollide = 0, CurrPosYCollide = 0;
    const auto [fst, snd] = QuickCheckSideCollision(
        MFCPP::CollisionObject(MarioProjectileList[i].getCurrentPosition(), MarioProjectileList[i].getOrigin(),
                               MarioProjectileList[i].getHitbox()), MarioDirection,
        CurrPosXCollide, CurrPosYCollide);
    if (fst || snd)
        DeleteMarioProjectile(i);
}

void MarioProjectileMovementUpdate(const float deltaTime) {
    for (int i = 0; i < MarioProjectileList.size(); ++i) {
        if (MarioProjectileList[i].willBeDestroyed()) return;

        switch (MarioProjectileList[i].getBehaviour()) {
            case FIREBALL_BEHAVIOUR:
                FireballXUpdate(i, deltaTime);
                FireballYUpdate(i, deltaTime);
                break;
            default: ;
        }
    }
}
void MarioProjectileDraw() {
    for (auto& i : MarioProjectileList) {
        if (i.willBeDestroyed()) return;

        i.AnimationUpdate(i.getInterpolatedPosition(), i.getOrigin());
        i.setRotation(i.getInterpolatedAngle());
        i.AnimationDraw(window);
    }
}
void MarioProjectileCleanup() {
    if (!MarioProjectileDeleteGate) return;
    int i = 0;
    while (i < MarioProjectileList.size()) {
        if (!MarioProjectileList[i].willBeDestroyed()) ++i;
        else MarioProjectileList.erase(MarioProjectileList.begin() + i);
    }
    MarioProjectileDeleteGate = false;
}