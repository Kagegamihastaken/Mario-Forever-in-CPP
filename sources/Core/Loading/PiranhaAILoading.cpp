#include "Core/Loading/PiranhaAILoading.hpp"

#include "Core/Enumeration/enum.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Enemy/GreenPiranhaDown.hpp"
#include "Object/Enemy/GreenPiranhaLeft.hpp"
#include "Object/Enemy/GreenPiranhaRight.hpp"
#include "Object/Enemy/GreenPiranhaUp.hpp"
#include "Object/Enemy/PinkPiranhaDown.hpp"
#include "Object/Enemy/PinkPiranhaLeft.hpp"
#include "Object/Enemy/PinkPiranhaRight.hpp"
#include "Object/Enemy/PinkPiranhaUp.hpp"

void MFCPP::AddPiranha(const PiranhaID ID, const PiranhaDirection dir, const float x, const float y) {
	switch (ID) {
		case PiranhaID::GREEN:
			switch (dir) {
			case PiranhaDirection::UP:
				GameScene::enemyManager.addEnemy<GreenPiranhaUp>(sf::Vector2f(x, y));
				break;
			case PiranhaDirection::DOWN:
				GameScene::enemyManager.addEnemy<GreenPiranhaDown>(sf::Vector2f(x, y));
				break;
			case PiranhaDirection::RIGHT:
				GameScene::enemyManager.addEnemy<GreenPiranhaRight>(sf::Vector2f(x, y));
				break;
			case PiranhaDirection::LEFT:
				GameScene::enemyManager.addEnemy<GreenPiranhaLeft>(sf::Vector2f(x, y));
				break;
			default: ;
			}
			break;
		case PiranhaID::PINK:
			switch (dir) {
			case PiranhaDirection::UP:
				GameScene::enemyManager.addEnemy<PinkPiranhaUp>(sf::Vector2f(x, y));
				break;
			case PiranhaDirection::DOWN:
				GameScene::enemyManager.addEnemy<PinkPiranhaDown>(sf::Vector2f(x, y));
				break;
			case PiranhaDirection::RIGHT:
				GameScene::enemyManager.addEnemy<PinkPiranhaRight>(sf::Vector2f(x, y));
				break;
			case PiranhaDirection::LEFT:
				GameScene::enemyManager.addEnemy<PinkPiranhaLeft>(sf::Vector2f(x, y));
				break;
			default: ;
			}
			break;
		default: ;
	}
}