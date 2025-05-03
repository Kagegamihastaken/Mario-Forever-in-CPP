#include "../headers/Effect/CoinEffect.hpp"
#include "../headers/Core/Animate/AnimationManager.hpp"
#include "../headers/Core/Scroll.hpp"
#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Effect/ScoreEffect.hpp"
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
	CoinEffectTextureManager.Loadingtexture(COINEFFECT_TEXTURE, "CoinEffect", 0, 0, 777, 32);
	//CoinEffectTextureManager.LoadingAnimatedTexture(COINEFFECT_TEXTURE, "CoinEffect", 0, 20, 0, 37, 32);
}
void AddCoinEffect(CoinID ID, CoinAtt att, float x, float y) {
	CoinEffect Init;
	Init.coinEffectAnimation.setAnimation(0, 20, 37, 32, 0, 70);
	Init.coinEffectAnimation.setTexture(Init.property, CoinEffectTextureManager.GetTexture("CoinEffect"));
	Init.property.setPosition({ static_cast<float>(round(x)), y });
	Init.property.setOrigin({ 18, 31 });
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
	if (CoinEffectList.size() == 0) return;
	for (auto& i : CoinEffectList) {
		if (i.coinEffectAnimation.isAtTheEnd()) {
			DeleteCoinEffect(i.property.getPosition().x, i.property.getPosition().y);
			continue;
		}
		i.property.move({ 0.0f, i.velocity * deltaTime });
		if (i.velocity < 0.0f) i.velocity += 0.125f * deltaTime;
		else i.velocity = 0.0f;
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