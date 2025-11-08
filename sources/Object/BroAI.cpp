#include "Object/BroAI.hpp"
#include "Core/Class/BroAIClass.hpp"
#include "Core/ImageManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/ExternalHeaders/plf_colony.h"
#include "Effect/BroAIEffect.hpp"
#include "Effect/MarioEffect.hpp"
#include "Object/Mario.hpp"
#include "Projectiles/BroAIProjectile.hpp"

plf::colony<MFCPP::BroAI> BroAIList;
static bool BroAIDeleteGate = false;

static std::vector<std::string> HammerBroAnimName;
static std::vector<std::string> HammerBroLaunchAnimName;
static std::vector<std::string> FireBroAnimName;
static std::vector<std::string> FireBroLaunchAnimName;
static constexpr int HAMMER_BRO_IMAGE_WIDTH = 96;
static constexpr int HAMMER_BRO_WIDTH = 48;
static constexpr int HAMMER_BRO_HEIGHT = 64;

void BroAILoadRes() {
    for (int i = 0; i < HAMMER_BRO_IMAGE_WIDTH / HAMMER_BRO_WIDTH; i++) {
        ImageManager::AddTexture(fmt::format("HammerBro_{}", i), "data/resources/Bro/HammerBro.png", sf::IntRect({i * HAMMER_BRO_WIDTH, 0}, {HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}));
        HammerBroAnimName.push_back(fmt::format("HammerBro_{}", i));
        ImageManager::AddTexture(fmt::format("FireBro_{}", i), "data/resources/Bro/FireBro.png", sf::IntRect({i * HAMMER_BRO_WIDTH, 0}, {HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}));
        FireBroAnimName.push_back(fmt::format("FireBro_{}", i));

        ImageManager::AddTexture(fmt::format("HammerBroLaunch_{}", i), "data/resources/Bro/HammerBro.png", sf::IntRect({i * HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}, {HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}));
        HammerBroLaunchAnimName.push_back(fmt::format("HammerBroLaunch_{}", i));
        ImageManager::AddTexture(fmt::format("FireBroLaunch_{}", i), "data/resources/Bro/FireBro.png", sf::IntRect({i * HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}, {HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}));
        FireBroLaunchAnimName.push_back(fmt::format("FireBroLaunch_{}", i));
    }
}
static void BroLaunchProjectile(const plf::colony<MFCPP::BroAI>::colony_iterator<false>& it) {
    switch (it->getType()) {
        case BroAIType::HAMMER_BRO:
            if (it->getAnimationDirection() == AnimationDirection::ANIM_RIGHT)
                AddBroAIProjectile(static_cast<bool>(it->getAnimationDirection()), BROAI_HAMMER, it->getCurrentPosition().x + 5.f, it->getCurrentPosition().y - 31.f);
            else
                AddBroAIProjectile(static_cast<bool>(it->getAnimationDirection()), BROAI_HAMMER, it->getCurrentPosition().x - 5.f, it->getCurrentPosition().y - 31.f);
            SoundManager::PlaySound("Hammer");
            break;
        case BroAIType::FIRE_BRO:
            if (it->getAnimationDirection() == AnimationDirection::ANIM_RIGHT)
                AddBroAIProjectile(static_cast<bool>(it->getAnimationDirection()), BROAI_FIREBALL, it->getCurrentPosition().x + 6.f, it->getCurrentPosition().y - 21.f);
            else
                AddBroAIProjectile(static_cast<bool>(it->getAnimationDirection()), BROAI_FIREBALL, it->getCurrentPosition().x - 6.f, it->getCurrentPosition().y - 21.f);
            SoundManager::PlaySound("Fireball");
            break;
        default: ;
    }
}
void DeleteBroAIIndex(const plf::colony<MFCPP::BroAI>::colony_iterator<false>& it) {
    BroAIDeleteGate = true;
    it->setDestroyed(true);
}
void DeleteBroAI(const float x, const float y) {
    for (auto it = BroAIList.begin(); it != BroAIList.end(); ++it) {
        if (it->getCurrentPosition().x == x && it->getCurrentPosition().y == y)
            DeleteBroAIIndex(it);
    }
}
void DeleteAllBroAI() {
    BroAIList.clear();
}
void SetPrevBroAIPos() {
    for (auto & i : BroAIList) {
        if (i.isDestroyed()) continue;

        i.setPreviousPosition(i.getCurrentPosition());
    }
}
void InterpolateBroAIPos(const float alpha) {
    for (auto & i : BroAIList) {
        if (i.isDestroyed()) continue;

        i.setInterpolatedPosition(linearInterpolation(i.getPreviousPosition(), i.getCurrentPosition(), alpha));
    }
}
void AddBroAI(const BroAIType type, const BroAIMovementType movementType, const float x, const float y) {
    plf::colony<MFCPP::BroAI>::colony_iterator<false> it;
    switch (type) {
        case BroAIType::HAMMER_BRO:
            it = BroAIList.emplace(type, movementType, 2.f, 100.f, 3.f, 1, 0.f, sf::FloatRect({7.f, 16.f}, {32.f, 48.f}),
                               sf::Vector2f(x, y), sf::Vector2f(24.f, 63.f));
            it->setAnimation(0, 1, 14, true);
            it->setAnimationSequence(HammerBroAnimName);
            break;
        case BroAIType::FIRE_BRO:
            it = BroAIList.emplace(type, movementType, 2.f, 100.f, 3.f, 1, 0.f, sf::FloatRect({7.f, 16.f}, {32.f, 48.f}),
                   sf::Vector2f(x, y), sf::Vector2f(24.f, 63.f));
            it->setAnimation(0, 1, 14, true);
            it->setAnimationSequence(FireBroAnimName);
            break;
        default: ;
    }
}
void BroAIStatusUpdate() {
    for (auto it = BroAIList.begin(); it != BroAIList.end(); ++it) {
        if (it->isDestroyed()) continue;

        if (isOutScreenYBottom(it->getCurrentPosition().y, 80.f)) {
            DeleteBroAIIndex(it);
            //BroAIDeletionPositionList.push_back(it->getCurrentPosition());
        }
        if (!isOutScreen(it->getCurrentPosition().x, it->getCurrentPosition().y, 64.0f, 64.0f))
            if (it->isDisabled()) it->setDisabled(false);
        if (!EffectActive) {
            if (it->getCurrentPosition().x > player.curr.x) it->setAnimationDirection(ANIM_LEFT);
            else if (it->getCurrentPosition().x < player.curr.x) it->setAnimationDirection(ANIM_RIGHT);
        }
        else it->setAnimationDirection(ANIM_LEFT);
    }
}
void BroAICheckCollide() {
    if (EffectActive) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(player.hitboxMain, player.curr, player.property.getOrigin());
    for (auto it = BroAIList.begin(); it != BroAIList.end(); ++it) {
        if (it->isDestroyed() || it->isDisabled() ||
            f_abs(player.curr.x - it->getCurrentPosition().x) >= 160.f) continue;
        if (const sf::FloatRect loopHitbox = getGlobalHitbox(it->getHitbox(), it->getCurrentPosition(), it->getOrigin()); isCollide(loopHitbox, hitbox_mario)) {
            if ((it->getCurrentPosition().y - 16.f > player.curr.y) && Yvelo > 0.f) {
                //player.curr = {player.curr.x, it->getCurrentPosition().y - it->getOrigin().y - 1.f};
                if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) Yvelo = -8.0f;
                else Yvelo = -13.0f;
                SoundManager::PlaySound("Stomp");
                it->DeathBehaviour(SCORE_200);
                AddBroAIEffect(it->getType(), static_cast<bool>(it->getAnimationDirection()), it->getCurrentPosition().x, it->getCurrentPosition().y);
                DeleteBroAIIndex(it);
                break;
            }
            else if (it->getCurrentPosition().y - 16.0f < player.curr.y) {
                PowerDown();
                break;
            }
        }
    }
}
void BroAIShootUpdate(const float deltaTime) {
    for (auto it = BroAIList.begin(); it != BroAIList.end(); ++it) {
        if (it->isDestroyed() || it->isDisabled()) continue;
        if (it->getLaunchTickingTime() == 0.f) {
            it->setLaunchIntervalTicking(it->getLaunchIntervalTicking() + 1.f * deltaTime);
            if (it->getLaunchIntervalTicking() >= it->getLaunchInterval()) {
                if (RandomIntNumberGenerator(0, static_cast<int>(it->getLaunchRNG())) == 1 && !isOutScreen(
                        it->getCurrentPosition().x, it->getCurrentPosition().y, 32.f, 32.f)) {
                    it->setLaunchTickingTime(it->getLaunchTickingTime() + 1.f * deltaTime);
                    switch (it->getType()) {
                        case HAMMER_BRO:
                            it->setAnimationSequence(HammerBroLaunchAnimName);
                            break;
                        case FIRE_BRO:
                            it->setAnimationSequence(FireBroLaunchAnimName);
                            break;
                        default: ;
                    }
                    //std::cout << "Standby\n";
                }
                it->setLaunchIntervalTicking(0.f);
            }
        }
        else if (it->getLaunchTickingTime() > 0.f) {
            if (it->getLaunchTickingTime() <= it->getLaunchWaitTime())
                it->setLaunchTickingTime(it->getLaunchTickingTime() + 1.f * deltaTime);
            else {
                if (it->getLaunchCounting() > 0) {
                    if (it->getLaunchIBTicking() < it->getLaunchIntervalBetween()) {
                        it->setLaunchIBTicking(it->getLaunchIBTicking() + 1.f * deltaTime);
                    }
                    else {
                        BroLaunchProjectile(it);
                        it->setLaunchIBTicking(0.f);
                        it->setLaunchCounting(it->getLaunchCounting() - 1);
                    }
                }
                else {
                    it->setLaunchIBTicking(it->getLaunchIntervalBetween());
                    it->setLaunchCounting(it->getLaunchCount());
                    it->setLaunchTickingTime(0.f);
                    switch (it->getType()) {
                        case HAMMER_BRO:
                            it->setAnimationSequence(HammerBroAnimName);
                            break;
                        case FIRE_BRO:
                            it->setAnimationSequence(FireBroAnimName);
                            break;
                        default: ;
                    }
                }
                //std::cout << "Throw\n";
            }
        }

    }
}
static void SetWalkingValue(const plf::colony<MFCPP::BroAI>::colony_iterator<false>& it, const int multiply, const bool reverse) {
    const float valtest = multiply * (it->getRandomWalking() + it->getFixedWalkingValue()) * (reverse ? -1.f : 1.f);
    if (valtest >= 0.0f) it->setDirectionWalking(false);
    else it->setDirectionWalking(true);
    it->setMovingValue(f_abs(valtest));
}
void BroAIVertXUpdate(const float deltaTime) {
    float CurrPosXCollide, CurrPosYCollide;
    for (auto it = BroAIList.begin(); it != BroAIList.end(); ++it) {
        if (it->isDestroyed() || it->isDisabled()) continue;
        // Movement
        if (it->getMovingValue() <= 0.f) {
            if (it->getMovingValue() < 0.f) it->setMovingValue(0.f);
            it->setTimeTicking(it->getTimeTicking() + it->getTickingValue() * deltaTime);
        }
        else if (it->getMovingValue() > 0.f) {
            if (!it->getDirectionWalking())
                it->setCurrentPosition(sf::Vector2f(it->getCurrentPosition().x + it->getSpeed() * deltaTime, it->getCurrentPosition().y));
            else
                it->setCurrentPosition(sf::Vector2f(it->getCurrentPosition().x - it->getSpeed() * deltaTime, it->getCurrentPosition().y));
            it->setMovingValue(it->getMovingValue() - 1.f * deltaTime);
        }
        switch (it->getState()) {
            case 1:
                SetWalkingValue(it, 1, true);
                it->setTimeTicking(0.f);
                it->setState(2);
                break;
            case 2:
                if (it->getMovingValue() <= 0.f) {
                    it->setMovingValue(0.f);
                    it->setState(3);
                }
                break;
            case 3:
                if (it->getTimeTicking() > it->getStopDuration()) {
                    it->setTimeTicking(0.f);
                    SetWalkingValue(it, 2, false);
                    it->setState(4);
                }
                break;
            case 4:
                if (it->getMovingValue() <= 0.f) {
                    it->setMovingValue(0.f);
                    it->setState(5);
                }
                break;
            case 5:
                if (it->getTimeTicking() > it->getStopDuration()) {
                    it->setTimeTicking(0.f);
                    SetWalkingValue(it, 1, true);
                    it->setState(6);
                }
                break;
            case 6:
                if (it->getMovingValue() <= 0.f) {
                    it->setMovingValue(0.f);
                    it->setState(7);
                }
                break;
            case 7:
                it->setTickingValue(it->getFixedTickingValue() + static_cast<float>(RandomIntNumberGenerator(0, static_cast<int>(it->getTickingRNGRange()-1.f))));
                it->setRandomWalking(static_cast<float>(RandomIntNumberGenerator(0, static_cast<int>(it->getWalkingRNGRange()-1.f))) * -1.f);
                it->setState(1);
                break;
            default: ;
        }
        //collision
        // Count if size AllCollideList equal to CollideAddCounter
        CurrPosXCollide = 0, CurrPosYCollide = 0;
        // 0 for right direction; 1 for left direction
        if (float PlatDistance; PlatformXCollision(MFCPP::CollisionObject(it->getCurrentPosition(), it->getOrigin(), it->getHitbox()), PlatDistance, it->getYVelo()))
            it->move(sf::Vector2f(PlatDistance, 0.f));
        bool NoAdd = false;
        const auto [fst, snd] = QuickCheckOnlyObstaclesSideCollision(
            MFCPP::CollisionObject(it->getCurrentPosition(), it->getOrigin(),
                it->getWallHitbox()), it->getDirectionWalking(), CurrPosXCollide, CurrPosYCollide, NoAdd);

        if (fst) {
            it->setCurrentPosition(sf::Vector2f(CurrPosXCollide + 32.0f - it->getHitbox().position.x + it->getOrigin().x, it->getCurrentPosition().y));
        }
        if (snd) {
            it->setCurrentPosition(sf::Vector2f(CurrPosXCollide - (it->getHitbox().position.x + (it->getHitbox().size.x - it->getOrigin().x)), it->getCurrentPosition().y));
        }
    }
}
void BroAIVertYUpdate(const float deltaTime) {
    for (auto & i : BroAIList) {
        float CurrPosYCollide, CurrPosXCollide;
        if (i.isDestroyed() || i.isDisabled()) continue;
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

        if (float PlatPosY; PlatformYCollision(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.getHitbox()), PlatPosY, i.getYVelo(), false)) {
            i.setCurrentPosition(sf::Vector2f(i.getCurrentPosition().x, PlatPosY));
            i.setYVelo(0.f);
        }

        bool NoAdd = false;
        if (QuickCheckOnlyObstacleBotCollision(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.getHitbox()), CurrPosXCollide, CurrPosYCollide, NoAdd)) {
            if (const float offset = std::min(i.getSpeed() + 1.f, 3.f); i.getYVelo() >= -i.getSpeed()) {
                const float floorY = GetCurrFloorY(i.getCurrentPosition(), CurrPosXCollide, CurrPosYCollide);
                if (i.getCurrentPosition().y < CurrPosYCollide + floorY - offset) continue;
                i.setYVelo(0.0f);
                i.setCurrentPosition(sf::Vector2f(i.getCurrentPosition().x, CurrPosYCollide + floorY - i.getHitbox().position.y - (i.getHitbox().size.y - i.getOrigin().y)));
                if (willJump) i.setYVelo(-8.f);
            }
        }
        else {
            if (QuickCheckOnlyHittableBotCollision(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.getHitbox()), CurrPosXCollide, CurrPosYCollide, NoAdd) && i.getYVelo() > 0.f) {
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
                    }
                }
            }
        }

        NoAdd = false;
        if (QuickCheckOnlyObstacleTopCollision(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.getHitbox()), CurrPosXCollide, CurrPosYCollide, NoAdd)) {
            i.setYVelo(0.0f);
            i.setCurrentPosition(sf::Vector2f(i.getCurrentPosition().x, CurrPosYCollide - i.getHitbox().position.y + (32.0f + i.getOrigin().y)));
        }
    }
}
void BroAIDraw() {
    for (auto &i : BroAIList) {
        if (!isOutScreen(i.getCurrentPosition().x, i.getCurrentPosition().y, 64.f, 64.f)) {
            i.AnimationUpdate(i.getInterpolatedPosition(), i.getOrigin());
            i.AnimationDraw();
        }
    }
}
void BroAICleanup() {
    if (!BroAIDeleteGate) return;
    auto it = BroAIList.begin();
    while (it != BroAIList.end()) {
        if (!it->isDestroyed()) ++it;
        else it = BroAIList.erase(it);
    }
    BroAIDeleteGate = false;
}