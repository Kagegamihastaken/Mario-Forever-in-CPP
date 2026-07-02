#include "Block/LuckyBlock.hpp"

#include "Core/AnimationSequenceManager.hpp"
#include "Object/Coin.hpp"
#include "../../headers/Core/Enumeration/enum.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Bonus/LuckyBlockBeetroot.hpp"
#include "Object/Bonus/LuckyBlockCoin.hpp"
#include "Object/Bonus/LuckyBlockFlower.hpp"
#include "Object/Bonus/LuckyBlockGreenMushroom.hpp"
#include "Object/Bonus/LuckyBlockMushroom.hpp"

void LuckyBLockTemp() {
    MFCPP::AnimationSequenceManager::newData("NormLuckyBlockAnimName");
    MFCPP::AnimationSequenceManager::newData("TreeLuckyBlockAnimName");
    for (int i = 0; i < 3; ++i) {
        ImageManager::AddTexture(fmt::format("NormalLuckyBlock_{}", i), "data/resources/luckyblock.png",
                                 sf::IntRect({i * 32, 0}, {32, 32}));
        MFCPP::AnimationSequenceManager::addSingleFrame("NormLuckyBlockAnimName",
                                                        fmt::format("NormalLuckyBlock_{}", i));
    }
    ImageManager::AddTexture("NormalLuckyBlockHit", "data/resources/luckyblock.png", sf::IntRect({96, 0}, {32, 32}));
    MFCPP::AnimationSequenceManager::addSingleFrame("NormLuckyBlockAnimName", "NormalLuckyBlockHit");
    ImageManager::AddTexture("TreeLuckyBlock", "data/resources/luckyblock.png", sf::IntRect({64, 32}, {32, 32}));
    MFCPP::AnimationSequenceManager::addSingleFrame("TreeLuckyBlockAnimName", "TreeLuckyBlock");
    ImageManager::AddTexture("TreeLuckyBlockHit", "data/resources/luckyblock.png", sf::IntRect({96, 32}, {32, 32}));
    MFCPP::AnimationSequenceManager::addSingleFrame("TreeLuckyBlockAnimName", "TreeLuckyBlockHit");
}

void ForceLoadLuckyBlockTexture(const LuckyBlockID ID) {
    switch (ID) {
        case LuckyBlockID::NORMAL:
            for (int i = 0; i < 3; ++i)
                ImageManager::LoadTexture(fmt::format("NormalLuckyBlock_{}", i));
            ImageManager::LoadTexture("NormalLuckyBlockHit");
            break;
        case LuckyBlockID::TREE:
            ImageManager::LoadTexture("TreeLuckyBlock");
            ImageManager::LoadTexture("TreeLuckyBlockHit");
            break;
        default: ;
    }
}

void LoadLuckyBlock() {
    MFCPP::AnimationSequenceManager::newData("NormLuckyBlockAnimName");
    MFCPP::AnimationSequenceManager::newData("TreeLuckyBlockAnimName");
    for (int i = 0; i < 3; ++i) {
        ImageManager::PreloadTexture(fmt::format("NormalLuckyBlock_{}", i), "data/resources/luckyblock.png",
                                     sf::IntRect({i * 32, 0}, {32, 32}));
        MFCPP::AnimationSequenceManager::addSingleFrame("NormLuckyBlockAnimName",
                                                        fmt::format("NormalLuckyBlock_{}", i));
    }
    ImageManager::PreloadTexture("NormalLuckyBlockHit", "data/resources/luckyblock.png",
                                 sf::IntRect({96, 0}, {32, 32}));
    MFCPP::AnimationSequenceManager::addSingleFrame("NormLuckyBlockAnimName", "NormalLuckyBlockHit");
    ImageManager::PreloadTexture("TreeLuckyBlock", "data/resources/luckyblock.png", sf::IntRect({64, 32}, {32, 32}));
    MFCPP::AnimationSequenceManager::addSingleFrame("TreeLuckyBlockAnimName", "TreeLuckyBlock");
    ImageManager::PreloadTexture("TreeLuckyBlockHit", "data/resources/luckyblock.png", sf::IntRect({96, 32}, {32, 32}));
    MFCPP::AnimationSequenceManager::addSingleFrame("TreeLuckyBlockAnimName", "TreeLuckyBlockHit");
}

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
