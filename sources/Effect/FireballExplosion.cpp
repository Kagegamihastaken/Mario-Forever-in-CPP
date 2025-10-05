#include "Effect/FireballExplosion.hpp"
#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Class/FireballExplosionClass.hpp"

std::vector<MFCPP::FireballExplosion> FireballExplosionList;
void FireballExplosionInit() {
	for (int i = 0; i < FIREBALLEXPLOSION_IMAGE_WIDTH / FIREBALLEXPLOSION_WIDTH; ++i) {
		ImageManager::AddTexture(fmt::format("FireballExplosion_{}", i), "data/resources/FireballExplosion.png", sf::IntRect({i * FIREBALLEXPLOSION_WIDTH, 0}, {FIREBALLEXPLOSION_WIDTH, FIREBALLEXPLOSION_HEIGHT}));
		FireballExplosionAnimName.push_back(fmt::format("FireballExplosion_{}", i));
	}
}
void AddFireballExplosion(const float x, const float y) {
	FireballExplosionList.emplace_back(sf::Vector2f(x, y), sf::Vector2f(16.f, 15.f));
	FireballExplosionList.back().setAnimation(0, 2, 24);
	FireballExplosionList.back().setAnimationSequence(FireballExplosionAnimName);
}
void DeleteFireballExplosion(const int i) {
	FireballExplosionList.erase(FireballExplosionList.begin() + i);
}
void DeleteFireballExplosion(const float x, const float y) {
	for (int i = 0; i < FireballExplosionList.size(); ++i) {
		if (FireballExplosionList[i].getCurrentPosition().x == x && FireballExplosionList[i].getCurrentPosition().y == y) {
			FireballExplosionList.erase(FireballExplosionList.begin() + i);
			break;
		}
	}
}
void DeleteAllFireballExplosion() {
	FireballExplosionList.clear();
}
void FireballExplosionStatusUpdate() {
	for (auto & i : FireballExplosionList) {
		if (i.isAnimationAtTheEnd())
			DeleteFireballExplosion(i.getCurrentPosition().x, i.getCurrentPosition().y);
	}
}
void FireballExplosionDraw() {
	if (FireballExplosionList.empty()) return;
	for (auto& i : FireballExplosionList) {
		if (!isOutScreen(i.getInterpolatedPosition().x, i.getInterpolatedPosition().y, 32, 32)) {
			i.AnimationUpdate(i.getInterpolatedPosition(), i.getOrigin());
			i.AnimationDraw();
		}
	}
}