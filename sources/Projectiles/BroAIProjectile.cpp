#include "Class/BroAIProjectileClass.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Interpolation.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Scroll.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Loading/enum.hpp"
#include "Object/Mario.hpp"

std::vector<MFCPP::BroAIProjectile> BroAIProjectileList;

void BroAIProjectileInit() {
    ImageManager::AddImage("HammerImage", "data/resources/Projectiles/Hammer.png");
    ImageManager::AddTexture("HammerImage", "HammerRight");
    ImageManager::AddTexture("HammerImage", "HammerLeft", false, true);
}
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
void DeleteBroAIProjectile(const float x, const float y) {
    for (int i = 0; i < BroAIProjectileList.size(); ++i) {
        if (BroAIProjectileList[i].getCurrentPosition().x == x && BroAIProjectileList[i].getCurrentPosition().y == y)
            BroAIProjectileList.erase(BroAIProjectileList.begin() + i);
    }
}
void DeleteAllBroAIProjectile() {
    BroAIProjectileList.clear();
}
void BroAIProjectileCollision() {
    if (BroAIProjectileList.empty()) return;
    const sf::FloatRect playerHitbox = getGlobalHitbox(player.hitboxMain, player.curr, player.property.getOrigin());
    for (int i = 0; i < BroAIProjectileList.size(); ++i) {
        if (sf::FloatRect loopHitbox = getGlobalHitbox(BroAIProjectileList[i].getHitbox(), BroAIProjectileList[i].getCurrentPosition(), BroAIProjectileList[i].getOrigin()); isCollide(loopHitbox, playerHitbox)) PowerDown();
    }
}
void BroAIProjectileStatusUpdate() {
    for (int i = 0; i < BroAIProjectileList.size(); ++i) {
        if (isOutScreenY(BroAIProjectileList[i].getInterpolatedPosition().y, 64.f)) {
            DeleteBroAIProjectile(BroAIProjectileList[i].getCurrentPosition().x, BroAIProjectileList[i].getCurrentPosition().y);
        }
    }
}
void BroAIProjectileSpin(const float deltaTime) {
    for (int i = 0; i < BroAIProjectileList.size(); ++i) {
        if (BroAIProjectileList[i].getDirection()) BroAIProjectileList[i].setCurrentAngle(BroAIProjectileList[i].getCurrentAngle() - sf::degrees(5.f * deltaTime));
        else BroAIProjectileList[i].setCurrentAngle(BroAIProjectileList[i].getCurrentAngle() + sf::degrees(5.f * deltaTime));
    }
}
void AddBroAIProjectile(const bool direction, const BroAIProjectileType type, const float x, const float y) {
    switch (type) {
        case BroAIProjectileType::HAMMER:
            BroAIProjectileList.emplace_back(direction, type, HAMMER_BEHAVIOUR, sf::FloatRect({0.f, 0.f}, {24.f, 33.f}), sf::Vector2f(x, y), sf::Vector2f(13, 18));
            if (direction) BroAIProjectileList.back().setTexture("HammerLeft");
            else BroAIProjectileList.back().setTexture("HammerRight");
            break;
        default: ;
    }
}
void BroAIProjectileMovementUpdate(const float deltaTime) {
    for (int i = 0; i < BroAIProjectileList.size(); ++i) {
        if (BroAIProjectileList[i].getBehaviour() == BroAIProjectileBehavior::HAMMER_BEHAVIOUR) {
            //X
            if (BroAIProjectileList[i].getDirection())
                BroAIProjectileList[i].setCurrentPosition(sf::Vector2f(BroAIProjectileList[i].getCurrentPosition().x - BroAIProjectileList[i].getXVelo() * deltaTime, BroAIProjectileList[i].getCurrentPosition().y));
            else
                BroAIProjectileList[i].setCurrentPosition(sf::Vector2f(BroAIProjectileList[i].getCurrentPosition().x + BroAIProjectileList[i].getXVelo() * deltaTime, BroAIProjectileList[i].getCurrentPosition().y));
            //Y
            BroAIProjectileList[i].move(sf::Vector2f(0.0f, BroAIProjectileList[i].getYVelo() * deltaTime));
            BroAIProjectileList[i].setYVelo(BroAIProjectileList[i].getYVelo() + (BroAIProjectileList[i].getYVelo() >= 10.0f ? 0.0f : 1.0f * deltaTime * 0.215f));
        }
    }
}
void BroAIProjectileDraw() {
    for (auto& i : BroAIProjectileList) {
        i.AnimationUpdate(i.getInterpolatedPosition(), i.getOrigin());
        i.setRotation(i.getInterpolatedAngle());
        i.AnimationDraw(window);
    }
}