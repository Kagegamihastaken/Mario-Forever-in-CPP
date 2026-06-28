#include "Effect/FireballExplosion.hpp"

#include "Core/AnimationSequenceManager.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Effect/FireballExplosionEffect.hpp"

void FireballExplosionInit() {
	MFCPP::AnimationSequenceManager::newData("FireballExplosionAnimName");
	for (int i = 0; i < FIREBALLEXPLOSION_IMAGE_WIDTH / FIREBALLEXPLOSION_WIDTH; ++i) {
		ImageManager::AddTexture(fmt::format("FireballExplosion_{}", i), "data/resources/FireballExplosion.png", sf::IntRect({i * FIREBALLEXPLOSION_WIDTH, 0}, {FIREBALLEXPLOSION_WIDTH, FIREBALLEXPLOSION_HEIGHT}));
		MFCPP::AnimationSequenceManager::addSingleFrame("FireballExplosionAnimName", fmt::format("FireballExplosion_{}", i));
	}
}
void AddFireballExplosion(const float x, const float y) {
	GameScene::effectManager.addEffect<FireballExplosionEffect>(sf::Vector2f(x, y));
}