#include "Effect/BulletBillEffect.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Class/BulletBillEffectClass.hpp"
#include "Core/Scroll.hpp"
#include "Core/Interpolation.hpp"
#include "Core/ExternalHeaders/plf_colony.h"

plf::colony<MFCPP::BulletBillEffect> BulletBillEffectList;
static bool BulletBillEffectDeleteGate = false;
void SetPrevBulletBillEffectPos() {
	for (auto & i : BulletBillEffectList) {
		if (i.isDestroyed()) continue;

		i.setPreviousPosition(i.getCurrentPosition());
	}
}
void InterpolateBulletBillEffectPos(const float alpha) {
	for (auto & i : BulletBillEffectList) {
		if (i.isDestroyed()) continue;

		i.setInterpolatedPosition(linearInterpolation(i.getPreviousPosition(), i.getCurrentPosition(), alpha));
	}
}
void AddBulletBillEffect(const BulletType type, const bool dir, const float Xvelo, const float x, const float y) {
	plf::colony<MFCPP::BulletBillEffect>::colony_iterator<false> it;
	switch (type) {
		case BULLET_NORMAL:
			it = BulletBillEffectList.emplace(0.f, Xvelo, sf::Vector2f(x, y), sf::Vector2f(16.f, 28.f));
			it->setTexture("BulletBillNormal_0", dir);
			break;
		default: ;
	}
}

void DeleteBulletBillEffectIndex(const plf::colony<MFCPP::BulletBillEffect>::colony_iterator<false> it) {
	BulletBillEffectDeleteGate = true;
	it->setDestroyed(true);
}
void DeleteBulletBillEffect(const float x, const float y) {
	for (auto it = BulletBillEffectList.begin(); it != BulletBillEffectList.end(); ++it) {
		if (it->getCurrentPosition().x == x && it->getCurrentPosition().y == y) {
			DeleteBulletBillEffectIndex(it);
			break;
		}
	}
}
void DeleteAllBulletBillEffect() {
	BulletBillEffectList.clear();
}
void BulletBillEffectStatusUpdate() {
	if (BulletBillEffectList.empty()) return;
	for (auto it = BulletBillEffectList.begin(); it != BulletBillEffectList.end(); ++it) {
		if (isOutScreenYBottom(it->getInterpolatedPosition().y, 80)) DeleteBulletBillEffectIndex(it);
	}
}
void BulletBillEffectDraw() {
	if (BulletBillEffectList.empty()) return;
	for (auto & i : BulletBillEffectList) {
		if (!isOutScreen(i.getInterpolatedPosition().x, i.getInterpolatedPosition().y, 80, 80)) {
			i.AnimationUpdate(i.getInterpolatedPosition(), i.getOrigin());
			i.AnimationDraw(window);
		}
	}
}
void BulletBillEffectPositionUpdate(const float deltaTime) {
	for (auto & i : BulletBillEffectList) {
		if (i.isDestroyed()) continue;

		i.move(sf::Vector2f(i.getXVelo() * (i.getAnimationDirection() ? -deltaTime : deltaTime), i.getYVelo() * deltaTime));
		i.setYVelo(i.getYVelo() + 1.f * deltaTime * 0.3f);
	}
}
void BulletBillEffectCleanup() {
	if (!BulletBillEffectDeleteGate) return;
	auto it = BulletBillEffectList.begin();
	while (it != BulletBillEffectList.end()) {
		if (!it->isDestroyed()) ++it;
		else it = BulletBillEffectList.erase(it);
	}
	BulletBillEffectDeleteGate = false;
}