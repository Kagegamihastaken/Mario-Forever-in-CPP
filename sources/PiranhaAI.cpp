#include "../headers/Object/PiranhaAI.hpp"
#include "../headers/Core/TextureManager.hpp"
#include "../headers/Core/Loading/Loading.hpp"
#include "../headers/Core/Loading/enum.hpp"
#include "../headers/Core/Animate/LocalAnimationManager.hpp"
#include "../headers/Core/Collision/Collide.hpp"
#include "../headers/Object/Mario.hpp"
#include "../headers/Core/Scroll.hpp"
#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Core/Interpolation.hpp"

#include <vector>
#include <string>

TextureManager PiranhaAITextureManager;

std::vector<sf::Sprite> PiranhaAIList;
std::vector<sf::Vector2f> PiranhaAIPosCurrList;
std::vector<sf::Vector2f> PiranhaAIPosPrevList;
std::vector<PiranhaID> PiranhaAITypeList;
std::vector<sf::FloatRect> PiranhaAIHitboxList;
std::vector<LocalAnimationManager> PiranhaAIAnimationList;
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
			PiranhaAIDisabledList.erase(PiranhaAIDisabledList.begin() + i);
			PiranhaAISpeedList.erase(PiranhaAISpeedList.begin() + i);
			PiranhaAIPosLimitList.erase(PiranhaAIPosLimitList.begin() + i);
			PiranhaAIPosTempList.erase(PiranhaAIPosTempList.begin() + i);
			PiranhaAIStateList.erase(PiranhaAIStateList.begin() + i);
			PiranhaAIStopTimeList.erase(PiranhaAIStopTimeList.begin() + i);
			PiranhaAIStopTimerList.erase(PiranhaAIStopTimerList.begin() + i);
			PiranhaAIStopList.erase(PiranhaAIStopList.begin() + i);
			PiranhaAIDistanceAppearList.erase(PiranhaAIDistanceAppearList.begin() + i);
			PiranhaAIPosCurrList.erase(PiranhaAIPosCurrList.begin() + i);
			PiranhaAIPosPrevList.erase(PiranhaAIPosPrevList.begin() + i);
			break;
		}
	}
}
void ClearPiranhaAI() {
	PiranhaAIList.clear();
	PiranhaAITypeList.clear();
	PiranhaAIHitboxList.clear();
	PiranhaAIAnimationList.clear();
	PiranhaAIDisabledList.clear();
	PiranhaAISpeedList.clear();
	PiranhaAIPosLimitList.clear();
	PiranhaAIPosTempList.clear();
	PiranhaAIStateList.clear();
	PiranhaAIStopTimeList.clear();
	PiranhaAIStopTimerList.clear();
	PiranhaAIStopList.clear();
	PiranhaAIDistanceAppearList.clear();
	PiranhaAIPosCurrList.clear();
	PiranhaAIPosPrevList.clear();
}
void PiranhaAIInit() {
	PiranhaAITextureManager.Loadingtexture("data/resources/Piranha/PiranhaGreen.png", "PiranhaGreen", 0, 0, 128, 64);
	//PiranhaAITextureManager.LoadingAnimatedTexture(PIRANHA_GREEN_TEXTURE, "PiranhaGreen", 0, 1, 0, 64, 64);
}
void SetPrevPiranhaAIPos() {
	for (int i = 0; i < PiranhaAIList.size(); i++) {
		PiranhaAIPosPrevList[i] = PiranhaAIPosCurrList[i];
	}
}
void InterpolatePiranhaAIPos(float alpha) {
	for (int i = 0; i < PiranhaAIList.size(); i++) {
		PiranhaAIList[i].setPosition(linearInterpolation(PiranhaAIPosPrevList[i], PiranhaAIPosCurrList[i], alpha));
	}
}
void AddPiranha(PiranhaID ID, float x, float y) {
	sf::Sprite Init(tempTex);
	LocalAnimationManager InitAnimation;
	switch (ID) {
	case GREEN:
		InitAnimation.setAnimation(0, 1, 64, 64, 0, 14);
		InitAnimation.setTexture(Init, PiranhaAITextureManager.GetTexture("PiranhaGreen"));
		//PiranhaAIAnimationNameList.push_back("PiranhaGreen");
		PiranhaAISpeedList.push_back(1.0f);
		PiranhaAIStopTimeList.push_back(1.4f);
		PiranhaAIDistanceAppearList.push_back(80.0f);
		break;
	}
	PiranhaAITypeList.push_back(ID);
	PiranhaAIDisabledList.push_back(true);
	PiranhaAIHitboxList.push_back(sf::FloatRect({ 16, 17 }, { 31, 47 }));
	Init.setPosition({ x, y + 64.0f });
	Init.setOrigin({ 32, 63 });
	PiranhaAIPosCurrList.push_back(Init.getPosition());
	PiranhaAIPosPrevList.push_back(Init.getPosition());
	PiranhaAIAnimationList.push_back(InitAnimation);
	PiranhaAIPosLimitList.push_back(64.0f);
	PiranhaAIPosTempList.push_back(64.0f);
	PiranhaAIStateList.push_back(true);
	PiranhaAIStopTimerList.push_back(sf::Clock());
	PiranhaAIStopList.push_back(false);
	PiranhaAIList.push_back(Init);
}
void PiranhaAIMovementUpdate(float deltaTime) {
	for (int i = 0; i < PiranhaAIList.size(); ++i) {
		if (!isOutScreen(PiranhaAIPosCurrList[i].x, PiranhaAIPosCurrList[i].y, 64, 64) && !PiranhaAIDisabledList[i]) {
			if (!PiranhaAIStopList[i]) {
				if (!PiranhaAIStateList[i]) {
					if (PiranhaAIPosTempList[i] <= PiranhaAIPosLimitList[i]) {
						PiranhaAIPosCurrList[i] = { PiranhaAIPosCurrList[i].x, PiranhaAIPosCurrList[i].y + PiranhaAISpeedList[i] * deltaTime };
						PiranhaAIPosTempList[i] += PiranhaAISpeedList[i] * deltaTime;
					}
					else {
						PiranhaAIStateList[i] = true;
						PiranhaAIPosCurrList[i] = { PiranhaAIPosCurrList[i].x, PiranhaAIPosCurrList[i].y + PiranhaAIPosLimitList[i] - PiranhaAIPosTempList[i] };
						PiranhaAIPosTempList[i] = PiranhaAIPosLimitList[i];
						PiranhaAIStopList[i] = true;
						PiranhaAIStopTimerList[i].restart();
					}
				}
				else {
					if (PiranhaAIPosTempList[i] >= 0.0f) {
						PiranhaAIPosCurrList[i] = { PiranhaAIPosCurrList[i].x, PiranhaAIPosCurrList[i].y - PiranhaAISpeedList[i] * deltaTime };
						PiranhaAIPosTempList[i] -= PiranhaAISpeedList[i] * deltaTime;
					}
					else {
						PiranhaAIStateList[i] = false;
						PiranhaAIPosCurrList[i] = { PiranhaAIPosCurrList[i].x, PiranhaAIPosCurrList[i].y - PiranhaAIPosTempList[i] };
						PiranhaAIPosTempList[i] = 0.0f;
						PiranhaAIStopList[i] = true;
						PiranhaAIStopTimerList[i].restart();
					}
				}
			}
			else {
				if (PiranhaAIStopTimerList[i].getElapsedTime().asSeconds() > PiranhaAIStopTimeList[i] && f_abs(player.curr.x - PiranhaAIPosCurrList[i].x) > PiranhaAIDistanceAppearList[i] && PiranhaAIStateList[i]) PiranhaAIStopList[i] = false;
				else if (PiranhaAIStopTimerList[i].getElapsedTime().asSeconds() > PiranhaAIStopTimeList[i] && !PiranhaAIStateList[i]) PiranhaAIStopList[i] = false;
			}
		}
	}
}
void PiranhaAIStatusUpdate() {
	if (PiranhaAIList.size() == 0) return;
	sf::FloatRect playerHitbox = getGlobalHitbox(player.hitboxMain, player.property);
	for (int i = 0; i < PiranhaAIList.size(); ++i) {
		if (!isOutScreen(PiranhaAIPosCurrList[i].x, PiranhaAIPosCurrList[i].y, 64, 64)) {
			if (PiranhaAIDisabledList[i]) PiranhaAIDisabledList[i] = false;
		}
		else continue;
		if (isCollide(PiranhaAIHitboxList[i], PiranhaAIList[i], PiranhaAIPosCurrList[i], playerHitbox)) PowerDown();
	}
}
void PiranhaAIUpdate() {
	for (int i = 0; i < PiranhaAIList.size(); ++i) {
		if (!isOutScreen(PiranhaAIList[i].getPosition().x, PiranhaAIList[i].getPosition().y, 64, 64) && !PiranhaAIDisabledList[i]) {
			PiranhaAIAnimationList[i].update(PiranhaAIList[i]);
			rTexture.draw(PiranhaAIList[i]);
		}
		else if (isOutScreen(PiranhaAIList[i].getPosition().x, PiranhaAIList[i].getPosition().y, 64, 64) && !PiranhaAIDisabledList[i]) {
			PiranhaAIAnimationList[i].silentupdate();
		}
	}
}