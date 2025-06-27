#include "Object/PiranhaAI.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Collision/Collide.hpp"
#include "Object/Mario.hpp"
#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Interpolation.hpp"
#include "Class/PiranhaAIClass.hpp"
static std::vector<std::string> PiranhaAnimName;
static constexpr int PIRANHA_IMAGE_WIDTH = 128;
static constexpr int PIRANHA_WIDTH = 64;
static constexpr int PIRANHA_HEIGHT = 64;
std::vector<MFCPP::PiranhaAI> PiranhaAIList;
static bool PiranhaAIDeleteGate = false;
void DeletePiranhaAIIndex(const int i) {
	PiranhaAIList[i].willDestroy(true);
	PiranhaAIDeleteGate = true;
}
void ClearPiranhaAI() {
	PiranhaAIList.clear();
}
void PiranhaAIInit() {
	ImageManager::AddImage("PiranhaGreenImage", "data/resources/Piranha/PiranhaGreen.png");
	for (int i = 0; i < PIRANHA_IMAGE_WIDTH / PIRANHA_WIDTH; ++i) {
		ImageManager::AddTexture("PiranhaGreenImage", sf::IntRect({i * PIRANHA_WIDTH, 0}, {PIRANHA_WIDTH, PIRANHA_HEIGHT}), "PiranhaGreen_" + std::to_string(i));
		PiranhaAnimName.emplace_back("PiranhaGreen_" + std::to_string(i));
	}
	//PiranhaAITextureManager.LoadingAnimatedTexture(PIRANHA_GREEN_TEXTURE, "PiranhaGreen", 0, 1, 0, 64, 64);
}
void SetPrevPiranhaAIPos() {
	for (auto &i : PiranhaAIList) {
		if (i.willBeDestroyed()) continue;
		i.setPreviousPosition(i.getCurrentPosition());
	}
}
void InterpolatePiranhaAIPos(const float alpha) {
	for (auto &i : PiranhaAIList) {
		if (i.willBeDestroyed()) continue;
		i.setInterpolatedPosition(linearInterpolation(i.getPreviousPosition(), i.getCurrentPosition(), alpha));
	}
}
void AddPiranha(const PiranhaID ID, const float x, const float y) {
	PiranhaAIList.emplace_back();
	switch (ID) {
		case GREEN:
			PiranhaAIList.back().setAnimation(0, 1, 14);
			PiranhaAIList.back().setAnimationSequence(PiranhaAnimName, PiranhaAnimName);
			PiranhaAIList.back().setSpeed(1.0f);
			PiranhaAIList.back().setStopTime(1.4f);
			PiranhaAIList.back().setDistanceAppear(80.0f);
			break;
	}
	PiranhaAIList.back().setID(ID);
	PiranhaAIList.back().setDisabled(true);
	PiranhaAIList.back().setHitbox(sf::FloatRect({ 16, 17 }, { 31, 47 }));
	PiranhaAIList.back().setOrigin({32, 63});

	PiranhaAIList.back().setCurrentPosition(sf::Vector2f({x, y + 64.0f}));
	PiranhaAIList.back().setPreviousPosition(PiranhaAIList.back().getCurrentPosition());
	PiranhaAIList.back().setInterpolatedPosition(PiranhaAIList.back().getCurrentPosition());
	PiranhaAIList.back().setPositionLimit(64.0f);
	PiranhaAIList.back().setPositionTemporary(64.0f);
	PiranhaAIList.back().setState(true);
	PiranhaAIList.back().setStop(false);
}
void PiranhaAIMovementUpdate(const float deltaTime) {
	for (auto & i : PiranhaAIList) {
		if (i.willBeDestroyed()) continue;

		if (!isOutScreen(i.getCurrentPosition().x, i.getCurrentPosition().y, 64, 64) && !i.isDisabled()) {
			if (!i.getStop()) {
				if (!i.getState()) {
					if (i.getPositionTemporary() <= i.getPositionLimit()) {
						i.setCurrentPosition({ i.getCurrentPosition().x, i.getCurrentPosition().y + i.getSpeed() * deltaTime });
						i.setPositionTemporary(i.getPositionTemporary() + i.getSpeed() * deltaTime);
					}
					else {
						i.setState(true);
						i.setCurrentPosition({ i.getCurrentPosition().x, i.getCurrentPosition().y + i.getPositionLimit() - i.getPositionTemporary() });
						i.setPositionTemporary(i.getPositionLimit());
						i.setStop(true);
						i.restartStopClock();
					}
				}
				else {
					if (i.getPositionTemporary() >= 0.0f) {
						i.setCurrentPosition({ i.getCurrentPosition().x, i.getCurrentPosition().y - i.getSpeed() * deltaTime });
						i.setPositionTemporary(i.getPositionTemporary() - i.getSpeed() * deltaTime);
					}
					else {
						i.setState(false);
						i.setCurrentPosition({ i.getCurrentPosition().x, i.getCurrentPosition().y - i.getPositionTemporary() });
						i.setPositionTemporary(0.0f);
						i.setStop(true);
						i.restartStopClock();
					}
				}
			}
			else {
				if (i.getStopClock().getElapsedTime().asSeconds() > i.getStopTime() && f_abs(
					    player.curr.x - i.getCurrentPosition().x) > i.getDistanceAppear() && i.getState()) i.setStop(false);
				else if (i.getStopClock().getElapsedTime().asSeconds() > i.getStopTime() && !i.getState()) i.setStop(false);
			}
		}
	}
}
void PiranhaAIStatusUpdate() {
	if (PiranhaAIList.empty()) return;
	const sf::FloatRect playerHitbox = getGlobalHitbox(player.hitboxMain, player.curr, player.property.getOrigin());
	for (auto & i : PiranhaAIList) {
		if (i.willBeDestroyed()) continue;

		if (!isOutScreen(i.getCurrentPosition().x, i.getCurrentPosition().y, 64, 64)) {
			if (i.isDisabled()) i.setDisabled(false);
		}
		else continue;
		if (sf::FloatRect PiranhaHitbox = getGlobalHitbox(i.getHitbox(), i.getCurrentPosition(), i.getOrigin()); isCollide(PiranhaHitbox, playerHitbox)) PowerDown();
	}
}
void PiranhaAIDraw() {
	for (auto & i : PiranhaAIList) {
		if (!isOutScreen(i.getInterpolatedPosition().x, i.getInterpolatedPosition().y, 64, 64) && !i.isDisabled()) {
			i.AnimationUpdate(i.getInterpolatedPosition(), i.getOrigin());
			i.AnimationDraw(window);
		}
	}
}
void PiranhaAICleanup() {
	if (!PiranhaAIDeleteGate) return;
	int i = 0;
	while (i < PiranhaAIList.size()) {
		if (!PiranhaAIList[i].willBeDestroyed()) ++i;
		else PiranhaAIList.erase(PiranhaAIList.begin() + i);
	}
	PiranhaAIDeleteGate = false;
}