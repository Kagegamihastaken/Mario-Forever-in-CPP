#include "Block/LuckyBlock.hpp"
#include "Object/Coin.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Bonus/LuckyBlockCoin.hpp"
#include "Object/Bonus/LuckyBlockFlower.hpp"
#include "Object/Bonus/LuckyBlockGreenMushroom.hpp"
#include "Object/Bonus/LuckyBlockMushroom.hpp"

std::vector<std::string> NormLuckyBlockAnimName;
static std::vector<std::string> TreeLuckyBlockAnimName;
void LuckyBLockTemp() {
	for (int i = 0; i < 3; ++i) {
		ImageManager::AddTexture(fmt::format("NormalLuckyBlock_{}", i), "data/resources/luckyblock.png", sf::IntRect({i * 32, 0}, {32, 32}));
		NormLuckyBlockAnimName.emplace_back(fmt::format("NormalLuckyBlock_{}", i));
	}
	ImageManager::AddTexture("NormalLuckyBlockHit", "data/resources/luckyblock.png", sf::IntRect({96, 0}, {32, 32}));
	NormLuckyBlockAnimName.emplace_back("NormalLuckyBlockHit");
	ImageManager::AddTexture("TreeLuckyBlock", "data/resources/luckyblock.png", sf::IntRect({64, 32}, {32, 32}));
	TreeLuckyBlockAnimName.emplace_back("TreeLuckyBlock");
	ImageManager::AddTexture("TreeLuckyBlockHit", "data/resources/luckyblock.png", sf::IntRect({96, 32}, {32, 32}));
	TreeLuckyBlockAnimName.emplace_back("TreeLuckyBlockHit");
}
void ForceLoadLuckyBlockTexture(const LuckyBlockID ID) {
	switch (ID) {
		case LUCKY_BLOCK:
			for (int i = 0; i < 3; ++i)
				ImageManager::LoadTexture(fmt::format("NormalLuckyBlock_{}", i));
			ImageManager::LoadTexture("NormalLuckyBlockHit");
			break;
		case TREE_LUCKY_BLOCK:
			ImageManager::LoadTexture("TreeLuckyBlock");
			ImageManager::LoadTexture("TreeLuckyBlockHit");
			break;
		default: ;
	}
}
void LoadLuckyBlock() {
	for (int i = 0; i < 3; ++i) {
		ImageManager::PreloadTexture(fmt::format("NormalLuckyBlock_{}", i), "data/resources/luckyblock.png", sf::IntRect({i * 32, 0}, {32, 32}));
		NormLuckyBlockAnimName.emplace_back(fmt::format("NormalLuckyBlock_{}", i));
	}
	ImageManager::PreloadTexture("NormalLuckyBlockHit", "data/resources/luckyblock.png", sf::IntRect({96, 0}, {32, 32}));
	NormLuckyBlockAnimName.emplace_back("NormalLuckyBlockHit");
	ImageManager::PreloadTexture("TreeLuckyBlock", "data/resources/luckyblock.png", sf::IntRect({64, 32}, {32, 32}));
	TreeLuckyBlockAnimName.emplace_back("TreeLuckyBlock");
	ImageManager::PreloadTexture("TreeLuckyBlockHit", "data/resources/luckyblock.png", sf::IntRect({96, 32}, {32, 32}));
	TreeLuckyBlockAnimName.emplace_back("TreeLuckyBlockHit");
}
void AddLuckyBlock(const LuckyBlockID ID, const LuckyBlockAtt Att, float x, float y) {
	switch (Att) {
		case LUCKY_GREEN_MUSHROOM:
			GameScene::customTileManager.addCustomTile<LuckyBlockGreenMushroom>(sf::Vector2f(x, y));
			break;
		case LUCKY_MUSHROOM:
			GameScene::customTileManager.addCustomTile<LuckyBlockMushroom>(sf::Vector2f(x, y));
			break;
		case LUCKY_FIRE_FLOWER:
			GameScene::customTileManager.addCustomTile<LuckyBlockFlower>(sf::Vector2f(x, y));
			break;
		case LUCKY_COIN:
			GameScene::customTileManager.addCustomTile<LuckyBlockCoin>(sf::Vector2f(x, y));
			break;
		default: ;
	}
}