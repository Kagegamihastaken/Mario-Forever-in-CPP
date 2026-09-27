#include "Core/Loading/SpikeLoading.hpp"

#include "Core/Scene/GameScene.hpp"
#include "Object/Enemy/CastleSpikeUp.hpp"
#include "Object/Enemy/GreenPiranhaGround.hpp"
#include "Object/Enemy/TankSpikeHorizon.hpp"

void MFCPP::AddSpike(const SpikeID ID, const float x, const float y) {
	switch (ID) {
		case SpikeID::PIRANHA_GROUND:
			GameScene::enemyManager.addEnemy<GreenPiranhaGround>(sf::Vector2f(x, y));
			break;
		case SpikeID::CASTLE_SPIKE_UP:
			GameScene::enemyManager.addEnemy<CastleSpikeUp>(sf::Vector2f(x, y));
			break;
		case SpikeID::TANK_SPIKE_LEFT:
			GameScene::enemyManager.addEnemy<TankSpikeHorizon>(sf::Vector2f(x, y), false);
			break;
		case SpikeID::TANK_SPIKE_RIGHT:
			GameScene::enemyManager.addEnemy<TankSpikeHorizon>(sf::Vector2f(x, y), true);
			break;
		default: ;
	}
}
