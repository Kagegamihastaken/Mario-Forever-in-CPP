#include "Block/Brick.hpp"
#include "Core/Enumeration/enum.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Bonus/NormalBrick.hpp"
#include "Object/Bonus/NormalCoinBrick.hpp"
void AddBrick(const BrickID ID, const BrickAtt att, const float x, const float y) {
	switch (att) {
		case BrickAtt::NORMAL:
			GameScene::customTileManager.addCustomTile<NormalBrick>(sf::Vector2f(x, y));
			break;
		case BrickAtt::MULTICOIN:
			GameScene::customTileManager.addCustomTile<NormalCoinBrick>(sf::Vector2f(x, y));
			break;
		default: ;
	}
}