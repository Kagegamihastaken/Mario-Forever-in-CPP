#include "Object/GoombaAI.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Logging.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Enemy/FireFlower.hpp"
#include "Object/Enemy/Goomba.hpp"
#include "Object/Enemy/GreenKoopa.hpp"
#include "Object/Enemy/GreenMushroom.hpp"
#include "Object/Enemy/GreenSpiny.hpp"
#include "Object/Enemy/Mushroom.hpp"
#include "Object/Enemy/RedKoopa.hpp"
#include "Object/Enemy/RedSpiny.hpp"

std::vector<std::string> GoombaAnimName;
static int GOOMBA_IMAGE_WIDTH = 62;
static int GOOMBA_WIDTH = 31;
static int GOOMBA_HEIGHT = 32;
static std::vector<std::string> MushroomAnimName; // Unused
static std::vector<std::string> GreenMushroomAnimName; // Unused
static int MUSHROOM_IMAGE_WIDTH = 31;
static int MUSHROOM_WIDTH = 31;
static int MUSHROOM_HEIGHT = 32;
std::vector<std::string> GreenKoopaAnimName;
std::vector<std::string> RedKoopaAnimName;
static int KOOPA_IMAGE_WIDTH = 64;
static int KOOPA_WIDTH = 32;
static int KOOPA_HEIGHT = 47;
std::vector<std::string> GreenKoopaShellAnimName;
std::vector<std::string> RedKoopaShellAnimName;
static int KOOPA_SHELL_IMAGE_WIDTH = 132;
static int KOOPA_SHELL_WIDTH = 33;
static int KOOPA_SHELL_HEIGHT = 28;
std::vector<std::string> RedSpinyAnimName;
std::vector<std::string> GreenSpinyAnimName;
static int SPINY_IMAGE_WIDTH = 66;
static int SPINY_WIDTH = 33;
static int SPINY_HEIGHT = 32;
std::vector<std::string> FireFlowerAnimName;
static int FIRE_FLOWER_IMAGE_WIDTH = 128;
static int FIRE_FLOWER_WIDTH = 32;
static int FIRE_FLOWER_HEIGHT = 32;

