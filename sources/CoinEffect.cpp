#include "Effect/CoinEffect.hpp"
#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Core/Loading/Loading.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Interpolation.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

std::vector<CoinEffect> CoinEffectList;
std::vector<CoinID> CoinEffectIDList;
std::vector<CoinAtt> CoinEffectAttList;
static std::vector<std::string> CoinEffectAnimName;
static constexpr int COINEFFECT_IMAGE_WIDTH = 777;
static constexpr int COINEFFECT_WIDTH = 37;
static constexpr int COINEFFECT_HEIGHT = 32;
void CoinEffectInit() {
	ImageManager::AddImage("CoinEffectImage", "data/resources/CoinEffect.png");
	for (int i = 0; i < COINEFFECT_IMAGE_WIDTH / COINEFFECT_WIDTH; ++i) {
		ImageManager::AddTexture("CoinEffectImage", sf::IntRect({i * COINEFFECT_WIDTH, 0}, {COINEFFECT_WIDTH, COINEFFECT_HEIGHT}), "CoinEffect_"+std::to_string(i));
		CoinEffectAnimName.push_back("CoinEffect_"+std::to_string(i));
	}
}
void SetPrevCoinEffectPos() {
	for (auto & i : CoinEffectList) {
		i.prev = i.curr;
	}
}
void InterpolateCoinEffectPos(const float alpha) {
	for (auto & i : CoinEffectList) {
		i.property.setPosition(linearInterpolation(i.prev, i.curr, alpha));
	}
}
void AddCoinEffect(const CoinID ID, const CoinAtt att, const float x, const float y) {
	CoinEffect Init;
	Init.coinEffectAnimation.setAnimation(0, 20, 70);
	Init.coinEffectAnimation.setAnimationSequence(CoinEffectAnimName, CoinEffectAnimName);
	Init.property.setPosition({ static_cast<float>(round(x)), y });
	Init.property.setOrigin({ 18, 31 });
	Init.curr = Init.prev = Init.property.getPosition();
	CoinEffectList.push_back(Init);
	CoinEffectIDList.push_back(ID);
	CoinEffectAttList.push_back(att);
}
void DeleteCoinEffect(const int i) {
	const sf::Vector2f pos = CoinEffectList[i].property.getPosition();
	CoinEffectList.erase(CoinEffectList.begin() + i);
	CoinEffectIDList.erase(CoinEffectIDList.begin() + i);
	CoinEffectAttList.erase(CoinEffectAttList.begin() + i);
	AddScoreEffect(SCORE_200, pos.x, pos.y);
}
void DeleteCoinEffect(const float x, const float y) {
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
inline void CoinEffectStatusUpdate(const float deltaTime) {
	for (auto & i : CoinEffectList) {
		if (i.coinEffectAnimation.isAnimationAtTheEnd()) {
			DeleteCoinEffect(i.curr.x, i.curr.y);
			continue;
		}
		i.curr = { i.curr.x, i.curr.y + i.velocity * deltaTime };
		if (i.velocity < 0.0f) i.velocity += 0.125f * deltaTime;
		else i.velocity = 0.0f;
	}
}
inline void CoinEffectUpdate() {
	if (CoinEffectList.empty()) return;
	for (auto& i : CoinEffectList) {
		if (!isOutScreen(i.property.getPosition().x, i.property.getPosition().y, 32, 32)) {
			i.coinEffectAnimation.AnimationUpdate(i.property.getPosition(), i.property.getOrigin());
			i.coinEffectAnimation.AnimationDraw(window);
		}
	}
}