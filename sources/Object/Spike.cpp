#include "Object/Spike.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Object/Mario.hpp"
#include "Core/Loading/enum.hpp"

plf::colony<MFCPP::Spike> SpikeList;
static std::vector<std::string> PiranhaGroundAnimName;
static bool SpikeDeleteGate = false;
void SpikeInit() {
	ImageManager::AddTexture("NormalSpike", "data/resources/Spike/Spike.png");
	for (int i = 0; i < 4; ++i) {
		ImageManager::AddTexture(fmt::format("PiranhaGreenGround_{}", i), "data/resources/Spike/PiranhaGround.png", sf::IntRect({i * 32, 0}, {32, 32}));
		PiranhaGroundAnimName.emplace_back(fmt::format("PiranhaGreenGround_{}", i));
	}
}
void DeleteAllSpike() {
	SpikeList.clear();
}
void DeleteSpikeIndex(const plf::colony<MFCPP::Spike>::colony_iterator<false>& it) {
	it->setDestroyed(true);
	SpikeDeleteGate = true;
}
void DeleteSpike(const float x, const float y) {
	for (auto it = SpikeList.begin(); it != SpikeList.end(); ++it) {
		if (it->getCurrentPosition().x == x && it->getCurrentPosition().y == y) {
			DeleteSpikeIndex(it);
		}
	}
}
void AddSpike(const SpikeID ID, const float x, const float y) {
	plf::colony<MFCPP::Spike>::colony_iterator<false> it;
	switch (ID) {
		case PIRANHA_GROUND:
			it = SpikeList.emplace(ID, sf::FloatRect({0.f, 0.f}, {32.f, 32.f}), sf::Vector2f(x, y), sf::Vector2f(0.f, 0.f));
			it->setAnimation(0, 3, 22);
			it->setAnimationSequence(PiranhaGroundAnimName);
			break;
		case SPIKE_NORMAL:
			it = SpikeList.emplace(ID, sf::FloatRect({0.f, 0.f}, {32.f, 32.f}), sf::Vector2f(x, y), sf::Vector2f(0.f, 0.f));
			it->setAnimation(0, 0, 100);
			it->AddAnimationSequence("NormalSpike");
			break;
		default: ;
	}
}
void SpikeStatusUpdate() {
	if (SpikeList.empty()) return;
	const sf::FloatRect player_hitbox = getGlobalHitbox(player.hitboxMain, player.property);
	for (auto it = SpikeList.begin(); it != SpikeList.end(); ++it) {
		if (it->isDestroyed()) continue;

		if (!isOutScreen(it->getCurrentPosition().x, it->getCurrentPosition().y, 32, 32)) {
			if (const sf::FloatRect spike_hitbox = getGlobalHitbox(it->getHitbox(), it->getCurrentPosition(), it->getOrigin()); isCollide(spike_hitbox, player_hitbox))
				PowerDown();
		}
	}
}
void SpikeDraw() {
	for (auto it = SpikeList.begin(); it != SpikeList.end(); ++it) {
		if (it->isDestroyed()) continue;
		if (!isOutScreen(it->getInterpolatedPosition().x, it->getInterpolatedPosition().y, 32, 32)) {
			it->AnimationUpdate(it->getInterpolatedPosition(), it->getOrigin());
			it->AnimationDraw();
		}
	}
}
void SpikeCleanup() {
	if (!SpikeDeleteGate) return;
	auto it = SpikeList.begin();
	while (it != SpikeList.end()) {
		if (it->isDestroyed()) ++it;
		else it = SpikeList.erase(it);
	}
	SpikeDeleteGate = false;
}