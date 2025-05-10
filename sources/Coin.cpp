#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../headers/Object/Coin.hpp"
#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Object/Mario.hpp"
#include "../headers/Core/Scroll.hpp"
#include "../headers/Core/Animate/LocalAnimationManager.hpp"
#include "../headers/Core/Loading/Loading.hpp"
#include "../headers/Core/Collision/Collide.hpp"
#include "../headers/Core/Sound.hpp"
#include "../headers/Effect/ScoreEffect.hpp"
#include "../headers/Effect/MarioEffect.hpp"
#include "../headers/Core/TextureManager.hpp"

#include <vector>
#include <iostream>
#include <utility>
std::vector<CoinID> CoinIDList;
std::vector<Coin> CoinList;
std::vector<CoinAtt> CoinAttList;
TextureManager CoinTexture;
LocalAnimationManager CoinAnimation;
int CoinCount = 0;
bool firstUpdate = true;

void CoinInit() {
	CoinTexture.Loadingtexture("data/resources/Coin.png", "Coin", 0, 0, 96, 32);
	//CoinTexture.LoadingAnimatedTexture(COIN_TEXTURE, "CoinTexture", 0, 2, 0, 32, 32);
	//CoinAnimation.setAnimation(0, 2, 20);
	CoinAnimation.setAnimation(0, 2, 32, 32, 0, 20);
}
void AddCoin(CoinID ID, CoinAtt att, float x, float y) {
	Coin operate;
	setHitbox(operate.hitbox, sf::FloatRect({ 6, 2 }, { 19, 28 }));
	operate.property.setPosition({ x, y });
	CoinAnimation.setTexture(operate.property, CoinTexture.GetTexture("Coin"));
	CoinList.push_back(operate);
	CoinIDList.push_back(ID);
	CoinAttList.push_back(att);
}
void DeleteCoin(float x, float y) {
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
	auto playerHitbox = getGlobalHitbox(player.hitboxMain, player.property);
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
	for (auto& i : CoinList) {
		if (isOutScreen(i.property.getPosition().x, i.property.getPosition().y, 32, 32)) continue;
		CoinAnimation.update(i.property);
		rTexture.draw(i.property);
	}
}