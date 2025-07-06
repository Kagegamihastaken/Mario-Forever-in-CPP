#include "Effect/BroAIEffect.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Class/BroAIEffectClass.hpp"
#include "Core/Scroll.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Interpolation.hpp"

std::vector<MFCPP::BroAIEffect> BroAIEffectList;
static bool BroAIEffectDeleteGate = false;
void BroAIEffectInit() {
	ImageManager::AddTexture("DEAD_HammerBro", "data/resources/HammerBro/DEAD_HammerBro.png");
}
void SetPrevBroAIEffectPos() {
	for (auto & i : BroAIEffectList) {
		if (i.willBeDestroyed()) continue;

		i.setPreviousPosition(i.getCurrentPosition());
	}
}
void InterpolateBroAIEffectPos(const float alpha) {
	for (auto & i : BroAIEffectList) {
		if (i.willBeDestroyed()) continue;

		i.setInterpolatedPosition(linearInterpolation(i.getPreviousPosition(), i.getCurrentPosition(), alpha));
	}
}
void AddBroAIEffect(const BroAIType type, const bool dir, const float x, const float y) {
	switch (type) {
		case HAMMER_BRO:
			BroAIEffectList.emplace_back(0.f, sf::Vector2f(x, y), sf::Vector2f(24.f, 64.f));
			BroAIEffectList.back().setTexture("DEAD_HammerBro", dir);
	}
}

void DeleteBroAIEffectIndex(const int i) {
	BroAIEffectDeleteGate = true;
	BroAIEffectList[i].willDestroy(true);
	//BroAIEffectList.erase(BroAIEffectList.begin() + i);
}
void DeleteBroAIEffect(const float x, const float y) {
	for (int i = 0; i < BroAIEffectList.size(); ++i) {
		if (BroAIEffectList[i].getCurrentPosition().x == x && BroAIEffectList[i].getCurrentPosition().y == y) {
			DeleteBroAIEffectIndex(i);
			break;
		}
	}
}
void DeleteAllBroAIEffect() {
	BroAIEffectList.clear();
}
void BroAIEffectStatusUpdate(const float deltaTime) {
	if (BroAIEffectList.empty()) return;
	for (int i = 0; i < BroAIEffectList.size(); ++i) {
		if (isOutScreenYBottom(BroAIEffectList[i].getInterpolatedPosition().y, 80)) DeleteBroAIEffectIndex(i);
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
		if (i.willBeDestroyed()) continue;

		i.move(sf::Vector2f(0.f, i.getYVelo() * deltaTime));
		i.setYVelo(i.getYVelo() + 1.f * deltaTime * 0.15f);
	}
}
void BroAIEffectCleanup() {
	if (!BroAIEffectDeleteGate) return;
	int i = 0;
	while (i < BroAIEffectList.size()) {
		if (!BroAIEffectList[i].willBeDestroyed()) ++i;
		else BroAIEffectList.erase(BroAIEffectList.begin() + i);
	}
	BroAIEffectDeleteGate = false;
}