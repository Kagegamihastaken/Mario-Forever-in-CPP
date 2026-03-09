#include "Core/ImageManager.hpp"
#include "Block/BulletLauncher.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Enemy/BulletBillLauncher.hpp"

void BulletLauncherInit() {
    ImageManager::AddTexture("BulletLauncher", "data/resources/BulletLauncher.png");
}
void AddBulletLauncher(const BulletType type, const float x, const float y) {
    GameScene::customTileManager.addCustomTile<BulletBillLauncher>(sf::Vector2f(x, y));
}