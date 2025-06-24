#include "Block/Brick.hpp"
#include "Block/LuckyBlock.hpp"
#include "Class/MarioProjectileClass.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Interpolation.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Scroll.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Loading/enum.hpp"
#include "Effect/MarioEffect.hpp"
//#include "Object/Mario.hpp"
#include <set>

#include "Core/SoundManager.hpp"
#include "Effect/FireballExplosion.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Object/BroAI.hpp"
#include "Object/GoombaAI.hpp"

std::vector<MFCPP::MarioProjectile> MarioProjectileList;

void MarioProjectileInit() {
    ImageManager::AddImage("FireballImage", "data/resources/Projectiles/Fireball.png");
    ImageManager::AddTexture("FireballImage", "FireballRight");
    ImageManager::AddTexture("FireballImage", "FireballLeft", false, true);
}
int getAmountProjectile() {
    return MarioProjectileList.size();
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
    std::set<std::pair<GoombaAIType, std::pair<float, float>>> GoombaAIDeleteSet;
    std::set<std::pair<float, float>> BroAIDeleteSet;
    if (MarioProjectileList.empty()) return;
    for (int i = 0; i < MarioProjectileList.size(); ++i) {
        const sf::FloatRect playerHitbox = getGlobalHitbox(MarioProjectileList[i].getHitbox(), MarioProjectileList[i].getCurrentPosition(), MarioProjectileList[i].getOrigin());
        //GoombaAI
        if (!MarioProjectileList[i].willBeDestroyed()) {
            for (const auto &j : GoombaAIList) {
                if (sf::FloatRect loopHitbox = getGlobalHitbox(j.GetHitboxMain(), j.getCurrentPosition(), j.getOrigin()); isCollide(loopHitbox, playerHitbox)) {
                    DeleteMarioProjectile(i);
                    j.DeathBehaviour(SCORE_100);
                    if (j.IsCanDeath()) GoombaAIDeleteSet.insert({j.GetType(), {j.getCurrentPosition().x, j.getCurrentPosition().y}});
                    break;
                }
            }
        }
        //BroAI
        if (!MarioProjectileList[i].willBeDestroyed()) {
            for (const auto& j : BroAIList) {
                if (sf::FloatRect loopHitbox = getGlobalHitbox(j.getHitbox(), j.getCurrentPosition(), j.getOrigin()); isCollide(loopHitbox, playerHitbox)) {
                    DeleteMarioProjectile(i);
                    j.DeathBehaviour(SCORE_200);
                    SoundManager::PlaySound("Kick2");
                    BroAIDeleteSet.insert({j.getCurrentPosition().x, j.getCurrentPosition().y});
                    break;
                }
            }
        }
    }
    if (!GoombaAIDeleteSet.empty())
        for (const auto &[fst, snd] : GoombaAIDeleteSet)
            DeleteGoombaAI(fst, snd.first, snd.second);
    if (!BroAIDeleteSet.empty())
        for (const auto &[fst, snd] : BroAIDeleteSet)
            DeleteBroAI(fst, snd);
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
            if (direction) MarioProjectileList.back().setTexture("FireballLeft");
            else MarioProjectileList.back().setTexture("FireballRight");
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
    bool NoAdd = false;
    int be = find_min_iny(MarioProjectileList[i].getCurrentPosition(), ObstaclesVertPosList);
    int nd = find_max_iny_dist(MarioProjectileList[i].getCurrentPosition(), ObstaclesVertPosList, 64.0f + (MarioProjectileList[i].getYVelo()) * 16.0f);
    bool ObstacleCollide = isAccurateCollideBot(MFCPP::CollisionObject(MarioProjectileList[i].getCurrentPosition(), MarioProjectileList[i].getOrigin(), MarioProjectileList[i].getHitbox()), ObstaclesVertPosList, CurrPosYCollide, NoAdd, be,
                                                    nd, 80.0f);
    be = find_min_iny(MarioProjectileList[i].getCurrentPosition(), BricksVertPosList);
    nd = find_max_iny_dist(MarioProjectileList[i].getCurrentPosition(), BricksVertPosList, 64.0f + (MarioProjectileList[i].getYVelo()) * 16.0f);
    bool BrickCollide = isAccurateCollideBot(MFCPP::CollisionObject(MarioProjectileList[i].getCurrentPosition(), MarioProjectileList[i].getOrigin(), MarioProjectileList[i].getHitbox()), BricksVertPosList, CurrPosYCollide, NoAdd, be, nd,
                                            80.0f);
    be = find_min_iny(MarioProjectileList[i].getCurrentPosition(), LuckyVertPosList);
    nd = find_max_iny_dist(MarioProjectileList[i].getCurrentPosition(), LuckyVertPosList, 64.0f + (MarioProjectileList[i].getYVelo()) * 16.0f);
    bool LuckyCollide = isAccurateCollideBot(MFCPP::CollisionObject(MarioProjectileList[i].getCurrentPosition(), MarioProjectileList[i].getOrigin(), MarioProjectileList[i].getHitbox()), LuckyVertPosList, CurrPosYCollide, NoAdd, be, nd,
                                            80.0f);
    //recolide
    if (ObstacleCollide || BrickCollide || LuckyCollide) {
        MarioProjectileList[i].setCurrentPosition(sf::Vector2f(MarioProjectileList[i].getCurrentPosition().x, CurrPosYCollide - (MarioProjectileList[i].getHitbox().size.y - MarioProjectileList[i].getOrigin().y)));
        MarioProjectileList[i].setYVelo(-5.f);
    }
    // top update
    // NoAdd = false;
    be = find_max_iny(MarioProjectileList[i].getCurrentPosition(), ObstaclesVertPosList);
    nd = find_min_iny_dist(MarioProjectileList[i].getCurrentPosition(), ObstaclesVertPosList, 64.0f - (MarioProjectileList[i].getYVelo()) * 16.0f);
    ObstacleCollide = isAccurateCollideTop(MFCPP::CollisionObject(MarioProjectileList[i].getCurrentPosition(), MarioProjectileList[i].getOrigin(), MarioProjectileList[i].getHitbox()), ObstaclesVertPosList, CurrPosYCollide, NoAdd, nd, be, 80.0f);
    const int br_be = find_max_iny(MarioProjectileList[i].getCurrentPosition(), BricksVertPosList);
    const int br_nd = find_min_iny_dist(MarioProjectileList[i].getCurrentPosition(), BricksVertPosList, 64.0f - (MarioProjectileList[i].getYVelo()) * 16.0f);
    BrickCollide = isAccurateCollideTop(MFCPP::CollisionObject(MarioProjectileList[i].getCurrentPosition(), MarioProjectileList[i].getOrigin(), MarioProjectileList[i].getHitbox()), BricksVertPosList, CurrPosYCollide, NoAdd, br_nd, br_be, 80.0f);
    const int lu_be = find_max_iny(MarioProjectileList[i].getCurrentPosition(), LuckyVertPosList);
    const int lu_nd = find_min_iny_dist(MarioProjectileList[i].getCurrentPosition(), LuckyVertPosList, 64.0f - (MarioProjectileList[i].getYVelo()) * 16.0f);
    LuckyCollide = isAccurateCollideTop(MFCPP::CollisionObject(MarioProjectileList[i].getCurrentPosition(), MarioProjectileList[i].getOrigin(), MarioProjectileList[i].getHitbox()), LuckyVertPosList, CurrPosYCollide, NoAdd, lu_nd, lu_be, 80.0f);
    if (ObstacleCollide || BrickCollide || LuckyCollide)
        DeleteMarioProjectile(i);
}
static void FireballXUpdate(const int i, const float deltaTime) {

    if (MarioProjectileList[i].getDirection()) MarioProjectileList[i].move(sf::Vector2f( - MarioProjectileList[i].getXVelo() * deltaTime, 0.f));
    else MarioProjectileList[i].move(sf::Vector2f( MarioProjectileList[i].getXVelo() * deltaTime, 0.f));

    int be, nd;
    std::pair<bool, bool> ObstacleCollide, BrickCollide, LuckyCollide;
    float CurrPosXCollide = 0, CurrPosYCollide = 0;
    bool NoAdd = false;
    if (!MarioDirection) {
        be = find_min_inx(MarioProjectileList[i].getCurrentPosition(), ObstaclesHorzPosList);
        nd = find_max_inx_dist(MarioProjectileList[i].getCurrentPosition(), ObstaclesHorzPosList, 64.0f + (MarioProjectileList[i].getXVelo()) * 4.0f);
        ObstacleCollide = isAccurateCollideSide(MFCPP::CollisionObject(MarioProjectileList[i].getCurrentPosition(), MarioProjectileList[i].getOrigin(), MarioProjectileList[i].getHitbox()), ObstaclesHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, be, nd, 80.0f);
        be = find_min_inx(MarioProjectileList[i].getCurrentPosition(), BricksHorzPosList);
        nd = find_max_inx_dist(MarioProjectileList[i].getCurrentPosition(), BricksHorzPosList, 64.0f + (MarioProjectileList[i].getXVelo()) * 4.0f);
        BrickCollide = isAccurateCollideSide(MFCPP::CollisionObject(MarioProjectileList[i].getCurrentPosition(), MarioProjectileList[i].getOrigin(), MarioProjectileList[i].getHitbox()), BricksHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, be, nd, 80.0f);
        be = find_min_inx(MarioProjectileList[i].getCurrentPosition(), LuckyHorzPosList);
        nd = find_max_inx_dist(MarioProjectileList[i].getCurrentPosition(), LuckyHorzPosList, 64.0f + (MarioProjectileList[i].getXVelo()) * 4.0f);
        LuckyCollide = isAccurateCollideSide(MFCPP::CollisionObject(MarioProjectileList[i].getCurrentPosition(), MarioProjectileList[i].getOrigin(), MarioProjectileList[i].getHitbox()), LuckyHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, be, nd, 80.0f);
    }
    else {
        be = find_max_inx(MarioProjectileList[i].getCurrentPosition(), ObstaclesHorzPosList);
        nd = find_min_inx_dist(MarioProjectileList[i].getCurrentPosition(), ObstaclesHorzPosList, 64.0f + (MarioProjectileList[i].getXVelo()) * 4.0f);
        ObstacleCollide = isAccurateCollideSide(MFCPP::CollisionObject(MarioProjectileList[i].getCurrentPosition(), MarioProjectileList[i].getOrigin(), MarioProjectileList[i].getHitbox()), ObstaclesHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, nd, be, 80.0f);
        be = find_max_inx(MarioProjectileList[i].getCurrentPosition(), BricksHorzPosList);
        nd = find_min_inx_dist(MarioProjectileList[i].getCurrentPosition(), BricksHorzPosList, 64.0f + (MarioProjectileList[i].getXVelo()) * 4.0f);
        BrickCollide = isAccurateCollideSide(MFCPP::CollisionObject(MarioProjectileList[i].getCurrentPosition(), MarioProjectileList[i].getOrigin(), MarioProjectileList[i].getHitbox()), BricksHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, nd, be, 80.0f);
        be = find_max_inx(MarioProjectileList[i].getCurrentPosition(), LuckyHorzPosList);
        nd = find_min_inx_dist(MarioProjectileList[i].getCurrentPosition(), LuckyHorzPosList, 64.0f + (MarioProjectileList[i].getXVelo()) * 4.0f);
        LuckyCollide = isAccurateCollideSide(MFCPP::CollisionObject(MarioProjectileList[i].getCurrentPosition(), MarioProjectileList[i].getOrigin(), MarioProjectileList[i].getHitbox()), LuckyHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, nd, be, 80.0f);
    }
    //snap back
    if (ObstacleCollide.first || BrickCollide.first || LuckyCollide.first)
        DeleteMarioProjectile(i);
    else if (ObstacleCollide.second || BrickCollide.second || LuckyCollide.second)
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
    int i = 0;
    while (i < MarioProjectileList.size()) {
        if (!MarioProjectileList[i].willBeDestroyed()) ++i;
        else MarioProjectileList.erase(MarioProjectileList.begin() + i);
    }
}