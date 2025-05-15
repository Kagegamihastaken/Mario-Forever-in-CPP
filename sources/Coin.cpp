#include <SFML/Graphics.hpp>
#include "Object/Coin.hpp"
#include "Core/WindowFrame.hpp"
#include "Object/Mario.hpp"
#include "Core/Scroll.hpp"
#include "Core/Animate/LocalAnimationManager.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Sound.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Effect/MarioEffect.hpp"
#include "Core/TextureManager.hpp"

#include <vector>
std::vector<CoinID> CoinIDList;
std::vector<Coin> CoinList;
std::vector<CoinAtt> CoinAttList;
TextureManager CoinTexture;
LocalAnimationManager CoinAnimation;
int CoinCount = 0;
bool firstUpdate = true;

void CoinInit() {
	CoinTexture.Loadingtexture("data/resources/Coin.png", "Coin", 0, 0, 96, 32);
	CoinAnimation.setAnimation(0, 2, 32, 32, 0, 20);
}
void AddCoin(const CoinID ID, const CoinAtt att, const float x, const float y) {
	Coin operate;
	setHitbox(operate.hitbox, sf::FloatRect({ 6, 2 }, { 19, 28 }));
	operate.property.setPosition({ x, y });
	CoinAnimation.setTexture(operate.property, CoinTexture.GetTexture("Coin"));
	CoinList.push_back(operate);
	CoinIDList.push_back(ID);
	CoinAttList.push_back(att);
}
void DeleteCoin(const float x, const float y) {
	for (int i = 0; i < CoinList.size(); i++) {
		if (CoinList[i].property.getPosition().x == x && CoinList[i].property.getPosition().y == y) {
			CoinList.erase(CoinList.begin() + i);
			CoinIDList.erase(CoinIDList.begin() + i);
			CoinAttList.erase(CoinAttList.begin() + i);
			break;
		}
	}
}
void DeleteAllCoin() {
	CoinList.clear();
	CoinIDList.clear();
	CoinAttList.clear();
}
inline void CoinOnTouch() {
	if (CoinList.size() == 0 || EffectActive) return;
	const auto playerHitbox = getGlobalHitbox(player.hitboxMain, player.property);
	for (int i = 0; i < CoinList.size(); i++) {
		if (isOutScreen(CoinList[i].property.getPosition().x, CoinList[i].property.getPosition().y, 32, 32)) continue;
		if (isCollide(CoinList[i].hitbox, CoinList[i].property, playerHitbox)) {
			Score += 200;
			DeleteCoin(CoinList[i].property.getPosition().x, CoinList[i].property.getPosition().y);
			Sounds.PlaySound("Coin");
			++CoinCount;
			break;
		}
	}
}
inline void CoinUpdate() {
	if (CoinCount > 99) {
		CoinCount = 0;
		AddScoreEffect(SCORE_1UP, player.property.getPosition().x, player.property.getPosition().y);
	}
	for (auto&[property, hitbox] : CoinList) {
		if (isOutScreen(property.getPosition().x, property.getPosition().y, 32, 32)) continue;
		CoinAnimation.update(property);
		rObject.draw(property);
	}
}