#include "Object/Coin.hpp"
#include "Core/WindowFrame.hpp"
#include "Object/Mario.hpp"
#include "Core/Scroll.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Sound.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Effect/MarioEffect.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Tilemap.hpp"

plf::colony<MFCPP::Coin> CoinList;
static bool CoinDeleteGate = false;
SingleAnimationObject CoinAnimation;
int CoinCount = 0;
static std::vector<std::string> CoinAnimName;
static constexpr int COIN_IMAGE_WIDTH = 96;
static constexpr int COIN_WIDTH_HEIGHT = 32;
void CoinInit() {
	for (int i = 0; i < COIN_IMAGE_WIDTH / COIN_WIDTH_HEIGHT; ++i) {
		ImageManager::PreloadTexture(fmt::format("Coin_{}", i), "data/resources/Coin.png", sf::IntRect({COIN_WIDTH_HEIGHT * i, 0}, {COIN_WIDTH_HEIGHT, COIN_WIDTH_HEIGHT}));
		CoinAnimName.emplace_back(fmt::format("Coin_{}", i));
	}
	CoinAnimation.setAnimation(0, 2, 20);
	CoinAnimation.setAnimationSequence(CoinAnimName);
}
void AddCoin(const CoinID ID, const CoinAtt att, const float x, const float y) {
	auto it = CoinList.emplace(ID, att, sf::FloatRect({6.f, 2.f}, {19.f, 28.f}), sf::Vector2f(x, y), sf::Vector2f(0.f, 0.f));
}
void DeleteIndexCoin(const plf::colony<MFCPP::Coin>::colony_iterator<false>& index) {
	index->setDestroyed(true);
	CoinDeleteGate = true;
}
void ForceLoadCoinTexture() {
	for (int i = 0; i < COIN_IMAGE_WIDTH / COIN_WIDTH_HEIGHT; ++i)
		if (!ImageManager::isExist(fmt::format("Coin_{}", i)))
			ImageManager::LoadTexture(fmt::format("Coin_{}", i));
}
void DeleteCoin(const float x, const float y) {
	for (auto it = CoinList.begin(); it != CoinList.end(); ++it)
		if (it->getCurrentPosition().x == x && it->getCurrentPosition().y == y)
			DeleteIndexCoin(it);
}
void DeleteAllCoin() {
	CoinList.clear();
}
void CoinOnTouch() {
	if (CoinList.empty() || EffectActive) return;
	std::vector<sf::Vector2f> CoinPos{};
	CoinPos = isAccurateCollideMainCollectable(MFCPP::CollisionObject(player.curr, player.property.getOrigin(), player.hitboxMain), 0, sf::FloatRect({ 6, 2 }, { 19, 28 }));
	for (const auto &i : CoinPos) {
		MFCPP::setIndexCollectableMapCollision(i.x, i.y, false);
		Score += 200;
		SoundManager::PlaySound("Coin");
		++CoinCount;
		DeleteCoin(i.x, i.y);
	}
}
void CoinDraw() {
	if (CoinCount > 99) {
		CoinCount = 0;
		AddScoreEffect(SCORE_1UP, player.property.getPosition().x, player.property.getPosition().y);
	}
	for (auto &i : CoinList) {
		if (i.isDestroyed()) continue;

		if (isOutScreen(i.getInterpolatedPosition().x, i.getInterpolatedPosition().y, 32, 32)) continue;
		CoinAnimation.AnimationUpdate(i.getInterpolatedPosition(), i.getOrigin());
		CoinAnimation.AnimationDraw();
	}
}
void CoinCleanup() {
	if (!CoinDeleteGate) return;
	auto it = CoinList.begin();
	while (it != CoinList.end()) {
		if (!it->isDestroyed()) ++it;
		else it = CoinList.erase(it);
	}
	CoinDeleteGate = false;
}