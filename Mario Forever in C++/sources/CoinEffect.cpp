#include "../headers/Effect/CoinEffect.hpp"
#include "../headers/Core/Animate/AnimationManager.hpp"
#include "../headers/Core/Scroll.hpp"
#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Effect/ScoreEffect.hpp"
#include "../headers/Effect/CoinEffect.hpp"
#include "../headers/Core/Loading/Loading.hpp"
#include "../headers/Core/TextureManager.hpp"

#include "../resource.h"

#include <SFML/Graphics.hpp>
#include <vector>

std::vector<CoinEffect> CoinEffectList;
std::vector<CoinID> CoinEffectIDList;
std::vector<CoinAtt> CoinEffectAttList;
TextureManager CoinEffectTextureManager;

void CoinEffectInit() {
	sf::Texture* CoinEffectTexture = new sf::Texture();
	sf::Texture* CoinEffectTextureTemp = new sf::Texture();
	std::vector<sf::Texture*> CoinEffectAnimatedTextureList;
	LoadTexture(*CoinEffectTexture, COINEFFECT_TEXTURE);
	for (int i = 0; i <= 20; ++i) {
		CoinEffectTextureTemp->loadFromImage(CoinEffectTexture->copyToImage(), sf::IntRect(i * 37, 0, 37, 32));
		CoinEffectAnimatedTextureList.push_back(CoinEffectTextureTemp);
		CoinEffectTextureTemp = new sf::Texture();
	}
	CoinEffectTextureManager.AddAnimatedTexture("CoinEffect", CoinEffectAnimatedTextureList);
	delete CoinEffectTexture;
	delete CoinEffectTextureTemp;
}
void AddCoinEffect(CoinID ID, CoinAtt att, float x, float y) {
	CoinEffect Init;
	Init.coinEffectAnimation.setAnimation(0, 20, 95);
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
			i.coinEffectAnimation.update(i.property, CoinEffectTextureManager.GetAnimatedTexture("CoinEffect"));
			window.draw(i.property);
		}
		else i.coinEffectAnimation.silentupdate(i.property);
	}
}