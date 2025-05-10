#include "../headers/Object/Spike.hpp"
#include "../headers/Core/TextureManager.hpp"
#include "../headers/Core/Loading/Loading.hpp"
#include "../headers/Core/Animate/LocalAnimationManager.hpp"
#include "../headers/Core/Scroll.hpp"
#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Core/Collision/Collide.hpp"
#include "../headers/Object/Mario.hpp"

#include "../headers/Core/Loading/enum.hpp"

#include <vector>
#include <string>

std::vector<sf::Sprite> SpikeList;
std::vector<SpikeID> SpikeIDList;
std::vector<LocalAnimationManager> SpikeAnimationList;
std::vector<sf::FloatRect> SpikeHitboxList;

TextureManager SpikeTextureManager;

void SpikeInit() {
	SpikeTextureManager.Loadingtexture("data/resources/Spike/PiranhaGround.png", "Piranha_Ground", 0, 0, 128, 32);
	SpikeTextureManager.Loadingtexture("data/resources/Spike/Spike.png", "Spike_Normal", 0, 0, 32, 32);

	//SpikeTextureManager.LoadingAnimatedTexture(PIRANHA_GROUND_TEXTURE, "Piranha_Ground", 0, 3, 0, 32, 32);
	//SpikeTextureManager.LoadingAnimatedTexture(SPIKE_TEXTURE, "Spike_Normal", 0, 0, 0, 32, 32);
}
void DeleteAllSpike() {
	SpikeList.clear();
	SpikeIDList.clear();
	SpikeAnimationList.clear();
	SpikeHitboxList.clear();
}
void DeleteSpike(float x, float y) {
	for (int i = 0; i < SpikeList.size(); ++i) {
		if (SpikeList[i].getPosition().x == x && SpikeList[i].getPosition().y == y) {
			SpikeList.erase(SpikeList.begin() + i);
			SpikeIDList.erase(SpikeIDList.begin() + i);
			SpikeAnimationList.erase(SpikeAnimationList.begin() + i);
			SpikeHitboxList.erase(SpikeHitboxList.begin() + i);
		}
	}
}
void AddSpike(SpikeID ID, float x, float y) {
	sf::Sprite Init(tempTex);
	LocalAnimationManager InitAnimation;
	switch (ID) {
	case PIRANHA_GROUND:
		InitAnimation.setAnimation(0, 3, 32, 32, 0, 22);
		InitAnimation.setTexture(Init, SpikeTextureManager.GetTexture("Piranha_Ground"));
		break;
	case SPIKE_NORMAL:
		InitAnimation.setAnimation(0, 0, 32, 32, 0, 100);
		InitAnimation.setTexture(Init, SpikeTextureManager.GetTexture("Spike_Normal"));
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
			SpikeAnimationList[i].update(SpikeList[i]);
		}
		else {
			SpikeAnimationList[i].silentupdate();
			continue;
		}
	}
}
void SpikeUpdate() {
	for (int i = 0; i < SpikeList.size(); ++i) {
		if (!isOutScreen(SpikeList[i].getPosition().x, SpikeList[i].getPosition().y, 32, 32)) rTexture.draw(SpikeList[i]);
	}
}