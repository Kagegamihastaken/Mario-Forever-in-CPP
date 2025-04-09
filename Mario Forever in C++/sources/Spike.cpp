#include "../headers/Object/Spike.hpp"
#include "../headers/Core/TextureManager.hpp"
#include "../headers/Core/Loading/Loading.hpp"
#include "../headers/Core/Animate/LocalAnimationManager.hpp"
#include "../headers/Core/Scroll.hpp"
#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Core/Collision/Collide.hpp"
#include "../headers/Object/Mario.hpp"

#include "../headers/Core/Loading/enum.hpp"
#include "../resource.h"

#include <vector>
#include <string>

std::vector<sf::Sprite> SpikeList;
std::vector<SpikeID> SpikeIDList;
std::vector<LocalAnimationManager> SpikeAnimationList;
std::vector<std::string> SpikeAnimationNameList;
std::vector<sf::FloatRect> SpikeHitboxList;

TextureManager SpikeTextureManager;

void SpikeInit() {
	SpikeTextureManager.LoadingAnimatedTexture(PIRANHA_GROUND_TEXTURE, "Piranha_Ground", 0, 3, 0, 32, 32);
	SpikeTextureManager.LoadingAnimatedTexture(SPIKE_TEXTURE, "Spike_Normal", 0, 0, 0, 32, 32);
}
void DeleteAllSpike() {
	SpikeList.clear();
	SpikeIDList.clear();
	SpikeAnimationList.clear();
	SpikeAnimationNameList.clear();
	SpikeHitboxList.clear();
}
void DeleteSpike(float x, float y) {
	for (int i = 0; i < SpikeList.size(); ++i) {
		if (SpikeList[i].getPosition().x == x && SpikeList[i].getPosition().y == y) {
			SpikeList.erase(SpikeList.begin() + i);
			SpikeIDList.erase(SpikeIDList.begin() + i);
			SpikeAnimationList.erase(SpikeAnimationList.begin() + i);
			SpikeAnimationNameList.erase(SpikeAnimationNameList.begin() + i);
			SpikeHitboxList.erase(SpikeHitboxList.begin() + i);
		}
	}
}
void AddSpike(SpikeID ID, float x, float y) {
	sf::Sprite Init(tempTex);
	LocalAnimationManager InitAnimation;
	switch (ID) {
	case PIRANHA_GROUND:
		InitAnimation.setAnimation(0, 3, 22);
		SpikeAnimationNameList.push_back("Piranha_Ground");
		break;
	case SPIKE_NORMAL:
		InitAnimation.setAnimation(0, 0, 100);
		SpikeAnimationNameList.push_back("Spike_Normal");
		break;
	}
	SpikeHitboxList.push_back(sf::FloatRect({ 0, 0 }, { 32, 32 }));
	Init.setPosition({ x, y });
	SpikeList.push_back(Init);
	SpikeIDList.push_back(ID);
	SpikeAnimationList.push_back(InitAnimation);
}
void SpikeStatusUpdate() {
	if (SpikeList.size() == 0) return;
	sf::FloatRect playerHitbox = getGlobalHitbox(player.hitboxMain, player.property);
	for (int i = 0; i < SpikeList.size(); ++i) {
		if (!isOutScreen(SpikeList[i].getPosition().x, SpikeList[i].getPosition().y, 32, 32)) {
			if (isCollide(SpikeHitboxList[i], SpikeList[i], playerHitbox)) PowerDown();
			SpikeAnimationList[i].update(SpikeList[i], SpikeTextureManager.GetAnimatedTexture(SpikeAnimationNameList[i]));
		}
		else {
			SpikeAnimationList[i].silentupdate(SpikeList[i]);
			continue;
		}
	}
}
void SpikeUpdate() {
	for (int i = 0; i < SpikeList.size(); ++i) {
		if (!isOutScreen(SpikeList[i].getPosition().x, SpikeList[i].getPosition().y, 32, 32)) window.draw(SpikeList[i]);
	}
}