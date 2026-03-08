#include "Block/Brick.hpp"
#include "Object/Coin.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Bonus/NormalBrick.hpp"
#include "Object/Bonus/NormalCoinBrick.hpp"
void BricksInitTemp() {
	ImageManager::AddTexture("NormalBrick", "data/resources/Brick.png", sf::IntRect({0, 0}, {32, 32}));
	ImageManager::AddTexture("NormalHittedBrick", "data/resources/Brick.png", sf::IntRect({0, 32}, {32, 32}));
	ImageManager::AddTexture("GrayBrick", "data/resources/Brick.png", sf::IntRect({32, 0}, {32, 32}));
	ImageManager::AddTexture("GrayHittedBrick", "data/resources/Brick.png", sf::IntRect({32, 32}, {32, 32}));
}
void ForceLoadBricksTexture(const BrickID ID) {
	switch (ID) {
		case BRICK_NORMAL:
			ImageManager::LoadTexture(R"(NormalBrick)");
			ImageManager::LoadTexture(R"(NormalHittedBrick)");
			ImageManager::LoadTexture(R"(NormalBrickParticle)");
			break;
		case BRICK_GRAY:
			ImageManager::LoadTexture("GrayBrick");
			ImageManager::LoadTexture("GrayHittedBrick");
			ImageManager::LoadTexture("GrayBrickParticle");
			break;
		default: ;
	}
}
void BricksInit() {
	// Loading Texture
	ImageManager::PreloadTexture("NormalBrick", "data/resources/Brick.png", sf::IntRect({0, 0}, {32, 32}));
	ImageManager::PreloadTexture("NormalHittedBrick", "data/resources/Brick.png", sf::IntRect({0, 32}, {32, 32}));
	ImageManager::PreloadTexture("GrayBrick", "data/resources/Brick.png", sf::IntRect({32, 0}, {32, 32}));
	ImageManager::PreloadTexture("GrayHittedBrick", "data/resources/Brick.png", sf::IntRect({32, 32}, {32, 32}));
	//ImageManager::Loadingtexture("data/resources/Brick.png", "Bricks", 0, 0, 64, 64);
}
void AddBrick(const BrickID ID, const BrickAtt att, const float x, const float y) {
	switch (att) {
		case NORMAL:
			GameScene::customTileManager.addCustomTile<NormalBrick>(sf::Vector2f(x, y));
			break;
		case MULTICOIN:
			GameScene::customTileManager.addCustomTile<NormalCoinBrick>(sf::Vector2f(x, y));
			break;
		default: ;
	}
}