#include "../headers/Object/PiranhaAI.hpp"
#include "../headers/Core/TextureManager.hpp"
#include "../headers/Core/Loading/Loading.hpp"
#include "../headers/Core/Loading/enum.hpp"
#include "../headers/Core/Animate/LocalAnimationManager.hpp"
#include "../headers/Core/Collision/Collide.hpp"
#include "../headers/Object/Mario.hpp"
#include "../headers/Core/Scroll.hpp"
#include "../headers/Core/WindowFrame.hpp"

#include "../resource.h"

#include <vector>
#include <string>

TextureManager PiranhaAITextureManager;

std::vector<sf::Sprite> PiranhaAIList;
std::vector<PiranhaID> PiranhaAITypeList;
std::vector<sf::FloatRect> PiranhaAIHitboxList;
std::vector<LocalAnimationManager> PiranhaAIAnimationList;
std::vector<std::string> PiranhaAIAnimationNameList;
std::vector<bool> PiranhaAIDisabledList;
std::vector<float> PiranhaAISpeedList;
std::vector<float> PiranhaAIPosLimitList;
std::vector<float> PiranhaAIPosTempList;
std::vector<bool> PiranhaAIStateList;
std::vector<float> PiranhaAIStopTimeList;
std::vector<sf::Clock> PiranhaAIStopTimerList;
std::vector<bool> PiranhaAIStopList;
std::vector<float> PiranhaAIDistanceAppearList;