void GoombaAIInit() {
	for (int i = 0; i < GOOMBA_IMAGE_WIDTH / GOOMBA_WIDTH; i++) {
		ImageManager::AddTexture(fmt::format("Goomba_{}", i), "data/resources/Goomba/Goomba.png", sf::IntRect({i * GOOMBA_WIDTH, 0}, {GOOMBA_WIDTH, GOOMBA_HEIGHT}));
		GoombaAnimName.push_back(fmt::format("Goomba_{}", i));
	}
	for (int i = 0; i < MUSHROOM_IMAGE_WIDTH / MUSHROOM_WIDTH; i++) {
		ImageManager::AddTexture(fmt::format("Mushroom_{}", i), "data/resources/Mushroom.png", sf::IntRect({i * MUSHROOM_WIDTH, 0}, {MUSHROOM_WIDTH, MUSHROOM_HEIGHT}));
		MushroomAnimName.push_back(fmt::format("Mushroom_{}", i));
		ImageManager::AddTexture(fmt::format("GreenMushroom_{}", i), "data/resources/GreenMushroom.png", sf::IntRect({i * MUSHROOM_WIDTH, 0}, {MUSHROOM_WIDTH, MUSHROOM_HEIGHT}));
		GreenMushroomAnimName.push_back(fmt::format("GreenMushroom_{}", i));
	}
	for (int i = 0; i < KOOPA_IMAGE_WIDTH / KOOPA_WIDTH; ++i) {
		ImageManager::AddTexture(fmt::format("GreenKoopa_{}", i), "data/resources/Koopa/GreenKoopa.png", sf::IntRect({i * KOOPA_WIDTH, 0}, {KOOPA_WIDTH, KOOPA_HEIGHT}));
		GreenKoopaAnimName.push_back(fmt::format("GreenKoopa_{}", i));
		ImageManager::AddTexture(fmt::format("RedKoopa_{}", i), "data/resources/Koopa/RedKoopa.png", sf::IntRect({i * KOOPA_WIDTH, 0}, {KOOPA_WIDTH, KOOPA_HEIGHT}));
		RedKoopaAnimName.push_back(fmt::format("RedKoopa_{}", i));
	}
	for (int i = 0; i < KOOPA_SHELL_IMAGE_WIDTH / KOOPA_SHELL_WIDTH; ++i) {
		ImageManager::AddTexture(fmt::format("GreenKoopaShell_{}", i), "data/resources/Koopa/GreenKoopaShell.png", sf::IntRect({i * KOOPA_SHELL_WIDTH, 0}, {KOOPA_SHELL_WIDTH, KOOPA_SHELL_HEIGHT}));
		GreenKoopaShellAnimName.push_back(fmt::format("GreenKoopaShell_{}", i));
		ImageManager::AddTexture(fmt::format("RedKoopaShell_{}", i), "data/resources/Koopa/RedKoopaShell.png", sf::IntRect({i * KOOPA_SHELL_WIDTH, 0}, {KOOPA_SHELL_WIDTH, KOOPA_SHELL_HEIGHT}));
		RedKoopaShellAnimName.push_back(fmt::format("RedKoopaShell_{}", i));
	}
	for (int i = 0; i < SPINY_IMAGE_WIDTH / SPINY_WIDTH; ++i) {
		ImageManager::AddTexture(fmt::format("RedSpiny_{}", i), "data/resources/Spiny/RedSpiny.png", sf::IntRect({i * SPINY_WIDTH, 0}, {SPINY_WIDTH, SPINY_HEIGHT}));
		RedSpinyAnimName.push_back(fmt::format("RedSpiny_{}", i));
		ImageManager::AddTexture(fmt::format("GreenSpiny_{}", i), "data/resources/Spiny/GreenSpiny.png", sf::IntRect({i * SPINY_WIDTH, 0}, {SPINY_WIDTH, SPINY_HEIGHT}));
		GreenSpinyAnimName.push_back(fmt::format("GreenSpiny_{}", i));
	}
	for (int i = 0; i < FIRE_FLOWER_IMAGE_WIDTH / FIRE_FLOWER_WIDTH; ++i) {
		ImageManager::AddTexture(fmt::format("FireFlower_{}", i), "data/resources/FireFlower.png", sf::IntRect({i * FIRE_FLOWER_WIDTH, 0}, {FIRE_FLOWER_WIDTH, FIRE_FLOWER_HEIGHT}));
		FireFlowerAnimName.push_back(fmt::format("FireFlower_{}", i));
	}
}
void AddGoombaAI(GoombaAIType type, int SkinID, const float x, const float y, const GoombaAIDirection Dir = LEFT) {
	switch (type) {
		case GOOMBA:
			GameScene::enemyManager.addEnemy<Goomba>(sf::Vector2f(x, y));
			break;
		case KOOPA:
			switch (SkinID) {
			case 0:
				GameScene::enemyManager.addEnemy<GreenKoopa>(sf::Vector2f(x, y), false);
				break;
			case 1:
				GameScene::enemyManager.addEnemy<RedKoopa>(sf::Vector2f(x, y), false);
				break;
			default: ;
			}
			break;
		case MUSHROOM:
			switch (SkinID) {
			case 0:
				GameScene::enemyManager.addEnemy<Mushroom>(sf::Vector2f(x, y));
				break;
			case 1:
				GameScene::enemyManager.addEnemy<GreenMushroom>(sf::Vector2f(x, y));
				break;
			default: ;
			}
			break;
		case SHELL:
			switch (SkinID) {
			case 0:
				GameScene::enemyManager.addEnemy<GreenKoopa>(sf::Vector2f(x, y), true);
				break;
			case 1:
				GameScene::enemyManager.addEnemy<RedKoopa>(sf::Vector2f(x, y), true);
				break;
			default: ;
			}
			break;
		case SPINY:
			switch (SkinID) {
			case 0:
				GameScene::enemyManager.addEnemy<RedSpiny>(sf::Vector2f(x, y));
				break;
			case 1:
				GameScene::enemyManager.addEnemy<GreenSpiny>(sf::Vector2f(x, y));
				break;
			default: ;
			}
			break;
		case FIRE_FLOWER:
			GameScene::enemyManager.addEnemy<FireFlower>(sf::Vector2f(x, y));
			break;
		default: ;
	}
}