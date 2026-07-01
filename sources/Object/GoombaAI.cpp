#include "Object/GoombaAI.hpp"

#include "Core/AnimationSequenceManager.hpp"
#include "../../headers/Core/Enumeration/enum.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Logging.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Bonus/FireFlower.hpp"
#include "Object/Enemy/Goomba.hpp"
#include "Object/Enemy/GreenKoopa.hpp"
#include "Object/Bonus/GreenMushroom.hpp"
#include "Object/Enemy/GreenSpiny.hpp"
#include "Object/Bonus/Mushroom.hpp"
#include "Object/Enemy/GreenKoopaParatroopa.hpp"
#include "Object/Enemy/RedKoopa.hpp"
#include "Object/Enemy/RedSpiny.hpp"

static constexpr uint16_t GOOMBA_IMAGE_WIDTH = 62;
static constexpr uint16_t GOOMBA_WIDTH = 31;
static constexpr uint16_t GOOMBA_HEIGHT = 32;

static constexpr uint16_t KOOPA_IMAGE_WIDTH = 64;
static constexpr uint16_t KOOPA_WIDTH = 32;
static constexpr uint16_t KOOPA_HEIGHT = 47;

static constexpr uint16_t KOOPA_PARATROOPA_IMAGE_WIDTH = 66;
static constexpr uint16_t KOOPA_PARATROOPA_WIDTH = 33;
static constexpr uint16_t KOOPA_PARATROOPA_HEIGHT = 47;

static constexpr uint16_t KOOPA_SHELL_IMAGE_WIDTH = 132;
static constexpr uint16_t KOOPA_SHELL_WIDTH = 33;
static constexpr uint16_t KOOPA_SHELL_HEIGHT = 28;

static constexpr uint16_t SPINY_IMAGE_WIDTH = 66;
static constexpr uint16_t SPINY_WIDTH = 33;
static constexpr uint16_t SPINY_HEIGHT = 32;

static constexpr uint16_t FIRE_FLOWER_IMAGE_WIDTH = 128;
static constexpr uint16_t FIRE_FLOWER_WIDTH = 32;
static constexpr uint16_t FIRE_FLOWER_HEIGHT = 32;

static constexpr uint16_t BEETROOT_IMAGE_WIDTH = 128;
static constexpr uint16_t BEETROOT_WIDTH = 32;
static constexpr uint16_t BEETROOT_HEIGHT = 33;

static constexpr uint16_t THWOMP_WIDTH = 64;
static constexpr uint16_t THWOMP_HEIGHT = 66;

