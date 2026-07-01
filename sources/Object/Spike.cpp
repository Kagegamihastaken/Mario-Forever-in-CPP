#include "Object/Spike.hpp"

#include "Core/AnimationSequenceManager.hpp"
#include "Core/ImageManager.hpp"
#include "../../headers/Core/Enumeration/enum.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Enemy/CastleSpikeUp.hpp"
#include "Object/Enemy/GreenPiranhaGround.hpp"

void SpikeInit() {
	ImageManager::AddTexture("CastleSpikeUp", "data/resources/Spike/Spike.png");
	MFCPP::AnimationSequenceManager::newData("PiranhaGroundAnimName");
	for (int i = 0; i < 4; ++i) {
		ImageManager::AddTexture(fmt::format("PiranhaGreenGround_{}", i), "data/resources/Spike/PiranhaGround.png", sf::IntRect({i * 32, 0}, {32, 32}));
		MFCPP::AnimationSequenceManager::addSingleFrame("PiranhaGroundAnimName", fmt::format("PiranhaGreenGround_{}", i));
	}
}
void AddSpike(const SpikeID ID, const float x, const float y) {
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