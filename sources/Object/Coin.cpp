#include "Object/Coin.hpp"

#include "Core/AnimationSequenceManager.hpp"
#include "Object/Mario.hpp"
#include "Core/Scroll.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Collision/Collide.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Effect/MarioEffect.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Logging.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Tilemap.hpp"

plf::colony<MFCPP::Coin> CoinList;
static bool CoinDeleteGate = false;
MFCPP::SingleAnimationObject CoinAnimation;
int CoinCount = 0;
static constexpr int COIN_IMAGE_WIDTH = 96;
static constexpr int COIN_WIDTH_HEIGHT = 32;
void CoinInit() {
	MFCPP::AnimationSequenceManager::newData("CoinAnimName");
	for (int i = 0; i < COIN_IMAGE_WIDTH / COIN_WIDTH_HEIGHT; ++i) {
		ImageManager::PreloadTexture(fmt::format("Coin_{}", i), "data/resources/Coin.png", sf::IntRect({COIN_WIDTH_HEIGHT * i, 0}, {COIN_WIDTH_HEIGHT, COIN_WIDTH_HEIGHT}));
		MFCPP::AnimationSequenceManager::addSingleFrame("CoinAnimName", fmt::format("Coin_{}", i));
	}
	CoinAnimation.setAnimation(0, 2, 20, true);
	CoinAnimation.setAnimationSequence("CoinAnimName");
}
void AddCoin(const CoinID ID, const CoinAtt att, const float x, const float y) {
	CoinList.emplace(ID, att, sf::FloatRect({6.f, 2.f}, {19.f, 28.f}), sf::Vector2f(x, y), sf::Vector2f(0.f, 0.f));
}
void DeleteIndexCoin(const plf::colony<MFCPP::Coin>::colony_iterator<false>& index) {
	index->destroy();
	CoinDeleteGate = true;
}
void ForceLoadCoinTexture() {
	for (int i = 0; i < COIN_IMAGE_WIDTH / COIN_WIDTH_HEIGHT; ++i) {
		if (ImageManager::isExist(fmt::format("Coin_{}", i))) {
			ImageManager::LoadTexture(fmt::format("Coin_{}", i));
		}
	}
}
void DeleteCoin(const float x, const float y) {
	for (auto it = CoinList.begin(); it != CoinList.end(); ++it)
		if (it->getPosition().x == x && it->getPosition().y == y)
			DeleteIndexCoin(it);
}
void DeleteAllCoin() {
	CoinList.clear();
}
void CoinOnTouch() {
	if (CoinList.empty() || EffectActive) return;
	std::vector<sf::Vector2f> CoinPos{};
	CoinPos = isAccurateCollideMainCollectable(MFCPP::CollisionObject(Mario::getCurrentPosition(), Mario::getOrigin(), Mario::getHitbox()), 0, sf::FloatRect({ 6, 2 }, { 19, 28 }));
	for (const auto &i : CoinPos) {
		MFCPP::Tilemap::setIndexCollectableMapCollision(i.x, i.y, false);
		Mario::setScore(Mario::getScore() + 200);
		SoundManager::PlaySound(SoundID::GAME_COIN);
		++CoinCount;
		DeleteCoin(i.x, i.y);
	}
}
void CoinAnimationUpdate(float deltaTime) {
	CoinAnimation.frameTimeAccumulate(deltaTime);
}
void CoinDraw(float alpha) {
	if (CoinCount > 99) {
		CoinCount = 0;
		AddScoreEffect(ScoreID::SCORE_1UP, Mario::getCurrentPosition().x, Mario::getCurrentPosition().y);
	}
	for (auto &i : CoinList) {
		if (i.isDestroyed()) continue;

		if (Scroll::isOutOfScreen(MFCPP::CollisionObject(i.getInterpolatedPosition(alpha), i.getOrigin(), i.getHitbox()), 0)) continue;
		CoinAnimation.animationUpdate(i.getInterpolatedPosition(alpha), i.getOrigin());
		CoinAnimation.animationDraw();
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