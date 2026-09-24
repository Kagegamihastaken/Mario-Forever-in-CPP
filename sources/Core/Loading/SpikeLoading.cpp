#include "Core/Loading/SpikeLoading.hpp"

#include "Core/Scene/GameScene.hpp"
#include "Object/Enemy/CastleSpikeUp.hpp"
#include "Object/Enemy/GreenPiranhaGround.hpp"

void MFCPP::AddSpike(const SpikeID ID, const float x, const float y) {
	switch (ID) {
		case SpikeID::PIRANHA_GROUND:
			GameScene::enemyManager.addEnemy<GreenPiranhaGround>(sf::Vector2f(x, y));
			break;
		case SpikeID::SPIKE_NORMAL:
			GameScene::enemyManager.addEnemy<CastleSpikeUp>(sf::Vector2f(x, y));
			break;
		default: ;
	}
}