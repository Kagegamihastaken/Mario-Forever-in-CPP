#include "Core/ImageManager.hpp"
#include "Block/BulletLauncher.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Enemy/BulletBillLauncher.hpp"
#include "Object/Enemy/BulletBillLauncherFlipped.hpp"
#include "Object/Enemy/FireLauncher.hpp"
#include "Object/Enemy/FireLauncherFLipped.hpp"
#include "Object/Enemy/FireLauncherStand.hpp"

void AddBulletLauncher(const BulletType type, const float x, const float y) {
    switch (type) {
        case BULLET_NORMAL:
            GameScene::customTileManager.addCustomTile<BulletBillLauncher>(sf::Vector2f(x, y));
            break;
        case BULLET_NORMAL_FLIPPED:
            GameScene::customTileManager.addCustomTile<BulletBillLauncherFlipped>(sf::Vector2f(x, y));
            break;
        case FIRE_LAUNCHER:
            GameScene::customTileManager.addCustomTile<FireLauncher>(sf::Vector2f(x, y));
            break;
        case FIRE_LAUNCHER_FLIPPED:
            GameScene::customTileManager.addCustomTile<FireLauncherFlipped>(sf::Vector2f(x, y));
            break;
        case FIRE_LAUNCHER_STAND:
            GameScene::customTileManager.addCustomTile<FireLauncherStand>(sf::Vector2f(x, y));
            break;
        default: ;
    }
}
