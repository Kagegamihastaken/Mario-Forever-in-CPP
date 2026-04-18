#include "Effect/CoinEffect.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Effect/NormalCoinEffect.hpp"

void CoinEffectInit() {
	for (int i = 0; i < COINEFFECT_IMAGE_WIDTH / COINEFFECT_WIDTH; ++i)
		ImageManager::AddTexture(fmt::format("CoinEffect_{}", i), "data/resources/CoinEffect.png", sf::IntRect({i * COINEFFECT_WIDTH, 0}, {COINEFFECT_WIDTH, COINEFFECT_HEIGHT}));
}
void AddCoinEffect(const CoinID ID, const CoinAtt att, const float x, const float y) {
	GameScene::effectManager.addEffect<NormalCoinEffect>(sf::Vector2f(x, y));
}