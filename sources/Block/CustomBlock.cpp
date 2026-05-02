#include "Block/CustomBlock.hpp"

#include "Core/ImageManager.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Block/TankGearLeft.hpp"
#include "Object/Block/TankGearMiddle.hpp"
#include "Object/Block/TankGearRight.hpp"

std::vector<std::string> TankGearLeftAnimName;
std::vector<std::string> TankGearMiddleAnimName;
std::vector<std::string> TankGearRightAnimName;

static int TANK_GEAR_IMAGE_WIDTH = 96;
static int TANK_GEAR_WIDTH = 32;
static int TANK_GEAR_HEIGHT = 32;

void CustomBlockInit() {
	for (int i = 0; i < TANK_GEAR_IMAGE_WIDTH / TANK_GEAR_WIDTH; i++) {
		ImageManager::AddTexture(fmt::format("TankGearLeft_{}", i), "data/resources/TankGearLeft.png", sf::IntRect({i * TANK_GEAR_WIDTH, 0}, {TANK_GEAR_WIDTH, TANK_GEAR_HEIGHT}));
		TankGearLeftAnimName.push_back(fmt::format("TankGearLeft_{}", i));
    	ImageManager::AddTexture(fmt::format("TankGearMiddle_{}", i), "data/resources/TankGearMiddle.png", sf::IntRect({i * TANK_GEAR_WIDTH, 0}, {TANK_GEAR_WIDTH, TANK_GEAR_HEIGHT}));
    	TankGearMiddleAnimName.push_back(fmt::format("TankGearMiddle_{}", i));
    	ImageManager::AddTexture(fmt::format("TankGearRight_{}", i), "data/resources/TankGearRight.png", sf::IntRect({i * TANK_GEAR_WIDTH, 0}, {TANK_GEAR_WIDTH, TANK_GEAR_HEIGHT}));
    	TankGearRightAnimName.push_back(fmt::format("TankGearRight_{}", i));
	}
}
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