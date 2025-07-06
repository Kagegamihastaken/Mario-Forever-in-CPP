#include "Object/BroAI.hpp"
#include "Core/Class/BroAIClass.hpp"
#include "Core/ImageManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Collision/Collide.hpp"
#include "Effect/BroAIEffect.hpp"
#include "Effect/MarioEffect.hpp"
#include "Object/Mario.hpp"
#include "Projectiles/BroAIProjectile.hpp"

std::vector<MFCPP::BroAI> BroAIList;
static bool BroAIDeleteGate = false;

static std::vector<std::string> HammerBroAnimName;
static std::vector<std::string> HammerBroLaunchAnimName;
static constexpr int HAMMER_BRO_IMAGE_WIDTH = 96;
static constexpr int HAMMER_BRO_WIDTH = 48;
static constexpr int HAMMER_BRO_HEIGHT = 64;

void BroAILoadRes() {
    for (int i = 0; i < HAMMER_BRO_IMAGE_WIDTH / HAMMER_BRO_WIDTH; i++) {
        ImageManager::AddTexture(fmt::format("HammerBro_{}", i), "data/resources/HammerBro/HammerBro.png", sf::IntRect({i * HAMMER_BRO_WIDTH, 0}, {HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}));
        HammerBroAnimName.push_back(fmt::format("HammerBro_{}", i));

        ImageManager::AddTexture(fmt::format("HammerBroLaunch_{}", i), "data/resources/HammerBro/HammerBro.png", sf::IntRect({i * HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}, {HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}));
        HammerBroLaunchAnimName.push_back(fmt::format("HammerBroLaunch_{}", i));
    }
}
static void BroLaunchProjectile(const int i) {
    switch (BroAIList[i].getType()) {
        case BroAIType::HAMMER_BRO:
            if (BroAIList[i].getAnimationDirection() == AnimationDirection::ANIM_RIGHT)
                AddBroAIProjectile(static_cast<bool>(BroAIList[i].getAnimationDirection()), HAMMER, BroAIList[i].getCurrentPosition().x + 5.f, BroAIList[i].getCurrentPosition().y - 31.f);
            else
                AddBroAIProjectile(static_cast<bool>(BroAIList[i].getAnimationDirection()), HAMMER, BroAIList[i].getCurrentPosition().x - 5.f, BroAIList[i].getCurrentPosition().y - 31.f);
            SoundManager::PlaySound("Hammer");
            break;
        default: ;
    }
}
void DeleteBroAIIndex(const int i) {
    BroAIDeleteGate = true;
    BroAIList[i].willDestroy(true);
}
void DeleteBroAI(const float x, const float y) {
    for (int i = 0; i < BroAIList.size(); ++i) {
        if (BroAIList[i].getCurrentPosition().x == x && BroAIList[i].getCurrentPosition().y == y)
            DeleteBroAIIndex(i);
            //BroAIList.erase(BroAIList.begin() + i);
    }
}
void DeleteAllBroAI() {
    BroAIList.clear();
}
void SetPrevBroAIPos() {
    for (auto & i : BroAIList) {
        if (i.willBeDestroyed()) continue;

        i.setPreviousPosition(i.getCurrentPosition());
    }
}
void InterpolateBroAIPos(const float alpha) {
    for (auto & i : BroAIList) {
        if (i.willBeDestroyed()) continue;

        i.setInterpolatedPosition(linearInterpolation(i.getPreviousPosition(), i.getCurrentPosition(), alpha));
    }
}
void AddBroAI(const BroAIType type, const BroAIMovementType movementType, const float x, const float y) {
    switch (type) {
        case BroAIType::HAMMER_BRO:
            BroAIList.emplace_back(type, movementType, 2.f, 100.f, 3.f, 1, 0.f, sf::FloatRect({7.f, 16.f}, {32.f, 48.f}),
                               sf::Vector2f(x, y), sf::Vector2f(24.f, 64.f));
            BroAIList.back().setAnimation(0, 1, 14, true);
            BroAIList.back().setAnimationSequence(HammerBroAnimName);
            break;
        default: ;
    }
}
void BroAIStatusUpdate() {
    for (int i = 0; i < BroAIList.size(); ++i) {
        if (BroAIList[i].willBeDestroyed()) continue;

        if (isOutScreenYBottom(BroAIList[i].getCurrentPosition().y, 80.f)) {
            DeleteBroAIIndex(i);
            //BroAIDeletionPositionList.push_back(BroAIList[i].getCurrentPosition());
        }
        if (!isOutScreen(BroAIList[i].getCurrentPosition().x, BroAIList[i].getCurrentPosition().y, 64.0f, 64.0f))
            if (BroAIList[i].isDisabled()) BroAIList[i].setDisabled(false);
        if (!EffectActive) {
            if (BroAIList[i].getCurrentPosition().x > player.curr.x) BroAIList[i].setAnimationDirection(ANIM_LEFT);
            else if (BroAIList[i].getCurrentPosition().x < player.curr.x) BroAIList[i].setAnimationDirection(ANIM_RIGHT);
        }
        else BroAIList[i].setAnimationDirection(ANIM_LEFT);
    }
}
void BroAICheckCollide() {
    if (EffectActive) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(player.hitboxMain, player.curr, player.property.getOrigin());
    for (int i = 0; i < BroAIList.size(); ++i) {
        if (BroAIList[i].willBeDestroyed() || BroAIList[i].isDisabled() ||
            f_abs(player.curr.x - BroAIList[i].getCurrentPosition().x) >= 160.f) continue;
        if (const sf::FloatRect loopHitbox = getGlobalHitbox(BroAIList[i].getHitbox(), BroAIList[i].getCurrentPosition(), BroAIList[i].getOrigin()); isCollide(loopHitbox, hitbox_mario)) {
            if ((BroAIList[i].getCurrentPosition().y - 16.f > player.curr.y) && Yvelo > 0.f) {
                //player.curr = {player.curr.x, BroAIList[i].getCurrentPosition().y - BroAIList[i].getOrigin().y - 1.f};
                if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) Yvelo = -8.0f;
                else Yvelo = -13.0f;
                SoundManager::PlaySound("Stomp");
                BroAIList[i].DeathBehaviour(SCORE_200);
                AddBroAIEffect(BroAIList[i].getType(), static_cast<bool>(BroAIList[i].getAnimationDirection()), BroAIList[i].getCurrentPosition().x, BroAIList[i].getCurrentPosition().y);
                DeleteBroAIIndex(i);
                break;
            }
            else if (BroAIList[i].getCurrentPosition().y - 16.0f < player.curr.y) {
                PowerDown();
                break;
            }
        }
    }
}
void BroAIShootUpdate(const float deltaTime) {
    for (int i = 0; i < BroAIList.size(); ++i) {
        if (BroAIList[i].willBeDestroyed() || BroAIList[i].isDisabled()) continue;
        if (BroAIList[i].getLaunchTickingTime() == 0.f) {
            BroAIList[i].setLaunchIntervalTicking(BroAIList[i].getLaunchIntervalTicking() + 1.f * deltaTime);
            if (BroAIList[i].getLaunchIntervalTicking() >= BroAIList[i].getLaunchInterval()) {
                if (RandomIntNumberGenerator(0, static_cast<int>(BroAIList[i].getLaunchRNG())) == 1 && !isOutScreen(
                        BroAIList[i].getCurrentPosition().x, BroAIList[i].getCurrentPosition().y, 32.f, 32.f)) {
                    BroAIList[i].setLaunchTickingTime(BroAIList[i].getLaunchTickingTime() + 1.f * deltaTime);
                    BroAIList[i].setAnimationSequence(HammerBroLaunchAnimName);
                    //std::cout << "Standby\n";
                }
                BroAIList[i].setLaunchIntervalTicking(0.f);
            }
        }
        else if (BroAIList[i].getLaunchTickingTime() > 0.f) {
            if (BroAIList[i].getLaunchTickingTime() <= BroAIList[i].getLaunchWaitTime())
                BroAIList[i].setLaunchTickingTime(BroAIList[i].getLaunchTickingTime() + 1.f * deltaTime);
            else {
                if (BroAIList[i].getLaunchCounting() > 0) {
                    if (BroAIList[i].getLaunchIBTicking() < BroAIList[i].getLaunchIntervalBetween()) {
                        BroAIList[i].setLaunchIBTicking(BroAIList[i].getLaunchIBTicking() + 1.0f * deltaTime);
                    }
                    else {
                        BroLaunchProjectile(i);
                        BroAIList[i].setLaunchIBTicking(0.f);
                        BroAIList[i].setLaunchCounting(BroAIList[i].getLaunchCounting() - 1);
                    }
                }
                else {
                    BroAIList[i].setLaunchIBTicking(BroAIList[i].getLaunchIntervalBetween());
                    BroAIList[i].setLaunchCounting(BroAIList[i].getLaunchCount());
                    BroAIList[i].setLaunchTickingTime(0.f);
                    BroAIList[i].setAnimationSequence(HammerBroAnimName);
                }
                //std::cout << "Throw\n";
            }
        }

    }
}
static void SetWalkingValue(const int index, const int multiply, const bool reverse) {
    const float valtest = multiply * (BroAIList[index].getRandomWalking() + BroAIList[index].getFixedWalkingValue()) * (reverse ? -1.f : 1.f);
    if (valtest >= 0.0f) BroAIList[index].setDirectionWalking(false);
    else BroAIList[index].setDirectionWalking(true);
    BroAIList[index].setMovingValue(f_abs(valtest));
}
void BroAIVertXUpdate(const float deltaTime) {
    float CurrPosXCollide, CurrPosYCollide;
    for (int i = 0; i < BroAIList.size(); ++i) {
        if (BroAIList[i].willBeDestroyed() || BroAIList[i].isDisabled()) continue;
        // Movement
        if (BroAIList[i].getMovingValue() <= 0.f) {
            if (BroAIList[i].getMovingValue() < 0.f) BroAIList[i].setMovingValue(0.f);
            BroAIList[i].setTimeTicking(BroAIList[i].getTimeTicking() + BroAIList[i].getTickingValue() * deltaTime);
        }
        else if (BroAIList[i].getMovingValue() > 0.f) {
            if (!BroAIList[i].getDirectionWalking())
                BroAIList[i].setCurrentPosition(sf::Vector2f(BroAIList[i].getCurrentPosition().x + BroAIList[i].getSpeed() * deltaTime, BroAIList[i].getCurrentPosition().y));
            else
                BroAIList[i].setCurrentPosition(sf::Vector2f(BroAIList[i].getCurrentPosition().x - BroAIList[i].getSpeed() * deltaTime, BroAIList[i].getCurrentPosition().y));
            BroAIList[i].setMovingValue(BroAIList[i].getMovingValue() - 1.f * deltaTime);
        }
        switch (BroAIList[i].getState()) {
            case 1:
                SetWalkingValue(i, 1, true);
                BroAIList[i].setTimeTicking(0.f);
                BroAIList[i].setState(2);
                break;
            case 2:
                if (BroAIList[i].getMovingValue() <= 0.f) {
                    BroAIList[i].setMovingValue(0.f);
                    BroAIList[i].setState(3);
                }
                break;
            case 3:
                if (BroAIList[i].getTimeTicking() > BroAIList[i].getStopDuration()) {
                    BroAIList[i].setTimeTicking(0.f);
                    SetWalkingValue(i, 2, false);
                    BroAIList[i].setState(4);
                }
                break;
            case 4:
                if (BroAIList[i].getMovingValue() <= 0.f) {
                    BroAIList[i].setMovingValue(0.f);
                    BroAIList[i].setState(5);
                }
                break;
            case 5:
                if (BroAIList[i].getTimeTicking() > BroAIList[i].getStopDuration()) {
                    BroAIList[i].setTimeTicking(0.f);
                    SetWalkingValue(i, 1, true);
                    BroAIList[i].setState(6);
                }
                break;
            case 6:
                if (BroAIList[i].getMovingValue() <= 0.f) {
                    BroAIList[i].setMovingValue(0.f);
                    BroAIList[i].setState(7);
                }
                break;
            case 7:
                BroAIList[i].setTickingValue(BroAIList[i].getFixedTickingValue() + static_cast<float>(RandomIntNumberGenerator(0, static_cast<int>(BroAIList[i].getTickingRNGRange()-1.f))));
                BroAIList[i].setRandomWalking(static_cast<float>(RandomIntNumberGenerator(0, static_cast<int>(BroAIList[i].getWalkingRNGRange()-1.f))) * -1.f);
                BroAIList[i].setState(1);
                break;
            default: ;
        }
        //collision
        // Count if size AllCollideList equal to CollideAddCounter
        CurrPosXCollide = 0, CurrPosYCollide = 0;
        // 0 for right direction; 1 for left direction

        bool NoAdd = false;
        const auto [fst, snd] = QuickCheckOnlyObstaclesSideCollision(
            MFCPP::CollisionObject(BroAIList[i].getCurrentPosition(), BroAIList[i].getOrigin(),
                BroAIList[i].getWallHitbox()), BroAIList[i].getDirectionWalking(), CurrPosXCollide, CurrPosYCollide, NoAdd);

        if (fst) {
            BroAIList[i].setCurrentPosition(sf::Vector2f(CurrPosXCollide + 32.0f - BroAIList[i].getHitbox().position.x + BroAIList[i].getOrigin().x, BroAIList[i].getCurrentPosition().y));
        }
        if (snd) {
            BroAIList[i].setCurrentPosition(sf::Vector2f(CurrPosXCollide - (BroAIList[i].getHitbox().position.x + (BroAIList[i].getHitbox().size.x - BroAIList[i].getOrigin().x)), BroAIList[i].getCurrentPosition().y));
        }
    }
}
void BroAIVertYUpdate(const float deltaTime) {
    float CurrPosYCollide;
    for (auto & i : BroAIList) {
        if (i.willBeDestroyed() || i.isDisabled()) continue;
        bool willJump = false;
        bool willFall = false;
        if (i.getMovementType() == BroAIMovementType::CAN_JUMP) {
            if (i.getJumpClock().getElapsedTime().asMilliseconds() >= 200) {
                i.restartJumpClock();
                switch (RandomIntNumberGenerator(0, 19)) {
                    case 1: willFall |= true; break;
                    case 2: willJump |= true; break;
                    default: break;
                }
            }
        }
        if (i.isFalling() && i.getCurrentPosition().y > i.getLastY() + 48.f) {
            i.setLastY(-1.f);
            i.setFalling(false);
        }

        i.move(sf::Vector2f(0.0f, i.getYVelo() * deltaTime));
        i.setYVelo(i.getYVelo() + (i.getYVelo() >= 10.0f ? 0.0f : 1.f * deltaTime * 0.175f));
        //}
        bool NoAdd = false;
        if (QuickCheckOnlyObstacleBotCollision(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.getHitbox()), CurrPosYCollide, NoAdd)) {
            if (i.getYVelo() >= 0.0f) {
                i.setYVelo(0.0f);
                i.setCurrentPosition(sf::Vector2f(i.getCurrentPosition().x, CurrPosYCollide - i.getHitbox().position.y - (i.getHitbox().size.y - i.getOrigin().y)));
                if (willJump) i.setYVelo(-8.f);
            }
            else {
                i.setYVelo(0.0f);
            }
        }
        else {
            if (QuickCheckOnlyHittableBotCollision(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.getHitbox()), CurrPosYCollide, NoAdd) && i.getYVelo() > 0.f) {
                if (!i.isFalling()) {
                    if (i.getYVelo() >= 0.0f) {
                        i.setYVelo(0.0f);
                        i.setCurrentPosition(sf::Vector2f(i.getCurrentPosition().x, CurrPosYCollide - i.getHitbox().position.y - (i.getHitbox().size.y - i.getOrigin().y)));
                        if (willJump) i.setYVelo(-8.f);
                        else if (willFall) {
                            i.setFalling(true);
                            i.setLastY(i.getCurrentPosition().y);
                            i.setYVelo(-3.f);
                        }
                    } else {
                        i.setYVelo(0.0f);
                    }
                }
            }
        }

        NoAdd = false;
        if (QuickCheckOnlyObstacleTopCollision(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.getHitbox()), CurrPosYCollide, NoAdd)) {
            i.setYVelo(0.0f);
            i.setCurrentPosition(sf::Vector2f(i.getCurrentPosition().x, CurrPosYCollide - i.getHitbox().position.y + (32.0f + i.getOrigin().y)));
        }
    }
}
void BroAIDraw() {
    for (auto &i : BroAIList) {
        if (!isOutScreen(i.getCurrentPosition().x, i.getCurrentPosition().y, 64.f, 64.f)) {
            i.AnimationUpdate(i.getInterpolatedPosition(), i.getOrigin());
            i.AnimationDraw(window);
        }
    }
}
void BroAICleanup() {
    if (!BroAIDeleteGate) return;
    int i = 0;
    while (i < BroAIList.size()) {
        if (!BroAIList[i].willBeDestroyed()) ++i;
        else BroAIList.erase(BroAIList.begin() + i);
    }
    BroAIDeleteGate = false;
}