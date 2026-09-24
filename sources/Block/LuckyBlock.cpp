#include "Block/LuckyBlock.hpp"

#include "Core/Enumeration/enum.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Bonus/LuckyBlockBeetroot.hpp"
#include "Object/Bonus/LuckyBlockCoin.hpp"
#include "Object/Bonus/LuckyBlockFlower.hpp"
#include "Object/Bonus/LuckyBlockGreenMushroom.hpp"
#include "Object/Bonus/LuckyBlockMushroom.hpp"

void AddLuckyBlock(const LuckyBlockID ID, const LuckyBlockAtt Att, float x, float y) {
    switch (Att) {
        case LuckyBlockAtt::GREEN_MUSHROOM:
            GameScene::customTileManager.addCustomTile<LuckyBlockGreenMushroom>(sf::Vector2f(x, y));
            break;
        case LuckyBlockAtt::MUSHROOM:
            GameScene::customTileManager.addCustomTile<LuckyBlockMushroom>(sf::Vector2f(x, y));
            break;
        case LuckyBlockAtt::FIRE_FLOWER:
            GameScene::customTileManager.addCustomTile<LuckyBlockFlower>(sf::Vector2f(x, y));
            break;
        case LuckyBlockAtt::COIN:
            GameScene::customTileManager.addCustomTile<LuckyBlockCoin>(sf::Vector2f(x, y));
            break;
        case LuckyBlockAtt::BEETROOT:
            GameScene::customTileManager.addCustomTile<LuckyBlockBeetroot>(sf::Vector2f(x, y));
            break;
        default: ;
    }
}
