#include "Core/ImageManager.hpp"
#include "Core/Tilemap.hpp"
#include "Block/BulletLauncher.hpp"
#include "Effect/FireballExplosion.hpp"

#include "Core/Logging.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"

#include "Core/WindowFrame.hpp"
#include "Core/Class/BulletLauncherClass.hpp"
#include "Core/ExternalHeaders/plf_colony.h"
#include "Object/Mario.hpp"

plf::colony<MFCPP::BulletLauncher> BulletLauncherList;

static void BulletLauncherShot(const plf::colony<MFCPP::BulletLauncher>::colony_iterator<false>& it) {
    SoundManager::PlaySound(fmt::format("Bullet{}", RandomIntNumberGenerator(1, 3)));
    if (it->getCurrentPosition().x > player.curr.x) AddFireballExplosion(it->getCurrentPosition().x - it->getOrigin().x, it->getCurrentPosition().y - 32.f / 2.f + 1.f);
    else AddFireballExplosion(it->getCurrentPosition().x + 32.f - it->getOrigin().x, it->getCurrentPosition().y - 32.f / 2.f + 1.f);
}

void BulletLauncherInit() {
    ImageManager::AddTexture("BulletLauncher", "data/resources/BulletLauncher.png");
}
void BulletLauncherStatusUpdate(const float deltaTime) {
    for (auto it = BulletLauncherList.begin(); it != BulletLauncherList.end(); ++it) {
        if (isOutScreen(it->getCurrentPosition().x, it->getCurrentPosition().y, 32.f, 32.f)) continue;

        if (f_abs(it->getCurrentPosition().x - player.curr.x) > 80.f) it->setDisabled(false);
        else it->setDisabled(true);

        if (!it->isDisabled())
            it->setTiming(it->getTiming() - 1.f * deltaTime);

        //state
        if (it->getState()) {
            if (it->getTiming() <= 0.f) {
                BulletLauncherShot(it);
                MFCPP::Log::InfoPrint("Bullet Shot! (2nd State)");
                it->setTiming(it->getLaunchInterval() + RandomIntNumberGenerator(0, it->getRandomFireInterval()));
            }
        }
        else {
            if (it->getTiming() <= -1.f * it->getFirstShotTime()) {
                BulletLauncherShot(it);
                MFCPP::Log::InfoPrint("Bullet Shot!");
                it->setTiming(it->getLaunchInterval() + RandomIntNumberGenerator(0, it->getRandomFireInterval()));
                it->setState(true);
            }
        }
    }
}
void AddBulletLauncher(const BulletType type, const float x, const float y) {
    MFCPP::setIndexTilemapCollision(x, y, true);
    MFCPP::setIndexTilemapID(x, y, 0);
    plf::colony<MFCPP::BulletLauncher>::colony_iterator<false> it;
    switch (type) {
        case BULLET_NORMAL:
            it = BulletLauncherList.emplace(75.f, sf::Vector2f(x, y));
            it->setTexture("BulletLauncher");
            break;
        default: ;
    }
}
void BulletLauncherClear() {
    BulletLauncherList.clear();
}
void BulletLauncherDraw() {
    for (auto &i : BulletLauncherList) {
        i.AnimationUpdate(i.getInterpolatedPosition(), i.getOrigin());
        i.AnimationDraw(window);
    }
}