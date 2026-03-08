#include "Object/Spike.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Enemy/CastleSpikeUp.hpp"
#include "Object/Enemy/GreenPiranhaGround.hpp"

std::vector<std::string> PiranhaGroundAnimName;
void SpikeInit() {
	ImageManager::AddTexture("CastleSpikeUp", "data/resources/Spike/Spike.png");
	for (int i = 0; i < 4; ++i) {
		ImageManager::AddTexture(fmt::format("PiranhaGreenGround_{}", i), "data/resources/Spike/PiranhaGround.png", sf::IntRect({i * 32, 0}, {32, 32}));
		PiranhaGroundAnimName.emplace_back(fmt::format("PiranhaGreenGround_{}", i));
	}
}
void AddSpike(const SpikeID ID, const float x, const float y) {
	switch (ID) {
		case PIRANHA_GROUND:
			GameScene::enemyManager.addEnemy<GreenPiranhaGround>(sf::Vector2f(x, y));
			break;
		case SPIKE_NORMAL:
			GameScene::enemyManager.addEnemy<CastleSpikeUp>(sf::Vector2f(x, y));
			break;
		default: ;
	}
}