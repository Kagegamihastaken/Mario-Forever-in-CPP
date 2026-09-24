#include "Core/Loading/PlatformLoading.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Platform/BluePlatform.hpp"
#include "Object/Platform/RedPlatform.hpp"

void MFCPP::AddPlatform(const sf::Vector2f& start, const sf::Vector2f& end, int skinID, const float speed, const bool smooth, const bool fall, const bool wait, const bool small) {
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