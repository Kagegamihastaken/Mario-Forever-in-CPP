#include "Effect/BroAIEffect.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/WindowFrame.hpp"
#include "Class/BroAIEffectClass.hpp"
#include "Core/Scroll.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Interpolation.hpp"

std::vector<MFCPP::BroAIEffect> BroAIEffectList;
void BroAIEffectInit() {
	ImageManager::AddImage("DEAD_HammerImage", "data/resources/HammerBro/DEAD_HammerBro.png");
	ImageManager::AddTexture("DEAD_HammerImage", "DEAD_HammerRight");
	ImageManager::AddTexture("DEAD_HammerImage", "DEAD_HammerLeft", false, true);
}
void SetPrevBroAIEffectPos() {
	for (auto & i : BroAIEffectList) {
		i.setPreviousPosition(i.getCurrentPosition());
	}
}
void InterpolateBroAIEffectPos(const float alpha) {
	for (auto & i : BroAIEffectList) {
		i.setInterpolatedPosition(linearInterpolation(i.getPreviousPosition(), i.getCurrentPosition(), alpha));
	}
}
void AddBroAIEffect(const BroAIType type, const bool dir, const float x, const float y) {
	switch (type) {
		case HAMMER_BRO:
			BroAIEffectList.emplace_back(0.f, sf::Vector2f(x, y), sf::Vector2f(24.f, 64.f));
			BroAIEffectList.back().setTexture((dir ? "DEAD_HammerLeft" : "DEAD_HammerRight"));
	}
}

void DeleteBroAIEffectIndex(const int i) {
	BroAIEffectList.erase(BroAIEffectList.begin() + i);
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
		if (isOutScreenYBottom(BroAIEffectList[i].getInterpolatedPosition().y, 80)) DeleteBroAIEffect(
			BroAIEffectList[i].getCurrentPosition().x, BroAIEffectList[i].getCurrentPosition().y);
	}
}
void BroAIEffectDraw() {
	if (BroAIEffectList.empty()) return;
	for (auto & i : BroAIEffectList) {
		if (!isOutScreen(i.getInterpolatedPosition().x, i.getInterpolatedPosition().y, 64, 64)) {
			i.AnimationUpdate(i.getInterpolatedPosition(), i.getOrigin());
			i.AnimationDraw(window);
		}
	}
}
void BroAIEffectVertYUpdate(const float deltaTime) {
	for (int i = 0; i < BroAIEffectList.size(); ++i) {
		BroAIEffectList[i].move(sf::Vector2f(0.f, BroAIEffectList[i].getYVelo() * deltaTime));
		BroAIEffectList[i].setYVelo(BroAIEffectList[i].getYVelo() + 1.f * deltaTime * 0.15f);
	}
}