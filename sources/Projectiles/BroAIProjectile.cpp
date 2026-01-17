#include "Core/Class/BroAIProjectileClass.hpp"
#include "Core/Interpolation.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Scroll.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Loading/enum.hpp"
#include "Effect/MarioEffect.hpp"
#include "Object/Mario.hpp"
#include "Core/ProjectileBehaviour/Fireball.hpp"
#include "Effect/FireballExplosion.hpp"

std::vector<MFCPP::BroAIProjectile> BroAIProjectileList;
static bool BroAIProjectileDeleteGate = false;

void SetPrevBroAIProjectilePos() {
    for (auto & i : BroAIProjectileList) {
        i.setPreviousPosition(i.getCurrentPosition());
        i.setPreviousAngle(i.getCurrentAngle());
    }
}
void InterpolateBroAIProjectilePos(const float alpha) {
    for (auto & i : BroAIProjectileList) {
        i.setInterpolatedPosition(linearInterpolation(i.getPreviousPosition(), i.getCurrentPosition(), alpha));
        i.setInterpolatedAngle(linearInterpolation(i.getPreviousAngle(), i.getCurrentAngle(), alpha));
    }
}
void DeleteBroAIProjectile(const std::vector<MFCPP::BroAIProjectile>::iterator& it, const bool out = false) {
    if (!it->isDestroyed() && !out) AddFireballExplosion(it->getCurrentPosition().x, it->getCurrentPosition().y);
    it->setDestroyed(true);
    BroAIProjectileDeleteGate = true;
}
void DeleteAllBroAIProjectile() {
    BroAIProjectileList.clear();
}
void BroAIProjectileCollision() {
    if (EffectActive) return;
    if (BroAIProjectileList.empty()) return;
    const sf::FloatRect playerHitbox = getGlobalHitbox(player.hitboxMain, player.curr, player.property.getOrigin());
    for (auto it = BroAIProjectileList.begin(); it != BroAIProjectileList.end(); ++it) {
        if (sf::FloatRect loopHitbox = getGlobalHitbox(it->getHitbox(), it->getCurrentPosition(), it->getOrigin()); isCollide(loopHitbox, playerHitbox)) {
            PowerDown();
            switch (it->getBehaviour()) {
                case BROAI_FIREBALL_BEHAVIOUR:
                    DeleteBroAIProjectile(it);
                    break;
                default: ;
            }
        }
    }
}
void BroAIProjectileStatusUpdate() {
    for (auto it = BroAIProjectileList.begin(); it != BroAIProjectileList.end(); ++it) {
        if (isOutScreenYBottom(it->getInterpolatedPosition().y, 64.f)) {
            DeleteBroAIProjectile(it, true);
        }
    }
}
void BroAIProjectileSpin(const float deltaTime) {
    for (auto it = BroAIProjectileList.begin(); it != BroAIProjectileList.end(); ++it) {
        if (it->isDestroyed()) continue;

        switch (it->getBehaviour()) {
            case BROAI_HAMMER_BEHAVIOUR:
                if (it->getDirection()) it->setCurrentAngle(it->getCurrentAngle() - sf::degrees(5.f * deltaTime));
                else it->setCurrentAngle(it->getCurrentAngle() + sf::degrees(5.f * deltaTime));
                break;
            case BROAI_FIREBALL_BEHAVIOUR:
                if (it->getDirection()) it->setCurrentAngle(it->getCurrentAngle() - sf::degrees(11.5f * deltaTime));
                else it->setCurrentAngle(it->getCurrentAngle() + sf::degrees(11.5f * deltaTime));
                break;
            default: ;
        }
    }
}
void AddBroAIProjectile(const bool direction, const BroAIProjectileType type, const float x, const float y) {
    switch (type) {
        case BroAIProjectileType::BROAI_HAMMER:
            BroAIProjectileList.emplace_back(direction, type, BROAI_HAMMER_BEHAVIOUR, 1.f + static_cast<float>(RandomIntNumberGenerator(0, 4)), (6.f + static_cast<float>(RandomIntNumberGenerator(0, 4))) * -1.f, sf::FloatRect({0.f, 0.f}, {24.f, 24.f}), sf::Vector2f(x, y), sf::Vector2f(13.f, 18.f));
            BroAIProjectileList.back().setTexture("Hammer", direction);
            break;
        case BroAIProjectileType::BROAI_FIREBALL:
            BroAIProjectileList.emplace_back(direction, type, BROAI_FIREBALL_BEHAVIOUR, 8.125f, 0.f, sf::FloatRect({0.f, 0.f}, {15.f, 16.f}), sf::Vector2f(x, y), sf::Vector2f(7.f, 8.f));
            BroAIProjectileList.back().setTexture("Fireball", direction);
            break;
        default: ;
    }
}
//behavior (will move to another file)
static void HammerBroX(const std::vector<MFCPP::BroAIProjectile>::iterator& it, const float deltaTime) {
    if (it->getDirection())
        it->setCurrentPosition(sf::Vector2f(it->getCurrentPosition().x - it->getXVelo() * deltaTime, it->getCurrentPosition().y));
    else
        it->setCurrentPosition(sf::Vector2f(it->getCurrentPosition().x + it->getXVelo() * deltaTime, it->getCurrentPosition().y));
}
static void HammerBroY(const std::vector<MFCPP::BroAIProjectile>::iterator& it, const float deltaTime) {
    it->move(sf::Vector2f(0.0f, it->getYVelo() * deltaTime));
    it->setYVelo(it->getYVelo() + (it->getYVelo() >= 10.0f ? 0.0f : 1.0f * deltaTime * 0.215f));
}
static void FireBroX(const std::vector<MFCPP::BroAIProjectile>::iterator& it, const float deltaTime) {
    const auto [Xvel, Yvel, X, Y, remove] = FireballX(it->getCurrentPosition(), it->getXVelo(), it->getYVelo(), it->getDirection(), deltaTime, it->getHitbox(), it->getOrigin());
    if (remove) {
        DeleteBroAIProjectile(it);
        return;
    }
    it->setCurrentPosition(sf::Vector2f(X, Y));
    it->setXVelo(Xvel);
    it->setYVelo(Yvel);
}
static void FireBroY(const std::vector<MFCPP::BroAIProjectile>::iterator& it, const float deltaTime) {
    const auto [Xvel, Yvel, X, Y, remove] = FireballY(it->getCurrentPosition(), it->getXVelo(), it->getYVelo(), deltaTime, it->getHitbox(), it->getOrigin());
    if (remove) {
        DeleteBroAIProjectile(it);
        return;
    }
    it->setCurrentPosition(sf::Vector2f(X, Y));
    it->setXVelo(Xvel);
    it->setYVelo(Yvel);
}
void BroAIProjectileMovementUpdate(const float deltaTime) {
    for (auto it = BroAIProjectileList.begin(); it != BroAIProjectileList.end(); ++it) {
        switch (it->getBehaviour()) {
            case BROAI_HAMMER_BEHAVIOUR:
                HammerBroX(it, deltaTime);
                HammerBroY(it, deltaTime);
                break;
            case BROAI_FIREBALL_BEHAVIOUR:
                FireBroX(it, deltaTime);
                FireBroY(it, deltaTime);
                break;
            default: ;
        }
    }
}
void BroAIProjectileDraw() {
    for (auto& i : BroAIProjectileList) {
        i.AnimationUpdate(i.getInterpolatedPosition(), i.getOrigin());
        i.setRotation(i.getInterpolatedAngle());
        i.AnimationDraw();
    }
}
void BroAIProjectileCleanup() {
    if (!BroAIProjectileDeleteGate) return;
    auto it = BroAIProjectileList.begin();
    while (it != BroAIProjectileList.end()) {
        if (!it->isDestroyed()) ++it;
        else it = BroAIProjectileList.erase(it);
    }
    BroAIProjectileDeleteGate = false;
}