#include "Effect/CoinEffect.hpp"
#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Class/CoinEffectClass.hpp"

std::vector<MFCPP::CoinEffect> CoinEffectList;
void CoinEffectInit() {
	for (int i = 0; i < COINEFFECT_IMAGE_WIDTH / COINEFFECT_WIDTH; ++i) {
		ImageManager::AddTexture(fmt::format("CoinEffect_{}", i), "data/resources/CoinEffect.png", sf::IntRect({i * COINEFFECT_WIDTH, 0}, {COINEFFECT_WIDTH, COINEFFECT_HEIGHT}));
		CoinEffectAnimName.push_back(fmt::format("CoinEffect_{}", i));
	}
}
void SetPrevCoinEffectPos() {
	for (auto & i : CoinEffectList)
		i.setPreviousPosition(i.getCurrentPosition());

}
void InterpolateCoinEffectPos(const float alpha) {
	for (auto & i : CoinEffectList)
		i.setInterpolatedPosition(linearInterpolation(i.getPreviousPosition(), i.getCurrentPosition(), alpha));
}
void AddCoinEffect(const CoinID ID, const CoinAtt att, const float x, const float y) {
	CoinEffectList.emplace_back(ID, att, sf::Vector2f(x, y), sf::Vector2f(18.f, 31.f));
	CoinEffectList.back().setAnimation(0, 20, 70);
	CoinEffectList.back().setAnimationSequence(CoinEffectAnimName);
}
void DeleteCoinEffect(const int i) {
	const sf::Vector2f pos = CoinEffectList[i].getCurrentPosition();
	CoinEffectList.erase(CoinEffectList.begin() + i);
	AddScoreEffect(SCORE_200, pos.x, pos.y);
}
void DeleteCoinEffect(const float x, const float y) {
	for (int i = 0; i < CoinEffectList.size(); ++i) {
		if (CoinEffectList[i].getCurrentPosition().x == x && CoinEffectList[i].getCurrentPosition().y == y) {
			CoinEffectList.erase(CoinEffectList.begin() + i);
			AddScoreEffect(SCORE_200, x, y);
			break;
		}
	}
}
void DeleteAllCoinEffect() {
	CoinEffectList.clear();
}
void CoinEffectStatusUpdate(const float deltaTime) {
	for (auto & i : CoinEffectList) {
		if (i.isAnimationAtTheEnd()) {
			DeleteCoinEffect(i.getCurrentPosition().x, i.getCurrentPosition().y);
			continue;
		}
		i.move(sf::Vector2f(0.f, i.getYVelo() * deltaTime));
		//i.curr = { i.curr.x, i.curr.y + i.velocity * deltaTime };
		if (i.getYVelo() < 0.0f) i.setYVelo(i.getYVelo() + 0.125f * deltaTime);
		else i.setYVelo(0.f);
	}
}
void CoinEffectDraw() {
	if (CoinEffectList.empty()) return;
	for (auto& i : CoinEffectList) {
		if (!isOutScreen(i.getInterpolatedPosition().x, i.getInterpolatedPosition().y, 32, 32)) {
			i.AnimationUpdate(i.getInterpolatedPosition(), i.getOrigin());
			i.AnimationDraw(window);
		}
	}
}