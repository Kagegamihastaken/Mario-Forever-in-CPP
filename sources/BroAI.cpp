#include "Object/BroAI.hpp"

#include <iostream>

#include "Block/Brick.hpp"
#include "Block/LuckyBlock.hpp"
#include "Class/BroAIClass.hpp"
#include "Core/ImageManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Collision/Collide.hpp"
#include "Effect/BroAIEffect.hpp"
#include "Effect/MarioEffect.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Object/Mario.hpp"
#include "Projectiles/BroAIProjectile.hpp"

std::vector<MFCPP::BroAI> BroAIList;
static std::vector<std::string> HammerBroLeftAnimName;
static std::vector<std::string> HammerBroRightAnimName;
static std::vector<std::string> HammerBroLaunchLeftAnimName;
static std::vector<std::string> HammerBroLaunchRightAnimName;
static constexpr int HAMMER_BRO_IMAGE_WIDTH = 96;
static constexpr int HAMMER_BRO_WIDTH = 48;
static constexpr int HAMMER_BRO_HEIGHT = 64;

void BroAILoadRes() {
    ImageManager::AddImage("HammerBroImage", "data/resources/HammerBro/HammerBro.png");
    for (int i = 0; i < HAMMER_BRO_IMAGE_WIDTH / HAMMER_BRO_WIDTH; i++) {
        ImageManager::AddTexture("HammerBroImage", sf::IntRect({i * HAMMER_BRO_WIDTH, 0}, {HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}), "HammerBroRight_" + std::to_string(i));
        ImageManager::AddTexture("HammerBroImage", sf::IntRect({i * HAMMER_BRO_WIDTH, 0}, {HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}), "HammerBroLeft_" + std::to_string(i), false, true);
        HammerBroLeftAnimName.push_back("HammerBroLeft_" + std::to_string(i));
        HammerBroRightAnimName.push_back("HammerBroRight_" + std::to_string(i));

        ImageManager::AddTexture("HammerBroImage", sf::IntRect({i * HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}, {HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}), "HammerBroLaunchRight_" + std::to_string(i));
        ImageManager::AddTexture("HammerBroImage", sf::IntRect({i * HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}, {HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}), "HammerBroLaunchLeft_" + std::to_string(i), false, true);
        HammerBroLaunchLeftAnimName.push_back("HammerBroLaunchLeft_" + std::to_string(i));
        HammerBroLaunchRightAnimName.push_back("HammerBroLaunchRight_" + std::to_string(i));
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
void DeleteBroAI(const float x, const float y) {
    for (int i = 0; i < BroAIList.size(); ++i) {
        if (BroAIList[i].getCurrentPosition().x == x && BroAIList[i].getCurrentPosition().y == y)
            BroAIList.erase(BroAIList.begin() + i);
    }
}
void DeleteAllBroAI() {
    BroAIList.clear();
}
void SetPrevBroAIPos() {
    for (auto & i : BroAIList) {
        i.setPreviousPosition(i.getCurrentPosition());
    }
}
void InterpolateBroAIPos(const float alpha) {
    for (auto & i : BroAIList) {
        i.setInterpolatedPosition(linearInterpolation(i.getPreviousPosition(), i.getCurrentPosition(), alpha));
    }
}
void AddBroAI(const BroAIType type, const BroAIMovementType movementType, const float x, const float y) {
    switch (type) {
        case BroAIType::HAMMER_BRO:
            BroAIList.emplace_back(type, movementType, 2.f, 100.f, 3.f, 1, 0.f, sf::FloatRect({7.f, 16.f}, {32.f, 48.f}),
                               sf::Vector2f(x, y), sf::Vector2f(24.f, 64.f));
            BroAIList.back().setAnimation(0, 1, 14);
            BroAIList.back().setAnimationSequence(HammerBroLeftAnimName, HammerBroRightAnimName);
            break;
        default: ;
    }
}
void BroAIStatusUpdate() {
    for (int i = 0; i < BroAIList.size(); ++i) {
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
        if (BroAIList[i].isDisabled()) continue;
        if (f_abs(player.curr.x - BroAIList[i].getCurrentPosition().x) >= 160.f) continue;
        if (const sf::FloatRect loopHitbox = getGlobalHitbox(BroAIList[i].getHitbox(), BroAIList[i].getCurrentPosition(), BroAIList[i].getOrigin()); isCollide(loopHitbox, hitbox_mario)) {
            if ((BroAIList[i].getCurrentPosition().y - 16.f > player.curr.y) && Yvelo > 0.f) {
                //player.curr = {player.curr.x, BroAIList[i].getCurrentPosition().y - BroAIList[i].getOrigin().y - 1.f};
                if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) Yvelo = -8.0f;
                else Yvelo = -13.0f;
                SoundManager::PlaySound("Stomp");
                BroAIList[i].DeathBehaviour(SCORE_200);
                AddBroAIEffect(BroAIList[i].getType(), static_cast<bool>(BroAIList[i].getAnimationDirection()), BroAIList[i].getCurrentPosition().x, BroAIList[i].getCurrentPosition().y);
                DeleteBroAI(BroAIList[i].getCurrentPosition().x, BroAIList[i].getCurrentPosition().y);
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
        if (BroAIList[i].isDisabled()) continue;
        if (BroAIList[i].getLaunchTickingTime() == 0.f) {
            BroAIList[i].setLaunchIntervalTicking(BroAIList[i].getLaunchIntervalTicking() + 1.f * deltaTime);
            if (BroAIList[i].getLaunchIntervalTicking() >= BroAIList[i].getLaunchInterval()) {
                if (RandomIntNumberGenerator(0, static_cast<int>(BroAIList[i].getLaunchRNG())) == 1 && !isOutScreen(
                        BroAIList[i].getCurrentPosition().x, BroAIList[i].getCurrentPosition().y, 32.f, 32.f)) {
                    BroAIList[i].setLaunchTickingTime(BroAIList[i].getLaunchTickingTime() + 1.f * deltaTime);
                    BroAIList[i].setAnimationSequence(HammerBroLaunchLeftAnimName, HammerBroLaunchRightAnimName);
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
                    BroAIList[i].setAnimationSequence(HammerBroLeftAnimName, HammerBroRightAnimName);
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
    std::pair<bool, bool> ObstacleCollide;
    int be, nd;
    for (int i = 0; i < BroAIList.size(); ++i) {
        if (BroAIList[i].isDisabled()) continue;
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
        // Loop through obstacles
        if (!BroAIList[i].getDirectionWalking()) {
            be = find_min_inx(BroAIList[i].getCurrentPosition(), ObstaclesHorzPosList);
            nd = find_max_inx_dist(BroAIList[i].getCurrentPosition(), ObstaclesHorzPosList, 64.0f + (BroAIList[i].getSpeed()) * 16.0f);
            ObstacleCollide = isAccurateCollideSide(MFCPP::CollisionObject(BroAIList[i].getCurrentPosition(), BroAIList[i].getOrigin(), BroAIList[i].getWallHitbox()), ObstaclesHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, be, nd, 80.0f);
            //be = find_min_inx(BroAIList[i].getCurrentPosition(), BricksHorzPosList);
            //nd = find_max_inx_dist(BroAIList[i].getCurrentPosition(), BricksHorzPosList, 64.0f + (BroAIList[i].getSpeed()) * 16.0f);
            //BrickCollide = isAccurateCollideSide(MFCPP::CollisionObject(BroAIList[i].getCurrentPosition(), BroAIList[i].getOrigin(), BroAIList[i].getHitbox()), BricksHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, be, nd, 80.0f);
            //be = find_min_inx(BroAIList[i].getCurrentPosition(), LuckyHorzPosList);
            //nd = find_max_inx_dist(BroAIList[i].getCurrentPosition(), LuckyHorzPosList, 64.0f + (BroAIList[i].getSpeed()) * 16.0f);
            //LuckyCollide = isAccurateCollideSide(MFCPP::CollisionObject(BroAIList[i].getCurrentPosition(), BroAIList[i].getOrigin(), BroAIList[i].getHitbox()), LuckyHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, be, nd, 80.0f);
        }
        else if (BroAIList[i].getDirectionWalking()) {
            be = find_max_inx(BroAIList[i].getCurrentPosition(), ObstaclesHorzPosList);
            nd = find_min_inx_dist(BroAIList[i].getCurrentPosition(), ObstaclesHorzPosList, 64.0f + (BroAIList[i].getSpeed()) * 16.0f);
            ObstacleCollide = isAccurateCollideSide(MFCPP::CollisionObject(BroAIList[i].getCurrentPosition(), BroAIList[i].getOrigin(), BroAIList[i].getWallHitbox()), ObstaclesHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, nd, be, 80.0f);
            //be = find_max_inx(BroAIList[i].getCurrentPosition(), BricksHorzPosList);
            //nd = find_min_inx_dist(BroAIList[i].getCurrentPosition(), BricksHorzPosList, 64.0f + (BroAIList[i].getSpeed()) * 16.0f);
            //BrickCollide = isAccurateCollideSide(MFCPP::CollisionObject(BroAIList[i].getCurrentPosition(), BroAIList[i].getOrigin(), BroAIList[i].getHitbox()), BricksHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, nd, be, 80.0f);
            //be = find_max_inx(BroAIList[i].getCurrentPosition(), LuckyHorzPosList);
            //nd = find_min_inx_dist(BroAIList[i].getCurrentPosition(), LuckyHorzPosList, 64.0f + (BroAIList[i].getSpeed()) * 16.0f);
            //LuckyCollide = isAccurateCollideSide(MFCPP::CollisionObject(BroAIList[i].getCurrentPosition(), BroAIList[i].getOrigin(), BroAIList[i].getHitbox()), LuckyHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, nd, be, 80.0f);
        }

        // Stop on wall
        // Adjust Position if collide
        if (ObstacleCollide.first) {
            BroAIList[i].setCurrentPosition(sf::Vector2f(CurrPosXCollide + 32.0f - BroAIList[i].getHitbox().position.x + BroAIList[i].getOrigin().x, BroAIList[i].getCurrentPosition().y));
        }
        if (ObstacleCollide.second) {
            BroAIList[i].setCurrentPosition(sf::Vector2f(CurrPosXCollide - (BroAIList[i].getHitbox().position.x + (BroAIList[i].getHitbox().size.x - BroAIList[i].getOrigin().x)), BroAIList[i].getCurrentPosition().y));
        }
    }
}
void BroAIVertYUpdate(const float deltaTime) {
    float CurrPosYCollide;
    for (int i = 0; i < BroAIList.size(); ++i) {
        if (BroAIList[i].isDisabled()) continue;
        bool willJump = false;
        bool willFall = false;
        if (BroAIList[i].getMovementType() == BroAIMovementType::CAN_JUMP) {
            if (BroAIList[i].getJumpClock().getElapsedTime().asMilliseconds() >= 200) {
                BroAIList[i].restartJumpClock();
                switch (RandomIntNumberGenerator(0, 19)) {
                    case 1: willFall |= true; break;
                    case 2: willJump |= true; break;
                    default: break;
                }
            }
        }
        if (BroAIList[i].isFalling() && BroAIList[i].getCurrentPosition().y > BroAIList[i].getLastY() + 48.f) {
            BroAIList[i].setLastY(-1.f);
            BroAIList[i].setFalling(false);
        }

        BroAIList[i].setYVelo(BroAIList[i].getYVelo() + (BroAIList[i].getYVelo() >= 10.0f ? 0.0f : 0.5f * deltaTime * 0.175f));
        BroAIList[i].move(sf::Vector2f(0.0f, BroAIList[i].getYVelo() * deltaTime));
        BroAIList[i].setYVelo(BroAIList[i].getYVelo() + (BroAIList[i].getYVelo() >= 10.0f ? 0.0f : 0.5f * deltaTime * 0.175f));
        //}
        bool NoAdd = false;
        int be = find_min_iny(BroAIList[i].getCurrentPosition(), ObstaclesVertPosList);
        int nd = find_max_iny_dist(BroAIList[i].getCurrentPosition(), ObstaclesVertPosList, 64.0f + (BroAIList[i].getYVelo()) * 16.0f);
        bool ObstacleCollide = isAccurateCollideBot(MFCPP::CollisionObject(BroAIList[i].getCurrentPosition(), BroAIList[i].getOrigin(), BroAIList[i].getHitbox()), ObstaclesVertPosList,
                                CurrPosYCollide, NoAdd, be, nd, 80.0f);
        if (ObstacleCollide) {
            if (BroAIList[i].getYVelo() >= 0.0f) {
                BroAIList[i].setYVelo(0.0f);
                BroAIList[i].setCurrentPosition(sf::Vector2f(BroAIList[i].getCurrentPosition().x, CurrPosYCollide - BroAIList[i].getHitbox().position.y - (BroAIList[i].getHitbox().size.y - BroAIList[i].getOrigin().y)));
                if (willJump) BroAIList[i].setYVelo(-8.f);
            }
            else {
                BroAIList[i].setYVelo(0.0f);
            }
        }
        else {
            int b_be = find_min_iny(BroAIList[i].getCurrentPosition(), BricksVertPosList);
            int b_nd = find_max_iny_dist(BroAIList[i].getCurrentPosition(), BricksVertPosList, 64.0f + (BroAIList[i].getYVelo()) * 16.0f);
            const bool BrickCollide = isAccurateCollideBot(MFCPP::CollisionObject(BroAIList[i].getCurrentPosition(), BroAIList[i].getOrigin(), BroAIList[i].getHitbox()), BricksVertPosList, CurrPosYCollide,
            NoAdd, b_be, b_nd, 80.0f);
            b_be = find_min_iny(BroAIList[i].getCurrentPosition(), LuckyVertPosList);
            b_nd = find_max_iny_dist(BroAIList[i].getCurrentPosition(), LuckyVertPosList, 64.0f + (BroAIList[i].getYVelo()) * 16.0f);
            const bool LuckyCollide = isAccurateCollideBot(MFCPP::CollisionObject(BroAIList[i].getCurrentPosition(), BroAIList[i].getOrigin(), BroAIList[i].getHitbox()), LuckyVertPosList, CurrPosYCollide,
            NoAdd, b_be, b_nd, 80.0f);
            if ((BrickCollide || LuckyCollide) && BroAIList[i].getYVelo() > 0.f) {
                if (!BroAIList[i].isFalling()) {
                    if (BroAIList[i].getYVelo() >= 0.0f) {
                        BroAIList[i].setYVelo(0.0f);
                        BroAIList[i].setCurrentPosition(sf::Vector2f(BroAIList[i].getCurrentPosition().x, CurrPosYCollide - BroAIList[i].getHitbox().position.y - (BroAIList[i].getHitbox().size.y - BroAIList[i].getOrigin().y)));
                        if (willJump) BroAIList[i].setYVelo(-8.f);
                        else if (willFall) {
                            BroAIList[i].setFalling(true);
                            BroAIList[i].setLastY(BroAIList[i].getCurrentPosition().y);
                            BroAIList[i].setYVelo(-3.f);
                        }
                    } else {
                        BroAIList[i].setYVelo(0.0f);
                    }
                }
            }
        }

        NoAdd = false;
        be = find_max_iny(BroAIList[i].getCurrentPosition(), ObstaclesVertPosList);
        nd = find_min_iny_dist(BroAIList[i].getCurrentPosition(), ObstaclesVertPosList, 64.0f - (BroAIList[i].getYVelo()) * 16.0f);
        ObstacleCollide = isAccurateCollideTop(MFCPP::CollisionObject(BroAIList[i].getCurrentPosition(), BroAIList[i].getOrigin(), BroAIList[i].getHitbox()), ObstaclesVertPosList, CurrPosYCollide, NoAdd, nd, be, 80.0f);
        if (ObstacleCollide) {
            BroAIList[i].setYVelo(0.0f);
            BroAIList[i].setCurrentPosition(sf::Vector2f(BroAIList[i].getCurrentPosition().x, CurrPosYCollide - BroAIList[i].getHitbox().position.y + (32.0f + BroAIList[i].getOrigin().y)));
        }
    }
}
void BroAIDraw() {
    for (auto &i : BroAIList) {
        if (!isOutScreen(i.getCurrentPosition().x, i.getCurrentPosition().y, 32.f, 32.f)) {
            i.AnimationUpdate(i.getInterpolatedPosition(), i.getOrigin());
            i.AnimationDraw(window);
        }
    }
}