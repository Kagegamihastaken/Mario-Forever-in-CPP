#include "Object/PiranhaAI.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Collision/Collide.hpp"
#include "Object/Mario.hpp"
#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Interpolation.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Class/PiranhaAIClass.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Enemy/GreenPiranhaDown.hpp"
#include "Object/Enemy/GreenPiranhaLeft.hpp"
#include "Object/Enemy/GreenPiranhaRight.hpp"
#include "Object/Enemy/GreenPiranhaUp.hpp"
#include "Object/Enemy/PinkPiranhaDown.hpp"
#include "Object/Enemy/PinkPiranhaLeft.hpp"
#include "Object/Enemy/PinkPiranhaRight.hpp"
#include "Object/Enemy/PinkPiranhaUp.hpp"
#include "Projectiles/PiranhaProjectile.hpp"
std::vector<std::string> GreenPiranhaUpAnimName;
std::vector<std::string> GreenPiranhaDownAnimName;
std::vector<std::string> GreenPiranhaRightAnimName;
std::vector<std::string> GreenPiranhaLeftAnimName;

std::vector<std::string> PinkPiranhaUpAnimName;
std::vector<std::string> PinkPiranhaDownAnimName;
std::vector<std::string> PinkPiranhaRightAnimName;
std::vector<std::string> PinkPiranhaLeftAnimName;
static constexpr int PIRANHA_IMAGE_WIDTH = 256;
static constexpr int PIRANHA_FIRE_IMAGE_WIDTH = 128;
static constexpr int PIRANHA_WIDTH = 64;
static constexpr int PIRANHA_HEIGHT = 64;
void PiranhaAIInit() {
	for (int i = 0; i < PIRANHA_IMAGE_WIDTH / PIRANHA_WIDTH; ++i) {
		ImageManager::AddTexture(fmt::format("GreenPiranhaUp_{}", i), "data/resources/Piranha/GreenPiranhaUp.png", sf::IntRect({i * PIRANHA_WIDTH, 0}, {PIRANHA_WIDTH, PIRANHA_HEIGHT}));
		GreenPiranhaUpAnimName.emplace_back(fmt::format("GreenPiranhaUp_{}", i));
		ImageManager::AddTexture(fmt::format("GreenPiranhaDown_{}", i), "data/resources/Piranha/GreenPiranhaDown.png", sf::IntRect({i * PIRANHA_WIDTH, 0}, {PIRANHA_WIDTH, PIRANHA_HEIGHT}));
		GreenPiranhaDownAnimName.emplace_back(fmt::format("GreenPiranhaDown_{}", i));
		ImageManager::AddTexture(fmt::format("GreenPiranhaRight_{}", i), "data/resources/Piranha/GreenPiranhaRight.png", sf::IntRect({i * PIRANHA_WIDTH, 0}, {PIRANHA_WIDTH, PIRANHA_HEIGHT}));
		GreenPiranhaRightAnimName.emplace_back(fmt::format("GreenPiranhaRight_{}", i));
		ImageManager::AddTexture(fmt::format("GreenPiranhaLeft_{}", i), "data/resources/Piranha/GreenPiranhaLeft.png", sf::IntRect({i * PIRANHA_WIDTH, 0}, {PIRANHA_WIDTH, PIRANHA_HEIGHT}));
		GreenPiranhaLeftAnimName.emplace_back(fmt::format("GreenPiranhaLeft_{}", i));
	}
	for (int i = 0; i < PIRANHA_FIRE_IMAGE_WIDTH / PIRANHA_WIDTH; ++i) {
		ImageManager::AddTexture(fmt::format("PinkPiranhaUp_{}", i), "data/resources/Piranha/PinkPiranhaUp.png", sf::IntRect({i * PIRANHA_WIDTH, 0}, {PIRANHA_WIDTH, PIRANHA_HEIGHT}));
		PinkPiranhaUpAnimName.emplace_back(fmt::format("PinkPiranhaUp_{}", i));
		ImageManager::AddTexture(fmt::format("PinkPiranhaDown_{}", i), "data/resources/Piranha/PinkPiranhaDown.png", sf::IntRect({i * PIRANHA_WIDTH, 0}, {PIRANHA_WIDTH, PIRANHA_HEIGHT}));
		PinkPiranhaDownAnimName.emplace_back(fmt::format("PinkPiranhaDown_{}", i));
		ImageManager::AddTexture(fmt::format("PinkPiranhaRight_{}", i), "data/resources/Piranha/PinkPiranhaRight.png", sf::IntRect({i * PIRANHA_WIDTH, 0}, {PIRANHA_WIDTH, PIRANHA_HEIGHT}));
		PinkPiranhaRightAnimName.emplace_back(fmt::format("PinkPiranhaRight_{}", i));
		ImageManager::AddTexture(fmt::format("PinkPiranhaLeft_{}", i), "data/resources/Piranha/PinkPiranhaLeft.png", sf::IntRect({i * PIRANHA_WIDTH, 0}, {PIRANHA_WIDTH, PIRANHA_HEIGHT}));
		PinkPiranhaLeftAnimName.emplace_back(fmt::format("PinkPiranhaLeft_{}", i));
	}
}
void AddPiranha(const PiranhaID ID, const PiranhaDirection dir, const float x, const float y) {
	switch (ID) {
		case GREEN:
			switch (dir) {
			case PIRANHA_UP:
				GameScene::enemyManager.addEnemy<GreenPiranhaUp>(sf::Vector2f(x, y));
				// PiranhaAIList.emplace_back(ID, PIRANHA_UP, 1.f, 70.f, 0, 10.f, sf::FloatRect({ 16, 17 }, { 31, 47 }), sf::Vector2f(x, y), sf::Vector2f(32, 63));
				// PiranhaAIList.back().setAnimation(0, 3, 24);
				// PiranhaAIList.back().setAnimationSequence(GreenPiranhaUpAnimName);
				break;
			case PIRANHA_DOWN:
				GameScene::enemyManager.addEnemy<GreenPiranhaDown>(sf::Vector2f(x, y));
				// PiranhaAIList.emplace_back(ID, PIRANHA_DOWN, 1.f, 70.f, 0, 10.f, sf::FloatRect({ 16, 0 }, { 31, 47 }), sf::Vector2f(x, y), sf::Vector2f(32, 0));
				// PiranhaAIList.back().setAnimation(0, 3, 24);
				// PiranhaAIList.back().setAnimationSequence(GreenPiranhaDownAnimName);
				break;
			case PIRANHA_RIGHT:
				GameScene::enemyManager.addEnemy<GreenPiranhaRight>(sf::Vector2f(x, y));
				// PiranhaAIList.emplace_back(ID, PIRANHA_RIGHT, 1.f, 70.f, 0, 10.f, sf::FloatRect({ 17, 17 }, { 47, 31 }), sf::Vector2f(x, y), sf::Vector2f(63, 32));
				// PiranhaAIList.back().setAnimation(0, 3, 24);
				// PiranhaAIList.back().setAnimationSequence(GreenPiranhaRightAnimName);
				break;
			case PIRANHA_LEFT:
				GameScene::enemyManager.addEnemy<GreenPiranhaLeft>(sf::Vector2f(x, y));
				// PiranhaAIList.emplace_back(ID, PIRANHA_LEFT, 1.f, 70.f, 0, 10.f, sf::FloatRect({ 0, 17 }, { 47, 31 }), sf::Vector2f(x, y), sf::Vector2f(0, 32));
				// PiranhaAIList.back().setAnimation(0, 3, 24);
				// PiranhaAIList.back().setAnimationSequence(GreenPiranhaLeftAnimName);
				break;
			default: ;
			}
			break;
		case PINK:
			switch (dir) {
			case PIRANHA_UP:
				GameScene::enemyManager.addEnemy<PinkPiranhaUp>(sf::Vector2f(x, y));
					// PiranhaAIList.emplace_back(ID, PIRANHA_UP, 1.f, 70.f, 3, 10.f, sf::FloatRect({ 16, 17 }, { 31, 47 }), sf::Vector2f(x, y), sf::Vector2f(32, 63));
					// PiranhaAIList.back().setAnimation(0, 1, 24);
					// PiranhaAIList.back().setAnimationSequence(PinkPiranhaUpAnimName);
				break;
			case PIRANHA_DOWN:
				GameScene::enemyManager.addEnemy<PinkPiranhaDown>(sf::Vector2f(x, y));
					// PiranhaAIList.emplace_back(ID, PIRANHA_DOWN, 1.f, 70.f, 3, 10.f, sf::FloatRect({ 16, 0 }, { 31, 47 }), sf::Vector2f(x, y), sf::Vector2f(32, 0));
					// PiranhaAIList.back().setAnimation(0, 1, 24);
					// PiranhaAIList.back().setAnimationSequence(PinkPiranhaDownAnimName);
				break;
			case PIRANHA_RIGHT:
				GameScene::enemyManager.addEnemy<PinkPiranhaRight>(sf::Vector2f(x, y));
					// PiranhaAIList.emplace_back(ID, PIRANHA_RIGHT, 1.f, 70.f, 3, 10.f, sf::FloatRect({ 17, 17 }, { 47, 31 }), sf::Vector2f(x, y), sf::Vector2f(63, 32));
					// PiranhaAIList.back().setAnimation(0, 1, 24);
					// PiranhaAIList.back().setAnimationSequence(PinkPiranhaRightAnimName);
				break;
			case PIRANHA_LEFT:
				GameScene::enemyManager.addEnemy<PinkPiranhaLeft>(sf::Vector2f(x, y));
					// PiranhaAIList.emplace_back(ID, PIRANHA_LEFT, 1.f, 70.f, 3, 10.f, sf::FloatRect({ 0, 17 }, { 47, 31 }), sf::Vector2f(x, y), sf::Vector2f(0, 32));
					// PiranhaAIList.back().setAnimation(0, 1, 24);
					// PiranhaAIList.back().setAnimationSequence(PinkPiranhaLeftAnimName);
				break;
			default: ;
			}
			break;
		default: ;
	}
}