void DeletePiranhaAI(float x, float y) {
	for (int i = 0; i < PiranhaAIList.size(); ++i) {
		if (PiranhaAIList[i].getPosition().x == x && PiranhaAIList[i].getPosition().y == y) {
			PiranhaAIList.erase(PiranhaAIList.begin() + i);
			PiranhaAITypeList.erase(PiranhaAITypeList.begin() + i);
			PiranhaAIHitboxList.erase(PiranhaAIHitboxList.begin() + i);
			PiranhaAIAnimationList.erase(PiranhaAIAnimationList.begin() + i);
			PiranhaAIAnimationNameList.erase(PiranhaAIAnimationNameList.begin() + i);
			PiranhaAIDisabledList.erase(PiranhaAIDisabledList.begin() + i);
			PiranhaAISpeedList.erase(PiranhaAISpeedList.begin() + i);
			PiranhaAIPosLimitList.erase(PiranhaAIPosLimitList.begin() + i);
			PiranhaAIPosTempList.erase(PiranhaAIPosTempList.begin() + i);
			PiranhaAIStateList.erase(PiranhaAIStateList.begin() + i);
			PiranhaAIStopTimeList.erase(PiranhaAIStopTimeList.begin() + i);
			PiranhaAIStopTimerList.erase(PiranhaAIStopTimerList.begin() + i);
			PiranhaAIStopList.erase(PiranhaAIStopList.begin() + i);
			PiranhaAIDistanceAppearList.erase(PiranhaAIDistanceAppearList.begin() + i);
			break;
		}
	}
}
void ClearPiranhaAI() {
	PiranhaAIList.clear();
	PiranhaAITypeList.clear();
	PiranhaAIHitboxList.clear();
	PiranhaAIAnimationList.clear();
	PiranhaAIAnimationNameList.clear();
	PiranhaAIDisabledList.clear();
	PiranhaAISpeedList.clear();
	PiranhaAIPosLimitList.clear();
	PiranhaAIPosTempList.clear();
	PiranhaAIStateList.clear();
	PiranhaAIStopTimeList.clear();
	PiranhaAIStopTimerList.clear();
	PiranhaAIStopList.clear();
	PiranhaAIDistanceAppearList.clear();
}
void PiranhaAIInit() {
	PiranhaAITextureManager.LoadingAnimatedTexture(PIRANHA_GREEN_TEXTURE, "PiranhaGreen", 0, 1, 0, 64, 64);
}
void AddPiranha(PiranhaID ID, float x, float y) {
	sf::Sprite Init;
	LocalAnimationManager InitAnimation;
	switch (ID) {
	case GREEN:
		InitAnimation.setAnimation(0, 1, 14);
		PiranhaAIAnimationNameList.push_back("PiranhaGreen");
		PiranhaAISpeedList.push_back(1.0f);
		PiranhaAIStopTimeList.push_back(1.4f);
		PiranhaAIDistanceAppearList.push_back(80.0f);
		break;
	}
	PiranhaAITypeList.push_back(ID);
	PiranhaAIDisabledList.push_back(true);
	PiranhaAIHitboxList.push_back(sf::FloatRect(16, 17, 31, 47));
	Init.setPosition(x, y);
	Init.setOrigin(32, 63);
	PiranhaAIAnimationList.push_back(InitAnimation);
	PiranhaAIPosLimitList.push_back(64.0f);
	PiranhaAIPosTempList.push_back(0.0f);
	PiranhaAIStateList.push_back(false);
	PiranhaAIStopTimerList.push_back(sf::Clock());
	PiranhaAIStopList.push_back(false);
	PiranhaAIList.push_back(Init);
}
void PiranhaAIMovementUpdate() {
	for (int i = 0; i < PiranhaAIList.size(); ++i) {
		if (!isOutScreen(PiranhaAIList[i].getPosition().x, PiranhaAIList[i].getPosition().y, 64, 64) && !PiranhaAIDisabledList[i]) {
			if (!PiranhaAIStopList[i]) {
				if (!PiranhaAIStateList[i]) {
					if (PiranhaAIPosTempList[i] <= PiranhaAIPosLimitList[i]) {
						PiranhaAIList[i].move(0.0f, PiranhaAISpeedList[i] * deltaTime);
						PiranhaAIPosTempList[i] += PiranhaAISpeedList[i] * deltaTime;
					}
					else {
						PiranhaAIStateList[i] = true;
						PiranhaAIList[i].move(0.0f, PiranhaAIPosLimitList[i] - PiranhaAIPosTempList[i]);
						PiranhaAIPosTempList[i] = PiranhaAIPosLimitList[i];
						PiranhaAIStopList[i] = true;
						PiranhaAIStopTimerList[i].restart().asSeconds();
					}
				}
				else {
					if (PiranhaAIPosTempList[i] >= 0.0f) {
						PiranhaAIList[i].move(0.0f, -PiranhaAISpeedList[i] * deltaTime);
						PiranhaAIPosTempList[i] -= PiranhaAISpeedList[i] * deltaTime;
					}
					else {
						PiranhaAIStateList[i] = false;
						PiranhaAIList[i].move(0.0f, -PiranhaAIPosTempList[i]);
						PiranhaAIPosTempList[i] = 0.0f;
						PiranhaAIStopList[i] = true;
						PiranhaAIStopTimerList[i].restart().asSeconds();
					}
				}
			}
			else {
				if (PiranhaAIStopTimerList[i].getElapsedTime().asSeconds() > PiranhaAIStopTimeList[i] && f_abs(player.property.getPosition().x - PiranhaAIList[i].getPosition().x) > PiranhaAIDistanceAppearList[i] && PiranhaAIStateList[i]) PiranhaAIStopList[i] = false;
				else if (PiranhaAIStopTimerList[i].getElapsedTime().asSeconds() > PiranhaAIStopTimeList[i] && !PiranhaAIStateList[i]) PiranhaAIStopList[i] = false;
			}
		}
	}
}
void PiranhaAIStatusUpdate() {
	sf::FloatRect playerHitbox = getGlobalHitbox(player.hitboxMain, player.property);
	for (int i = 0; i < PiranhaAIList.size(); ++i) {
		if (!isOutScreen(PiranhaAIList[i].getPosition().x, PiranhaAIList[i].getPosition().y, 64, 64) && PiranhaAIDisabledList[i]) PiranhaAIDisabledList[i] = false;
		if (isCollide(PiranhaAIHitboxList[i], PiranhaAIList[i], playerHitbox)) PowerDown();
	}
}
void PiranhaAIUpdate() {
	for (int i = 0; i < PiranhaAIList.size(); ++i) {
		if (!isOutScreen(PiranhaAIList[i].getPosition().x, PiranhaAIList[i].getPosition().y, 64, 64) && !PiranhaAIDisabledList[i]) {
			PiranhaAIAnimationList[i].update(PiranhaAIList[i], PiranhaAITextureManager.GetAnimatedTexture(PiranhaAIAnimationNameList[i]));
			window.draw(PiranhaAIList[i]);
		}
		else if (isOutScreen(PiranhaAIList[i].getPosition().x, PiranhaAIList[i].getPosition().y, 64, 64) && !PiranhaAIDisabledList[i]) {
			PiranhaAIAnimationList[i].silentupdate(PiranhaAIList[i]);
		}
	}
}