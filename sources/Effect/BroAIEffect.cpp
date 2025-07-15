#include "Effect/BroAIEffect.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Class/BroAIEffectClass.hpp"
#include "Core/Scroll.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Interpolation.hpp"
#include "Core/ExternalHeaders/plf_colony.h"

plf::colony<MFCPP::BroAIEffect> BroAIEffectList;
static bool BroAIEffectDeleteGate = false;
void BroAIEffectInit() {
	ImageManager::AddTexture("DEAD_HammerBro", "data/resources/HammerBro/DEAD_HammerBro.png");
}
void SetPrevBroAIEffectPos() {
	for (auto & i : BroAIEffectList) {
		if (i.isDestroyed()) continue;

		i.setPreviousPosition(i.getCurrentPosition());
	}
}
void InterpolateBroAIEffectPos(const float alpha) {
	for (auto & i : BroAIEffectList) {
		if (i.isDestroyed()) continue;

		i.setInterpolatedPosition(linearInterpolation(i.getPreviousPosition(), i.getCurrentPosition(), alpha));
	}
}
void AddBroAIEffect(const BroAIType type, const bool dir, const float x, const float y) {
	plf::colony<MFCPP::BroAIEffect>::colony_iterator<false> it;
	switch (type) {
		case HAMMER_BRO:
			it = BroAIEffectList.emplace(0.f, sf::Vector2f(x, y), sf::Vector2f(24.f, 64.f));
			it->setTexture("DEAD_HammerBro", dir);
			break;
		default: ;
	}
}

void DeleteBroAIEffectIndex(const plf::colony<MFCPP::BroAIEffect>::colony_iterator<false>& it) {
	BroAIEffectDeleteGate = true;
	it->setDestroyed(true);
	//BroAIEffectList.erase(BroAIEffectList.begin() + i);
}
void DeleteBroAIEffect(const float x, const float y) {
	for (auto it = BroAIEffectList.begin(); it != BroAIEffectList.end(); ++it) {
		if (it->getCurrentPosition().x == x && it->getCurrentPosition().y == y) {
			DeleteBroAIEffectIndex(it);
			break;
		}
	}
}
void DeleteAllBroAIEffect() {
	BroAIEffectList.clear();
}
void BroAIEffectStatusUpdate(const float deltaTime) {
	if (BroAIEffectList.empty()) return;
	for (auto it = BroAIEffectList.begin(); it != BroAIEffectList.end(); ++it) {
		if (isOutScreenYBottom(it->getInterpolatedPosition().y, 80)) DeleteBroAIEffectIndex(it);
	}
}
void BroAIEffectDraw() {
	if (BroAIEffectList.empty()) return;
	for (auto & i : BroAIEffectList) {
		if (!isOutScreen(i.getInterpolatedPosition().x, i.getInterpolatedPosition().y, 80, 80)) {
			i.AnimationUpdate(i.getInterpolatedPosition(), i.getOrigin());
			i.AnimationDraw(window);
		}
	}
}
void BroAIEffectVertYUpdate(const float deltaTime) {
	for (auto & i : BroAIEffectList) {
		if (i.isDestroyed()) continue;

		i.move(sf::Vector2f(0.f, i.getYVelo() * deltaTime));
		i.setYVelo(i.getYVelo() + 1.f * deltaTime * 0.15f);
	}
}
void BroAIEffectCleanup() {
	if (!BroAIEffectDeleteGate) return;
	auto it = BroAIEffectList.begin();
	while (it != BroAIEffectList.end()) {
		if (!it->isDestroyed()) ++it;
		else it = BroAIEffectList.erase(it);
	}
	BroAIEffectDeleteGate = false;
}