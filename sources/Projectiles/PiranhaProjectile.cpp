#include "Core/Class/PiranhaAIProjectileClass.hpp"
#include "Core/Interpolation.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Scroll.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/ExternalHeaders/plf_colony.h"
#include "Core/Loading/enum.hpp"
#include "Effect/FireballExplosion.hpp"
#include "Effect/MarioEffect.hpp"
#include "Object/Mario.hpp"

plf::colony<MFCPP::PiranhaAIProjectile> PiranhaAIProjectileList;
static bool PiranhaAIProjectileDeleteGate = false;
void SetPrevPiranhaAIProjectilePos() {
    for (auto & i : PiranhaAIProjectileList) {
        i.setPreviousPosition(i.getCurrentPosition());
        i.setPreviousAngle(i.getCurrentAngle());
    }
}
void InterpolatePiranhaAIProjectilePos(const float alpha) {
    for (auto & i : PiranhaAIProjectileList) {
        i.setInterpolatedPosition(linearInterpolation(i.getPreviousPosition(), i.getCurrentPosition(), alpha));
        i.setInterpolatedAngle(linearInterpolation(i.getPreviousAngle(), i.getCurrentAngle(), alpha));
    }
}
void DeletePiranhaAIProjectile(const plf::colony<MFCPP::PiranhaAIProjectile>::colony_iterator<false>& it) {
    it->setDestroyed(true);
    PiranhaAIProjectileDeleteGate = true;
}
void DeleteAllPiranhaAIProjectile() {
    PiranhaAIProjectileList.clear();
}
void PiranhaAIProjectileCollision() {
    if (EffectActive) return;
    if (PiranhaAIProjectileList.empty()) return;
    const sf::FloatRect playerHitbox = getGlobalHitbox(player.hitboxMain, player.curr, player.property.getOrigin());
    for (auto it = PiranhaAIProjectileList.begin(); it != PiranhaAIProjectileList.end(); ++it) {
        if (sf::FloatRect loopHitbox = getGlobalHitbox(it->getHitbox(), it->getCurrentPosition(), it->getOrigin()); isCollide(loopHitbox, playerHitbox)) {
            AddFireballExplosion(it->getCurrentPosition().x, it->getCurrentPosition().y);
            DeletePiranhaAIProjectile(it);
            PowerDown();
        }
    }
}
void PiranhaAIProjectileStatusUpdate() {
    for (auto it = PiranhaAIProjectileList.begin(); it != PiranhaAIProjectileList.end(); ++it) {
        if (isOutScreenYBottom(it->getInterpolatedPosition().y, 32.f)) {
            DeletePiranhaAIProjectile(it);
        }
    }
}
void PiranhaAIProjectileSpin(const float deltaTime) {
    for (auto it = PiranhaAIProjectileList.begin(); it != PiranhaAIProjectileList.end(); ++it) {
        if (it->getDirection()) it->setCurrentAngle(it->getCurrentAngle() - sf::degrees(11.5f * deltaTime));
        else it->setCurrentAngle(it->getCurrentAngle() + sf::degrees(11.5f * deltaTime));
    }
}
void AddPiranhaAIProjectile(const bool direction, const PiranhaDirection piranha_direction, const PiranhaProjectileType type, const float x, const float y) {
    plf::colony<MFCPP::PiranhaAIProjectile>::colony_iterator<false> it;
    switch (type) {
        case PiranhaProjectileType::PIRANHA_FIREBALL:
            it = PiranhaAIProjectileList.emplace(direction, type, piranha_direction, sf::FloatRect({0.f, 0.f}, {15.f, 16.f}), sf::Vector2f(x, y), sf::Vector2f(7, 8));
            it->setTexture("Fireball", direction);
            break;
        default: ;
    }
}
void PiranhaAIProjectileMovementUpdate(const float deltaTime) {
    for (auto it = PiranhaAIProjectileList.begin(); it != PiranhaAIProjectileList.end(); ++it) {
        //X
        if (it->getDirection())
            it->setCurrentPosition(sf::Vector2f(it->getCurrentPosition().x - it->getXVelo() * deltaTime, it->getCurrentPosition().y));
        else
            it->setCurrentPosition(sf::Vector2f(it->getCurrentPosition().x + it->getXVelo() * deltaTime, it->getCurrentPosition().y));
        //Y
        it->move(sf::Vector2f(0.0f, it->getYVelo() * deltaTime));
        it->setYVelo(it->getYVelo() + (it->getYVelo() >= 10.0f ? 0.0f : 1.0f * deltaTime * 0.175f));
    }
}
void PiranhaAIProjectileDraw() {
    for (auto& i : PiranhaAIProjectileList) {
        i.AnimationUpdate(i.getInterpolatedPosition(), i.getOrigin());
        i.setRotation(i.getInterpolatedAngle());
        i.AnimationDraw();
    }
}
void PiranhaAIProjectileCleanup() {
    if (!PiranhaAIProjectileDeleteGate) return;
    auto it = PiranhaAIProjectileList.begin();
    while (it != PiranhaAIProjectileList.end()) {
        if (!it->isDestroyed()) ++it;
        else it = PiranhaAIProjectileList.erase(it);
    }
    PiranhaAIProjectileDeleteGate = false;
}