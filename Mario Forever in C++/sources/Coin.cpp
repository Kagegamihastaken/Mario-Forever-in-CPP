#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../headers/Object/Coin.hpp"
#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Object/Mario.hpp"
#include "../headers/Core/Scroll.hpp"
#include "../headers/Core/Animate/AnimationManager.hpp"
#include "../headers/Core/Loading/Loading.hpp"
#include "../headers/Core/Collision/Collide.hpp"
#include "../headers/Core/Sound.hpp"

#include "../resource.h"

#include <vector>
#include <iostream>
#include <utility>
std::vector<CoinID> CoinIDList;
std::vector<Coin> CoinList;
std::vector<CoinAtt> CoinAttList;
sf::Texture CoinTexture;
AnimationManager CoinAnimation;
int CoinCount = 0;
bool firstUpdate = true;

int CoinInit() {
	LoadTexture(CoinTexture, COIN_TEXTURE);
	CoinAnimation.addAnimation("IdleCoin", &CoinTexture, { 3,0 }, { 32,32 }, { 0,0 }, 20);
	return 6;
}
int CoinIni = CoinInit();
void AddCoin(CoinID ID, CoinAtt att, float x, float y) {
	Coin operate;
	setHitbox(operate.hitbox, { 6, 2, 19, 28 });
	operate.property.setTexture(CoinTexture);
	operate.property.setPosition(x, y);
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
	auto playerHitbox = getGlobalHitbox(player.hitboxMain, player.property);
	for (int i = 0; i < CoinList.size(); i++) {
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
	if (CoinCount > 99) CoinCount = 0;
	for (auto& i : CoinList) {
		if (!isOutScreen(i.property.getPosition().x, i.property.getPosition().y, 32, 32)) {
			CoinAnimation.update("IdleCoin", i.property);
			window.draw(i.property);
		}
	}
}