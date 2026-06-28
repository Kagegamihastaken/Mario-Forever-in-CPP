#include "Effect/CoinEffect.hpp"

#include "Core/AnimationSequenceManager.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Effect/NormalCoinEffect.hpp"
std::vector<std::string> CoinEffectAnimName = {
	"CoinEffect_0", "CoinEffect_1", "CoinEffect_2", "CoinEffect_2", "CoinEffect_2", "CoinEffect_1", "CoinEffect_3",
	"CoinEffect_0", "CoinEffect_1", "CoinEffect_2", "CoinEffect_2", "CoinEffect_2", "CoinEffect_1", "CoinEffect_3",
	"CoinEffect_4", "CoinEffect_4", "CoinEffect_5", "CoinEffect_5", "CoinEffect_6", "CoinEffect_6", "CoinEffect_7"
};
void CoinEffectInit() {
	for (int i = 0; i < COINEFFECT_IMAGE_WIDTH / COINEFFECT_WIDTH; ++i)
		ImageManager::AddTexture(fmt::format("CoinEffect_{}", i), "data/resources/CoinEffect.png", sf::IntRect({i * COINEFFECT_WIDTH, 0}, {COINEFFECT_WIDTH, COINEFFECT_HEIGHT}));
	MFCPP::AnimationSequenceManager::newData("CoinEffectAnimName");
	for (const auto &i : CoinEffectAnimName)
		MFCPP::AnimationSequenceManager::addSingleFrame("CoinEffectAnimName", i);
	CoinEffectAnimName.clear();
	CoinEffectAnimName.shrink_to_fit();
}
void AddCoinEffect(const CoinID ID, const CoinAtt att, const float x, const float y) {
	GameScene::effectManager.addEffect<NormalCoinEffect>(sf::Vector2f(x, y));
}