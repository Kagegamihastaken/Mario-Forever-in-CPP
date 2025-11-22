#include "Effect/CoinEffect.hpp"
#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Class/CoinEffectClass.hpp"

std::vector<MFCPP::CoinEffect> CoinEffectList;
static bool CoinEffectDeleteGate = false;

void CoinEffectInit() {
	for (int i = 0; i < COINEFFECT_IMAGE_WIDTH / COINEFFECT_WIDTH; ++i)
		ImageManager::AddTexture(fmt::format("CoinEffect_{}", i), "data/resources/CoinEffect.png", sf::IntRect({i * COINEFFECT_WIDTH, 0}, {COINEFFECT_WIDTH, COINEFFECT_HEIGHT}));
}
void SetPrevCoinEffectPos() {
	for (auto & i : CoinEffectList) {
		if (i.isDestroyed()) continue;

		i.setPreviousPosition(i.getCurrentPosition());
	}

}
void InterpolateCoinEffectPos(const float alpha) {
	for (auto & i : CoinEffectList) {
		if (i.isDestroyed()) continue;

		i.setInterpolatedPosition(linearInterpolation(i.getPreviousPosition(), i.getCurrentPosition(), alpha));
	}
}
void AddCoinEffect(const CoinID ID, const CoinAtt att, const float x, const float y) {
	CoinEffectList.emplace_back(ID, att, sf::Vector2f(x, y), sf::Vector2f(18.f, 31.f));
	CoinEffectList.back().setAnimation(0, 20, 70);
	CoinEffectList.back().setAnimationSequence(CoinEffectAnimName);
}
void DeleteCoinEffect(const std::vector<MFCPP::CoinEffect>::iterator& it) {
	CoinEffectDeleteGate = true;
	const sf::Vector2f pos = it->getCurrentPosition();
	it->setDestroyed(true);
	AddScoreEffect(SCORE_200, pos.x, pos.y);
}
void DeleteAllCoinEffect() {
	CoinEffectList.clear();
}
void CoinEffectStatusUpdate(const float deltaTime) {
	for (auto it = CoinEffectList.begin(); it != CoinEffectList.end(); ++it) {
		if (it->isAnimationAtTheEnd()) {
			DeleteCoinEffect(it);
			continue;
		}
		it->move(sf::Vector2f(0.f, it->getYVelo() * deltaTime));
		if (it->getYVelo() < 0.0f) it->setYVelo(it->getYVelo() + 0.125f * deltaTime);
		else it->setYVelo(0.f);
	}
}
void CoinEffectDraw() {
	if (CoinEffectList.empty()) return;
	for (auto& i : CoinEffectList) {
		if (!isOutScreen(i.getInterpolatedPosition().x, i.getInterpolatedPosition().y, 32, 32)) {
			i.AnimationUpdate(i.getInterpolatedPosition(), i.getOrigin());
			i.AnimationDraw();
		}
		else i.FrameUpdate();
	}
}
void CoinEffectCleanup() {
	if (!CoinEffectDeleteGate) return;
	auto it = CoinEffectList.begin();
	while (it != CoinEffectList.end()) {
		if (!it->isDestroyed()) ++it;
		else it = CoinEffectList.erase(it);
	}
	CoinEffectDeleteGate = false;
}