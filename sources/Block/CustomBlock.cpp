#include "Block/CustomBlock.hpp"

#include "Core/Scene/GameScene.hpp"
#include "Object/Block/TankGearLeft.hpp"
#include "Object/Block/TankGearMiddle.hpp"
#include "Object/Block/TankGearRight.hpp"

void AddCustomTile(int id, const sf::Vector2f& pos) {
	switch (id) {
		case 0:
			GameScene::customTileManager.addCustomTile<TankGearLeft>(pos);
			break;
		case 1:
			GameScene::customTileManager.addCustomTile<TankGearMiddle>(pos);
			break;
		case 2:
			GameScene::customTileManager.addCustomTile<TankGearRight>(pos);
			break;
		default: ;
	}
}