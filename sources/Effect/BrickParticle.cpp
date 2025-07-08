#include "Core/Loading/enum.hpp"
#include "Effect/BrickParticle.hpp"
#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Class/BrickParticleClass.hpp"
#include "Core/ExternalHeaders/plf_colony.h"

plf::colony<MFCPP::BrickParticle> BrickParticleList;
static bool BrickParticleDeleteGate = false;

void BrickParticleInit() {
	ImageManager::AddTexture("NormalBrickParticle", "data/resources/BrickParticle.png", sf::IntRect({0, 0}, {16, 16}));
	ImageManager::AddTexture("GrayBrickParticle", "data/resources/BrickParticle.png", sf::IntRect({16, 0}, {16, 16}));
}
void AddSubBrickParticle(const BrickID id, float Xvelo, float Yvelo, const float ori_x, const float ori_y) {
	const auto it = BrickParticleList.emplace(Xvelo, Yvelo, sf::Vector2f(ori_x, ori_y), sf::Vector2f(8.f, 8.f));
	switch (id) {
		case BRICK_NORMAL:
			it->setTexture("NormalBrickParticle");
			break;
		case BRICK_GRAY:
			it->setTexture("GrayBrickParticle");
			break;
		default: ;
	}
}
void SetPrevBrickParticlePos() {
	for (auto &i : BrickParticleList) {
		if (i.willBeDestroyed()) continue;
		i.setPreviousAngle(i.getCurrentAngle());
		i.setPreviousPosition(i.getCurrentPosition());
	}
}
void InterpolateBrickParticlePos(const float alpha) {
	for (auto &i : BrickParticleList) {
		if (i.willBeDestroyed()) continue;
		i.setInterpolatedAngle(linearInterpolation(i.getPreviousAngle(), i.getCurrentAngle(), alpha));
		i.setInterpolatedPosition(linearInterpolation(i.getPreviousPosition(), i.getCurrentPosition(), alpha));
	}
}
void AddBrickParticle(const BrickID id, const float ori_x, const float ori_y) {
	AddSubBrickParticle(id, -2.f, -8.f, ori_x + 8.f, ori_y + 8.f);
	AddSubBrickParticle(id, 2.f, -8.f, ori_x + 24.f, ori_y + 8.f);
	AddSubBrickParticle(id, -4.f, -7.f, ori_x + 8.f, ori_y + 24.f);
	AddSubBrickParticle(id, 4.f, -7.f, ori_x + 24.f, ori_y + 24.f);
}
void DeleteBrickParticleIndex(const plf::colony<MFCPP::BrickParticle>::colony_iterator<false>& it) {
	BrickParticleDeleteGate = true;
	it->willDestroy(true);
}
void BrickParticleStatusUpdate(const float deltaTime) {
	if (BrickParticleList.empty()) return;

	for (auto it = BrickParticleList.begin(); it != BrickParticleList.end(); ++it) {
		if (it->willBeDestroyed()) continue;

		if (isOutScreen(it->getCurrentPosition().x, it->getCurrentPosition().y, 48, 48))
			DeleteBrickParticleIndex(it);
		it->move(sf::Vector2f(it->getXvelo() * deltaTime, it->getYvelo() * deltaTime));

		if (it->getXvelo() > 0.f) it->setCurrentAngle(it->getCurrentAngle() + sf::degrees(10.f * deltaTime));
		else if (it->getXvelo() < 0.f) it->setCurrentAngle(it->getCurrentAngle() - sf::degrees(10.f * deltaTime));
		it->setYvelo(it->getYvelo() + 1.f * deltaTime * 0.3f);
	}
}
void DeleteAllBrickParticle() {
	BrickParticleList.clear();
}
void BrickParticleDraw() {
	if (BrickParticleList.empty()) return;

	for (auto &i : BrickParticleList) {
		if (!isOutScreen(i.getCurrentPosition().x, i.getCurrentPosition().y, 48, 48)) {
			i.AnimationUpdate(i.getInterpolatedPosition(), i.getOrigin());
			i.setRotation(i.getInterpolatedAngle());
			i.AnimationDraw(window);
		}
	}
}
void BrickParticleCleanup() {
	if (!BrickParticleDeleteGate) return;
	auto it = BrickParticleList.begin();
	while (it != BrickParticleList.end()) {
		if (!it->willBeDestroyed()) ++it;
		else it = BrickParticleList.erase(it);
	}
	BrickParticleDeleteGate = false;
}