void GoombaAIInit() {
	MFCPP::AnimationSequenceManager::newData("GoombaAnimName");
	for (int i = 0; i < GOOMBA_IMAGE_WIDTH / GOOMBA_WIDTH; i++) {
		ImageManager::AddTexture(fmt::format("Goomba_{}", i), "data/resources/Goomba/Goomba.png", sf::IntRect({i * GOOMBA_WIDTH, 0}, {GOOMBA_WIDTH, GOOMBA_HEIGHT}));
		MFCPP::AnimationSequenceManager::addSingleFrame("GoombaAnimName", fmt::format("Goomba_{}", i));
	}
	ImageManager::AddTexture("Mushroom_0", "data/resources/Mushroom.png");
	ImageManager::AddTexture("GreenMushroom_0", "data/resources/Mushroom.png");
	MFCPP::AnimationSequenceManager::newData("GreenKoopaAnimName");
	MFCPP::AnimationSequenceManager::newData("RedKoopaAnimName");
	for (int i = 0; i < KOOPA_IMAGE_WIDTH / KOOPA_WIDTH; ++i) {
		ImageManager::AddTexture(fmt::format("GreenKoopa_{}", i), "data/resources/Koopa/GreenKoopa.png", sf::IntRect({i * KOOPA_WIDTH, 0}, {KOOPA_WIDTH, KOOPA_HEIGHT}));
		MFCPP::AnimationSequenceManager::addSingleFrame("GreenKoopaAnimName", fmt::format("GreenKoopa_{}", i));
		ImageManager::AddTexture(fmt::format("RedKoopa_{}", i), "data/resources/Koopa/RedKoopa.png", sf::IntRect({i * KOOPA_WIDTH, 0}, {KOOPA_WIDTH, KOOPA_HEIGHT}));
		MFCPP::AnimationSequenceManager::addSingleFrame("RedKoopaAnimName", fmt::format("RedKoopa_{}", i));
	}
	MFCPP::AnimationSequenceManager::newData("GreenKoopaParatroopaAnimName");
	for (int i = 0; i < KOOPA_PARATROOPA_IMAGE_WIDTH / KOOPA_PARATROOPA_WIDTH; ++i) {
		ImageManager::AddTexture(fmt::format("GreenKoopaParatroopa_{}", i), "data/resources/Koopa/GreenKoopaParatroopa.png", sf::IntRect({i * KOOPA_PARATROOPA_WIDTH, 0}, {KOOPA_PARATROOPA_WIDTH, KOOPA_PARATROOPA_HEIGHT}));
		MFCPP::AnimationSequenceManager::addSingleFrame("GreenKoopaParatroopaAnimName", fmt::format("GreenKoopaParatroopa_{}", i));
	}
	MFCPP::AnimationSequenceManager::newData("GreenKoopaShellAnimName");
	MFCPP::AnimationSequenceManager::newData("RedKoopaShellAnimName");
	for (int i = 0; i < KOOPA_SHELL_IMAGE_WIDTH / KOOPA_SHELL_WIDTH; ++i) {
		ImageManager::AddTexture(fmt::format("GreenKoopaShell_{}", i), "data/resources/Koopa/GreenKoopaShell.png", sf::IntRect({i * KOOPA_SHELL_WIDTH, 0}, {KOOPA_SHELL_WIDTH, KOOPA_SHELL_HEIGHT}));
		MFCPP::AnimationSequenceManager::addSingleFrame("GreenKoopaShellAnimName", fmt::format("GreenKoopaShell_{}", i));
		ImageManager::AddTexture(fmt::format("RedKoopaShell_{}", i), "data/resources/Koopa/RedKoopaShell.png", sf::IntRect({i * KOOPA_SHELL_WIDTH, 0}, {KOOPA_SHELL_WIDTH, KOOPA_SHELL_HEIGHT}));
		MFCPP::AnimationSequenceManager::addSingleFrame("RedKoopaShellAnimName", fmt::format("RedKoopaShell_{}", i));
	}
	MFCPP::AnimationSequenceManager::newData("RedSpinyAnimName");
	MFCPP::AnimationSequenceManager::newData("GreenSpinyAnimName");
	for (int i = 0; i < SPINY_IMAGE_WIDTH / SPINY_WIDTH; ++i) {
		ImageManager::AddTexture(fmt::format("RedSpiny_{}", i), "data/resources/Spiny/RedSpiny.png", sf::IntRect({i * SPINY_WIDTH, 0}, {SPINY_WIDTH, SPINY_HEIGHT}));
		MFCPP::AnimationSequenceManager::addSingleFrame("RedSpinyAnimName", fmt::format("RedSpiny_{}", i));
		ImageManager::AddTexture(fmt::format("GreenSpiny_{}", i), "data/resources/Spiny/GreenSpiny.png", sf::IntRect({i * SPINY_WIDTH, 0}, {SPINY_WIDTH, SPINY_HEIGHT}));
		MFCPP::AnimationSequenceManager::addSingleFrame("GreenSpinyAnimName", fmt::format("GreenSpiny_{}", i));
	}
	MFCPP::AnimationSequenceManager::newData("FireFlowerAnimName");
	for (int i = 0; i < FIRE_FLOWER_IMAGE_WIDTH / FIRE_FLOWER_WIDTH; ++i) {
		ImageManager::AddTexture(fmt::format("FireFlower_{}", i), "data/resources/FireFlower.png", sf::IntRect({i * FIRE_FLOWER_WIDTH, 0}, {FIRE_FLOWER_WIDTH, FIRE_FLOWER_HEIGHT}));
		MFCPP::AnimationSequenceManager::addSingleFrame("FireFlowerAnimName", fmt::format("FireFlower_{}", i));
	}
	MFCPP::AnimationSequenceManager::newData("BeetrootAnimName");
	for (int i = 0; i < BEETROOT_IMAGE_WIDTH / BEETROOT_WIDTH; ++i) {
		ImageManager::AddTexture(fmt::format("Beetroot_{}", i), "data/resources/Beetroot.png", sf::IntRect({i * BEETROOT_WIDTH, 0}, {BEETROOT_WIDTH, BEETROOT_HEIGHT}));
		MFCPP::AnimationSequenceManager::addSingleFrame("BeetrootAnimName", fmt::format("Beetroot_{}", i));
	}
}
void AddGoombaAI(GoombaAIType type, int SkinID, const float x, const float y, const GoombaAIDirection Dir = GoombaAIDirection::LEFT) {
	switch (type) {
		case GoombaAIType::GOOMBA:
			GameScene::enemyManager.addEnemy<Goomba>(sf::Vector2f(x, y));
			break;
		case GoombaAIType::KOOPA:
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
		case GoombaAIType::MUSHROOM:
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
		case GoombaAIType::SHELL:
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
		case GoombaAIType::SPINY:
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
		case GoombaAIType::FIRE_FLOWER:
			GameScene::enemyManager.addEnemy<FireFlower>(sf::Vector2f(x, y));
			break;
		case GoombaAIType::KOOPA_PARATROOPA:
			GameScene::enemyManager.addEnemy<GreenKoopaParatroopa>(sf::Vector2f(x, y), 0.f);
			break;
		default: ;
	}
}