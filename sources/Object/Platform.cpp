#include "Object/Platform.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Platform/BluePlatform.hpp"
#include "Object/Platform/RedPlatform.hpp"

void PlatformInit() {
    ImageManager::AddTexture("RedPlatform", "data/resources/Platform/RedPlatform.png");
    ImageManager::AddTexture("RedSmallPlatform", "data/resources/Platform/RedPlatform.png", sf::IntRect({0, 0}, {32, 32}));
    ImageManager::AddTexture("BluePlatform", "data/resources/Platform/BluePlatform.png");
    ImageManager::AddTexture("BlueSmallPlatform", "data/resources/Platform/BluePlatform.png", sf::IntRect({0, 0}, {32, 32}));
    ImageManager::AddTexture("TankGround", "data/resources/TankGround.png");
}
void AddPlatform(const sf::Vector2f& start, const sf::Vector2f& end, int skinID, const float speed, const bool smooth, const bool fall, const bool wait, const bool small) {
    switch (skinID) {
        case 0:
            GameScene::movingBlockManager.addMovingBlock<RedPlatform>(start, end, speed, smooth, fall, wait, small);
            break;
        case 1:
            GameScene::movingBlockManager.addMovingBlock<BluePlatform>(start, end, speed, smooth, fall, wait, small);
            break;
        default: ;
    }
}