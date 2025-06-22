#include <SFML/Graphics.hpp>
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

#include <vector>
std::vector<CoinID> CoinIDList;
std::vector<Coin> CoinList;
std::vector<CoinAtt> CoinAttList;
std::vector<std::pair<sf::FloatRect, sf::Vector2f>> CoinPosList;
SingleAnimationObject CoinAnimation;
int CoinCount = 0;
bool firstUpdate = true;
static std::vector<std::string> CoinAnimName;

static constexpr int COIN_IMAGE_WIDTH = 96;
static constexpr int COIN_WIDTH_HEIGHT = 32;
void CoinInit() {
	ImageManager::AddImage("CoinImage", "data/resources/Coin.png");
	for (int i = 0; i < COIN_IMAGE_WIDTH / COIN_WIDTH_HEIGHT; ++i) {
		ImageManager::AddTexture("CoinImage", sf::IntRect({COIN_WIDTH_HEIGHT * i, 0}, {COIN_WIDTH_HEIGHT, COIN_WIDTH_HEIGHT}), "Coin_" + std::to_string(i));
		CoinAnimName.emplace_back("Coin_" + std::to_string(i));
	}
	CoinAnimation.setAnimation(0, 2, 20);
	CoinAnimation.setAnimationSequence(CoinAnimName, CoinAnimName);
}
void AddCoin(const CoinID ID, const CoinAtt att, const float x, const float y) {
	Coin operate;
	setHitbox(operate.hitbox, sf::FloatRect({ 6, 2 }, { 19, 28 }));
	operate.property.setPosition({ x, y });
	CoinList.push_back(operate);
	CoinIDList.push_back(ID);
	CoinAttList.push_back(att);
	CoinPosList.emplace_back( operate.hitbox, operate.property.getPosition() );
}
void DeleteIndexCoin(const unsigned int& index) {
	CoinList.erase(CoinList.begin() + index);
	CoinIDList.erase(CoinIDList.begin() + index);
	CoinAttList.erase(CoinAttList.begin() + index);
	CoinPosList.erase(CoinPosList.begin() + index);
}
void DeleteCoin(const float x, const float y) {
	for (int i = 0; i < CoinList.size(); i++) {
		if (CoinPosList[i].second.x == x && CoinPosList[i].second.y == y) {
			DeleteIndexCoin(i);
			break;
		}
	}
}
void DeleteAllCoin() {
	CoinList.clear();
	CoinIDList.clear();
	CoinAttList.clear();
	CoinPosList.clear();
}
void CoinOnTouch() {
	if (CoinList.empty() || EffectActive) return;
	std::vector<sf::Vector2f> CoinPos{};
	if (!MarioDirection) {
		const int be = find_min_inx(player.curr, CoinPosList);
		const int nd = find_max_inx_dist(player.curr, CoinPosList, 64.0f + (Xvelo) * 4.0f);
		CoinPos = isAccurateCollideMaint(MFCPP::CollisionObject(player.curr, player.property.getOrigin(), player.hitboxMain), CoinPosList, be, nd, 80.0f);
	}
	else {
		const int be = find_max_inx(player.curr, CoinPosList);
		const int nd = find_min_inx_dist(player.curr, CoinPosList, 64.0f + (Xvelo) * 4.0f);
		CoinPos = isAccurateCollideMaint(MFCPP::CollisionObject(player.curr, player.property.getOrigin(), player.hitboxMain), CoinPosList, nd, be, 80.0f);
	}
	for (const auto &i : CoinPos) {
		Score += 200;
		SoundManager::PlaySound("Coin");
		++CoinCount;
		DeleteCoin(i.x, i.y);
	}
}
inline void CoinUpdate() {
	if (CoinCount > 99) {
		CoinCount = 0;
		AddScoreEffect(SCORE_1UP, player.property.getPosition().x, player.property.getPosition().y);
	}
	for (auto &[property, hitbox] : CoinList) {
		if (isOutScreen(property.getPosition().x, property.getPosition().y, 32, 32)) continue;
		CoinAnimation.AnimationUpdate(property.getPosition(), property.getOrigin());
		CoinAnimation.AnimationDraw(window);
		//CoinAnimationList[i].update(CoinList[i].property);
		//window.draw(CoinList[i].property);
	}
}