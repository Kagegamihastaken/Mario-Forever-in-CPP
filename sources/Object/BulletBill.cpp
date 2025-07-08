#include "Object/BulletBill.hpp"
#include <vector>
#include "Core/ImageManager.hpp"
#include "Core/Class/BulletBillClass.hpp"
#include "Core/ExternalHeaders/plf_colony.h"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Effect/BulletBillEffect.hpp"
#include "Effect/MarioEffect.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Object/Mario.hpp"

static std::vector<std::string> BulletBillNormalAnimName;
plf::colony<MFCPP::BulletBill> BulletBillList;
bool BulletBillDeleteGate = false;
void BulletBillInit() {
    ImageManager::AddTexture("BulletBillNormal_0", "data/resources/BulletBill/BulletBillNormal.png");
    BulletBillNormalAnimName.emplace_back("BulletBillNormal_0");
}
void SetPrevBulletBillPos() {
    for (auto & i : BulletBillList) {
        if (i.willBeDestroyed()) continue;

        i.setPreviousPosition(i.getCurrentPosition());
    }
}
void InterpolateBulletBillPos(const float alpha) {
    for (auto & i : BulletBillList) {
        if (i.willBeDestroyed()) continue;

        i.setInterpolatedPosition(linearInterpolation(i.getPreviousPosition(), i.getCurrentPosition(), alpha));
    }
}
void DeleteBulletBillIndex(const plf::colony<MFCPP::BulletBill>::colony_iterator<false>& it) {
    BulletBillDeleteGate = true;
    it->willDestroy(true);
}
void DeleteAllBulletBill() {
    BulletBillList.clear();
}
void BulletBillStatusUpdate() {
    for (auto it = BulletBillList.begin(); it != BulletBillList.end(); ++it) {
        if (it->willBeDestroyed()) continue;

        if (isOutScreen(it->getCurrentPosition().x, it->getCurrentPosition().y, 512, 512))
            DeleteBulletBillIndex(it);
    }
}
void BulletBillPositionUpdate(const float deltaTime) {
    for (auto it = BulletBillList.begin(); it != BulletBillList.end(); ++it) {
        if (it->willBeDestroyed()) continue;

        if (it->getAnimationDirection() == ANIM_LEFT)
            it->move(sf::Vector2f(-it->getSpeed() * deltaTime, 0.f));
        else
            it->move(sf::Vector2f(it->getSpeed() * deltaTime, 0.f));
    }
}
void AddBulletBill(const BulletType type, const float speed, const bool direction, const float x, const float y) {
    plf::colony<MFCPP::BulletBill>::colony_iterator<false> it;
    switch (type) {
        case BULLET_NORMAL:
            it = BulletBillList.emplace(speed, type, sf::FloatRect({0.f, 0.f}, {34.f, 28.f}), sf::Vector2f(x, y), sf::Vector2f(16.f, 28.f));
            it->setAnimation(0, 0, 100, true);
            it->setAnimationSequence(BulletBillNormalAnimName);
            it->setAnimationDirection(static_cast<AnimationDirection>(!direction));
            break;
        default: ;
    }
}
void BulletBillCheckCollide() {
    if (EffectActive) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(player.hitboxMain, player.curr, player.property.getOrigin());

    for (auto it = BulletBillList.begin(); it != BulletBillList.end(); ++it) {
        if (const sf::FloatRect BulletBillHitbox = getGlobalHitbox(it->getHitbox(), it->getCurrentPosition(), it->getOrigin()); isCollide(BulletBillHitbox, hitbox_mario)) {
            if ((it->getCurrentPosition().y - 16.f > player.curr.y) && Yvelo > 0.0f) {
                if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) Yvelo = -8.0f;
                else Yvelo = -13.0f;
                SoundManager::PlaySound("Stomp");
                AddScoreEffect(SCORE_100, it->getCurrentPosition().x, it->getCurrentPosition().y - it->getOrigin().y);
                AddBulletBillEffect(it->getType(), static_cast<bool>(it->getAnimationDirection()), it->getSpeed(), it->getCurrentPosition().x, it->getCurrentPosition().y);
                DeleteBulletBillIndex(it);
                break;
            }
            else if (it->getCurrentPosition().y - 16.f < player.curr.y) {
                PowerDown();
                break;
            }
        }
    }
}
void DrawBulletBill() {
    for (auto &i : BulletBillList) {
        if (i.willBeDestroyed()) continue;

        i.AnimationUpdate(i.getInterpolatedPosition(), i.getOrigin());
        i.AnimationDraw(window);
    }
}
void BulletBillCleanup() {
    if (!BulletBillDeleteGate) return;
    auto it = BulletBillList.begin();
    while (it != BulletBillList.end()) {
        if (!it->willBeDestroyed()) ++it;
        else it = BulletBillList.erase(it);
    }
}
