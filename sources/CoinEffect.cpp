#include "../headers/Effect/CoinEffect.hpp"
#include "../headers/Core/Animate/AnimationManager.hpp"
#include "../headers/Core/Scroll.hpp"
#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Effect/ScoreEffect.hpp"
#include "../headers/Core/Loading/Loading.hpp"
#include "../headers/Core/TextureManager.hpp"
#include "../headers/Core/Interpolation.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

std::vector<CoinEffect> CoinEffectList;
std::vector<CoinID> CoinEffectIDList;
std::vector<CoinAtt> CoinEffectAttList;
TextureManager CoinEffectTextureManager;

void CoinEffectInit() {
	CoinEffectTextureManager.Loadingtexture("data/resources/CoinEffect.png", "CoinEffect", 0, 0, 777, 32);
	//CoinEffectTextureManager.LoadingAnimatedTexture(COINEFFECT_TEXTURE, "CoinEffect", 0, 20, 0, 37, 32);
}
void SetPrevCoinEffectPos() {
	for (int i = 0; i < CoinEffectList.size(); i++) {
		CoinEffectList[i].prev = CoinEffectList[i].curr;
	}
}
void InterpolateCoinEffectPos(float alpha) {
	for (int i = 0; i < CoinEffectList.size(); i++) {
		CoinEffectList[i].property.setPosition(linearInterpolation(CoinEffectList[i].prev, CoinEffectList[i].curr, alpha));
	}
}
void AddCoinEffect(CoinID ID, CoinAtt att, float x, float y) {
	CoinEffect Init;
	Init.coinEffectAnimation.setAnimation(0, 20, 37, 32, 0, 70);
	Init.coinEffectAnimation.setTexture(Init.property, CoinEffectTextureManager.GetTexture("CoinEffect"));
	Init.property.setPosition({ static_cast<float>(round(x)), y });
	Init.property.setOrigin({ 18, 31 });
	Init.curr = Init.prev = Init.property.getPosition();
	CoinEffectList.push_back(Init);
	CoinEffectIDList.push_back(ID);
	CoinEffectAttList.push_back(att);
}
void DeleteCoinEffect(int i) {
	sf::Vector2f pos = CoinEffectList[i].property.getPosition();
	CoinEffectList.erase(CoinEffectList.begin() + i);
	CoinEffectIDList.erase(CoinEffectIDList.begin() + i);
	CoinEffectAttList.erase(CoinEffectAttList.begin() + i);
	AddScoreEffect(SCORE_200, pos.x, pos.y);
}
void DeleteCoinEffect(float x, float y) {
	for (int i = 0; i < CoinEffectList.size(); ++i) {
		if (CoinEffectList[i].curr.x == x && CoinEffectList[i].curr.y == y) {
			CoinEffectList.erase(CoinEffectList.begin() + i);
			CoinEffectIDList.erase(CoinEffectIDList.begin() + i);
			CoinEffectAttList.erase(CoinEffectAttList.begin() + i);
			AddScoreEffect(SCORE_200, x, y);
			break;
		}
	}
}
void DeleteAllCoinEffect() {
	CoinEffectList.clear();
	CoinEffectIDList.clear();
	CoinEffectAttList.clear();
}
inline void CoinEffectStatusUpdate(float deltaTime) {
	for (int i = 0; i < CoinEffectList.size(); ++i) {
		if (CoinEffectList[i].coinEffectAnimation.isAtTheEnd()) {
			DeleteCoinEffect(i);
			continue;
		}
		CoinEffectList[i].curr = { CoinEffectList[i].curr.x, CoinEffectList[i].curr.y + CoinEffectList[i].velocity * deltaTime };
		if (CoinEffectList[i].velocity < 0.0f) CoinEffectList[i].velocity += 0.125f * deltaTime;
		else CoinEffectList[i].velocity = 0.0f;
	}
}
inline void CoinEffectUpdate() {
	if (CoinEffectList.size() == 0) return;
	for (auto& i : CoinEffectList) {
		if (!isOutScreen(i.property.getPosition().x, i.property.getPosition().y, 32, 32)) {
			i.coinEffectAnimation.update(i.property);
			rTexture.draw(i.property);
		}
		else i.coinEffectAnimation.silentupdate();
	}
}