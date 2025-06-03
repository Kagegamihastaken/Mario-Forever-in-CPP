#include <SFML/Graphics.hpp>
#include "Object/Spike.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Animate/LocalAnimationManager.hpp"
#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Object/Mario.hpp"

#include "Core/Loading/enum.hpp"

#include <vector>

std::vector<sf::Sprite> SpikeList;
std::vector<SpikeID> SpikeIDList;
std::vector<LocalAnimationManager> SpikeAnimationList;
std::vector<sf::FloatRect> SpikeHitboxList;
static std::vector<std::string> PiranhaGroundAnimName;
void SpikeInit() {
	ImageManager::AddImage("NormalSpikeImage", "data/resources/Spike/Spike.png");
	ImageManager::AddTexture("NormalSpikeImage", "NormalSpike");
	ImageManager::AddImage("PiranhaGroundImage", "data/resources/Spike/PiranhaGround.png");
	for (int i = 0; i < 4; ++i) {
		ImageManager::AddTexture("PiranhaGroundImage", sf::IntRect({i * 32, 0}, {32, 32}), "PiranhaGreenGround_" + std::to_string(i));
		PiranhaGroundAnimName.emplace_back("PiranhaGreenGround_" + std::to_string(i));
	}
	//SpikeTextureManager.Loadingtexture("data/resources/Spike/Spike.png", "Spike_Normal", 0, 0, 32, 32);

	//SpikeTextureManager.LoadingAnimatedTexture(PIRANHA_GROUND_TEXTURE, "Piranha_Ground", 0, 3, 0, 32, 32);
	//SpikeTextureManager.LoadingAnimatedTexture(SPIKE_TEXTURE, "Spike_Normal", 0, 0, 0, 32, 32);
}
void DeleteAllSpike() {
	SpikeList.clear();
	SpikeIDList.clear();
	SpikeAnimationList.clear();
	SpikeHitboxList.clear();
}
void DeleteSpike(const float x, const float y) {
	for (int i = 0; i < SpikeList.size(); ++i) {
		if (SpikeList[i].getPosition().x == x && SpikeList[i].getPosition().y == y) {
			SpikeList.erase(SpikeList.begin() + i);
			SpikeIDList.erase(SpikeIDList.begin() + i);
			SpikeAnimationList.erase(SpikeAnimationList.begin() + i);
			SpikeHitboxList.erase(SpikeHitboxList.begin() + i);
		}
	}
}
void AddSpike(const SpikeID ID, float x, float y) {
	sf::Sprite Init(tempTex);
	LocalAnimationManager InitAnimation;
	switch (ID) {
	case PIRANHA_GROUND:
		InitAnimation.setAnimation(0, 3, 22);
		InitAnimation.SetSequence(PiranhaGroundAnimName, PiranhaGroundAnimName);
		break;
	case SPIKE_NORMAL:
		InitAnimation.setAnimation(0, 0, 100);
		InitAnimation.AddSequence("NormalSpike", "NormalSpike");
		break;
	}
	SpikeHitboxList.push_back(sf::FloatRect({ 0, 0 }, { 32, 32 }));
	Init.setPosition({ x, y });
	SpikeList.push_back(Init);
	SpikeIDList.push_back(ID);
	SpikeAnimationList.push_back(InitAnimation);
}
void SpikeStatusUpdate() {
	if (SpikeList.empty()) return;
	const sf::FloatRect playerHitbox = getGlobalHitbox(player.hitboxMain, player.property);
	for (int i = 0; i < SpikeList.size(); ++i) {
		if (!isOutScreen(SpikeList[i].getPosition().x, SpikeList[i].getPosition().y, 32, 32)) {
			if (isCollide(SpikeHitboxList[i], SpikeList[i], playerHitbox)) PowerDown();
		}
	}
}
void SpikeUpdate() {
	for (int i = 0; i < SpikeList.size(); ++i) {
		if (!isOutScreen(SpikeList[i].getPosition().x, SpikeList[i].getPosition().y, 32, 32)) {
			SpikeAnimationList[i].AnimationUpdate(SpikeList[i].getPosition(), SpikeList[i].getOrigin());
			SpikeAnimationList[i].AnimationDraw(window);
		}
	}
}