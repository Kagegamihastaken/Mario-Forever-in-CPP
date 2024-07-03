#include "../headers/CoinEffect.hpp"
#include "../headers/AnimationManager.hpp"
#include "../headers/Scroll.hpp"
#include "../headers/WindowFrame.hpp"
#include "../headers/ScoreEffect.hpp"
#include "../headers/CoinEffect.hpp"
#include "../headers/Loading.hpp"

#include "../resource.h"

#include <SFML/Graphics.hpp>
#include <vector>

std::vector<CoinEffect> CoinEffectList;
sf::Texture CoinEffectTexture;
std::vector<CoinID> CoinEffectIDList;
std::vector<CoinAtt> CoinEffectAttList;

int CoinEffectInit() {
	LoadTexture(CoinEffectTexture, COINEFFECT_TEXTURE);
	return 6;
}
int iniCoinEffect = CoinEffectInit();
void AddCoinEffect(CoinID ID, CoinAtt att, float x, float y) {
	CoinEffect Init;
	Init.coinEffectAnimation.setAnimation({ 37,32 }, { 0,0 }, { 21,0 }, 95);
	Init.property.setTexture(CoinEffectTexture);
	Init.property.setPosition(static_cast<float>(round(x)), y);
	CoinEffectList.push_back(Init);
	CoinEffectIDList.push_back(ID);
	CoinEffectAttList.push_back(att);
}
void DeleteCoinEffect(float x, float y) {
	for (int i = 0; i < CoinEffectList.size(); ++i) {
		if (CoinEffectList[i].property.getPosition().x == x && CoinEffectList[i].property.getPosition().y == y) {
			CoinEffectList.erase(CoinEffectList.begin() + i);
			CoinEffectIDList.erase(CoinEffectIDList.begin() + i);
			CoinEffectAttList.erase(CoinEffectAttList.begin() + i);
			AddScoreEffect(SCORE_200, x + 4, y);
			break;
		}
	}
}
void DeleteAllCoinEffect() {
	CoinEffectList.clear();
	CoinEffectIDList.clear();
	CoinEffectAttList.clear();
}
inline void CoinEffectStatusUpdate() {
	for (auto& i : CoinEffectList) {
		i.property.move(0.0f, i.velocity * deltaTime);
		if (i.velocity < 0.0f) i.velocity += 0.125f * deltaTime;
		else i.velocity = 0.0f;

		if (i.coinEffectAnimation.isAtTheEnd()) DeleteCoinEffect(i.property.getPosition().x, i.property.getPosition().y);
	}
}
inline void CoinEffectUpdate() {
	for (auto& i : CoinEffectList) {
		if (!isOutScreen(i.property.getPosition().x, i.property.getPosition().y, 32, 32)) {
			i.coinEffectAnimation.update(i.property, CoinEffectTexture);
			window.draw(i.property);
		}
		else i.coinEffectAnimation.silentupdate(i.property);
	}
}