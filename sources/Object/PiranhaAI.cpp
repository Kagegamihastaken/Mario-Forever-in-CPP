#include "Object/PiranhaAI.hpp"

#include "Core/AnimationSequenceManager.hpp"
#include "Core/ImageManager.hpp"
#include "../../headers/Core/Enumeration/enum.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Enemy/GreenPiranhaDown.hpp"
#include "Object/Enemy/GreenPiranhaLeft.hpp"
#include "Object/Enemy/GreenPiranhaRight.hpp"
#include "Object/Enemy/GreenPiranhaUp.hpp"
#include "Object/Enemy/PinkPiranhaDown.hpp"
#include "Object/Enemy/PinkPiranhaLeft.hpp"
#include "Object/Enemy/PinkPiranhaRight.hpp"
#include "Object/Enemy/PinkPiranhaUp.hpp"
static constexpr int PIRANHA_IMAGE_WIDTH = 256;
static constexpr int PIRANHA_FIRE_IMAGE_WIDTH = 128;
static constexpr int PIRANHA_WIDTH = 64;
static constexpr int PIRANHA_HEIGHT = 64;
void PiranhaAIInit() {
	MFCPP::AnimationSequenceManager::newData("GreenPiranhaUpAnimName");
	MFCPP::AnimationSequenceManager::newData("GreenPiranhaDownAnimName");
	MFCPP::AnimationSequenceManager::newData("GreenPiranhaRightAnimName");
	MFCPP::AnimationSequenceManager::newData("GreenPiranhaLeftAnimName");
	for (int i = 0; i < PIRANHA_IMAGE_WIDTH / PIRANHA_WIDTH; ++i) {
		ImageManager::AddTexture(fmt::format("GreenPiranhaUp_{}", i), "data/resources/Piranha/GreenPiranhaUp.png", sf::IntRect({i * PIRANHA_WIDTH, 0}, {PIRANHA_WIDTH, PIRANHA_HEIGHT}));
		MFCPP::AnimationSequenceManager::addSingleFrame("GreenPiranhaUpAnimName", fmt::format("GreenPiranhaUp_{}", i));
		ImageManager::AddTexture(fmt::format("GreenPiranhaDown_{}", i), "data/resources/Piranha/GreenPiranhaDown.png", sf::IntRect({i * PIRANHA_WIDTH, 0}, {PIRANHA_WIDTH, PIRANHA_HEIGHT}));
		MFCPP::AnimationSequenceManager::addSingleFrame("GreenPiranhaDownAnimName", fmt::format("GreenPiranhaDown_{}", i));
		ImageManager::AddTexture(fmt::format("GreenPiranhaRight_{}", i), "data/resources/Piranha/GreenPiranhaRight.png", sf::IntRect({i * PIRANHA_WIDTH, 0}, {PIRANHA_WIDTH, PIRANHA_HEIGHT}));
		MFCPP::AnimationSequenceManager::addSingleFrame("GreenPiranhaRightAnimName", fmt::format("GreenPiranhaRight_{}", i));
		ImageManager::AddTexture(fmt::format("GreenPiranhaLeft_{}", i), "data/resources/Piranha/GreenPiranhaLeft.png", sf::IntRect({i * PIRANHA_WIDTH, 0}, {PIRANHA_WIDTH, PIRANHA_HEIGHT}));
		MFCPP::AnimationSequenceManager::addSingleFrame("GreenPiranhaLeftAnimName", fmt::format("GreenPiranhaLeft_{}", i));
	}
	MFCPP::AnimationSequenceManager::newData("PinkPiranhaUpAnimName");
	MFCPP::AnimationSequenceManager::newData("PinkPiranhaDownAnimName");
	MFCPP::AnimationSequenceManager::newData("PinkPiranhaRightAnimName");
	MFCPP::AnimationSequenceManager::newData("PinkPiranhaLeftAnimName");
	for (int i = 0; i < PIRANHA_FIRE_IMAGE_WIDTH / PIRANHA_WIDTH; ++i) {
		ImageManager::AddTexture(fmt::format("PinkPiranhaUp_{}", i), "data/resources/Piranha/PinkPiranhaUp.png", sf::IntRect({i * PIRANHA_WIDTH, 0}, {PIRANHA_WIDTH, PIRANHA_HEIGHT}));
		MFCPP::AnimationSequenceManager::addSingleFrame("PinkPiranhaUpAnimName", fmt::format("PinkPiranhaUp_{}", i));
		ImageManager::AddTexture(fmt::format("PinkPiranhaDown_{}", i), "data/resources/Piranha/PinkPiranhaDown.png", sf::IntRect({i * PIRANHA_WIDTH, 0}, {PIRANHA_WIDTH, PIRANHA_HEIGHT}));
		MFCPP::AnimationSequenceManager::addSingleFrame("PinkPiranhaDownAnimName", fmt::format("PinkPiranhaDown_{}", i));
		ImageManager::AddTexture(fmt::format("PinkPiranhaRight_{}", i), "data/resources/Piranha/PinkPiranhaRight.png", sf::IntRect({i * PIRANHA_WIDTH, 0}, {PIRANHA_WIDTH, PIRANHA_HEIGHT}));
		MFCPP::AnimationSequenceManager::addSingleFrame("PinkPiranhaRightAnimName", fmt::format("PinkPiranhaRight_{}", i));
		ImageManager::AddTexture(fmt::format("PinkPiranhaLeft_{}", i), "data/resources/Piranha/PinkPiranhaLeft.png", sf::IntRect({i * PIRANHA_WIDTH, 0}, {PIRANHA_WIDTH, PIRANHA_HEIGHT}));
		MFCPP::AnimationSequenceManager::addSingleFrame("PinkPiranhaLeftAnimName", fmt::format("PinkPiranhaLeft_{}", i));
	}
}
void AddPiranha(const PiranhaID ID, const PiranhaDirection dir, const float x, const float y) {
	switch (ID) {
		case PiranhaID::GREEN:
			switch (dir) {
			case PiranhaDirection::UP:
				GameScene::enemyManager.addEnemy<GreenPiranhaUp>(sf::Vector2f(x, y));
				// PiranhaAIList.emplace_back(ID, PIRANHA_UP, 1.f, 70.f, 0, 10.f, sf::FloatRect({ 16, 17 }, { 31, 47 }), sf::Vector2f(x, y), sf::Vector2f(32, 63));
				// PiranhaAIList.back().setAnimation(0, 3, 24);
				// PiranhaAIList.back().setAnimationSequence(GreenPiranhaUpAnimName);
				break;
			case PiranhaDirection::DOWN:
				GameScene::enemyManager.addEnemy<GreenPiranhaDown>(sf::Vector2f(x, y));
				// PiranhaAIList.emplace_back(ID, PIRANHA_DOWN, 1.f, 70.f, 0, 10.f, sf::FloatRect({ 16, 0 }, { 31, 47 }), sf::Vector2f(x, y), sf::Vector2f(32, 0));
				// PiranhaAIList.back().setAnimation(0, 3, 24);
				// PiranhaAIList.back().setAnimationSequence(GreenPiranhaDownAnimName);
				break;
			case PiranhaDirection::RIGHT:
				GameScene::enemyManager.addEnemy<GreenPiranhaRight>(sf::Vector2f(x, y));
				// PiranhaAIList.emplace_back(ID, PIRANHA_RIGHT, 1.f, 70.f, 0, 10.f, sf::FloatRect({ 17, 17 }, { 47, 31 }), sf::Vector2f(x, y), sf::Vector2f(63, 32));
				// PiranhaAIList.back().setAnimation(0, 3, 24);
				// PiranhaAIList.back().setAnimationSequence(GreenPiranhaRightAnimName);
				break;
			case PiranhaDirection::LEFT:
				GameScene::enemyManager.addEnemy<GreenPiranhaLeft>(sf::Vector2f(x, y));
				// PiranhaAIList.emplace_back(ID, PIRANHA_LEFT, 1.f, 70.f, 0, 10.f, sf::FloatRect({ 0, 17 }, { 47, 31 }), sf::Vector2f(x, y), sf::Vector2f(0, 32));
				// PiranhaAIList.back().setAnimation(0, 3, 24);
				// PiranhaAIList.back().setAnimationSequence(GreenPiranhaLeftAnimName);
				break;
			default: ;
			}
			break;
		case PiranhaID::PINK:
			switch (dir) {
			case PiranhaDirection::UP:
				GameScene::enemyManager.addEnemy<PinkPiranhaUp>(sf::Vector2f(x, y));
					// PiranhaAIList.emplace_back(ID, PIRANHA_UP, 1.f, 70.f, 3, 10.f, sf::FloatRect({ 16, 17 }, { 31, 47 }), sf::Vector2f(x, y), sf::Vector2f(32, 63));
					// PiranhaAIList.back().setAnimation(0, 1, 24);
					// PiranhaAIList.back().setAnimationSequence(PinkPiranhaUpAnimName);
				break;
			case PiranhaDirection::DOWN:
				GameScene::enemyManager.addEnemy<PinkPiranhaDown>(sf::Vector2f(x, y));
					// PiranhaAIList.emplace_back(ID, PIRANHA_DOWN, 1.f, 70.f, 3, 10.f, sf::FloatRect({ 16, 0 }, { 31, 47 }), sf::Vector2f(x, y), sf::Vector2f(32, 0));
					// PiranhaAIList.back().setAnimation(0, 1, 24);
					// PiranhaAIList.back().setAnimationSequence(PinkPiranhaDownAnimName);
				break;
			case PiranhaDirection::RIGHT:
				GameScene::enemyManager.addEnemy<PinkPiranhaRight>(sf::Vector2f(x, y));
					// PiranhaAIList.emplace_back(ID, PIRANHA_RIGHT, 1.f, 70.f, 3, 10.f, sf::FloatRect({ 17, 17 }, { 47, 31 }), sf::Vector2f(x, y), sf::Vector2f(63, 32));
					// PiranhaAIList.back().setAnimation(0, 1, 24);
					// PiranhaAIList.back().setAnimationSequence(PinkPiranhaRightAnimName);
				break;
			case PiranhaDirection::LEFT:
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