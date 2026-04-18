#include "Effect/FireballExplosion.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Effect/FireballExplosionEffect.hpp"

std::vector<std::string> FireballExplosionAnimName;
void FireballExplosionInit() {
	for (int i = 0; i < FIREBALLEXPLOSION_IMAGE_WIDTH / FIREBALLEXPLOSION_WIDTH; ++i) {
		ImageManager::AddTexture(fmt::format("FireballExplosion_{}", i), "data/resources/FireballExplosion.png", sf::IntRect({i * FIREBALLEXPLOSION_WIDTH, 0}, {FIREBALLEXPLOSION_WIDTH, FIREBALLEXPLOSION_HEIGHT}));
		FireballExplosionAnimName.push_back(fmt::format("FireballExplosion_{}", i));
	}
}
void AddFireballExplosion(const float x, const float y) {
	GameScene::effectManager.addEffect<FireballExplosionEffect>(sf::Vector2f(x, y));
}