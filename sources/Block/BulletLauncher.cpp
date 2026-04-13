#include "Core/ImageManager.hpp"
#include "Block/BulletLauncher.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Enemy/BulletBillLauncher.hpp"
#include "Object/Enemy/BulletBillLauncherFlipped.hpp"

void BulletLauncherInit() {
    ImageManager::AddTexture("BulletLauncher", "data/resources/BulletLauncher.png");
    ImageManager::AddTexture("BulletLauncherFlipped", "data/resources/BulletLauncherFlipped.png");
}
void AddBulletLauncher(const BulletType type, const float x, const float y) {
    switch (type) {
        case BULLET_NORMAL:
            GameScene::customTileManager.addCustomTile<BulletBillLauncher>(sf::Vector2f(x, y));
            break;
        case BULLET_NORMAL_FLIPPED:
            GameScene::customTileManager.addCustomTile<BulletBillLauncherFlipped>(sf::Vector2f(x, y));
            break;
        default: ;
    }
}