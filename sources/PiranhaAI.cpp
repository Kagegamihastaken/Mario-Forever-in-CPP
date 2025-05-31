#include "Object/PiranhaAI.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/Animate/LocalAnimationManager.hpp"
#include "Core/Collision/Collide.hpp"
#include "Object/Mario.hpp"
#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Interpolation.hpp"
#include <vector>
#include "Class/PiranhaAIClass.hpp"
static std::vector<std::string> PiranhaAnimName;
static constexpr int PIRANHA_IMAGE_WIDTH = 128;
static constexpr int PIRANHA_WIDTH = 64;
static constexpr int PIRANHA_HEIGHT = 64;
std::vector<PiranhaAI> PiranhaAIList;
void DeletePiranhaAI(const float x, const float y) {
	for (unsigned int i = 0; i < PiranhaAIList.size(); ++i) {
		if (PiranhaAIList[i].getPosition().x == x && PiranhaAIList[i].getPosition().y == y) {
			PiranhaAIList.erase(PiranhaAIList.begin() + i);
			break;
		}
	}
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
		i.setPreviousPosition(i.getCurrentPosition());
	}
}
void InterpolatePiranhaAIPos(const float alpha) {
	for (auto &i : PiranhaAIList) {
		i.setPosition(linearInterpolation(i.getPreviousPosition(), i.getCurrentPosition(), alpha));
	}
}
void AddPiranha(const PiranhaID ID, const float x, const float y) {
	PiranhaAIList.emplace_back(sf::Vector2f({x, y + 64.0f}));
	switch (ID) {
		case GREEN:
			PiranhaAIList.back().m_animation.setAnimation(0, 1, 14);
			PiranhaAIList.back().m_animation.SetSequence(PiranhaAnimName, PiranhaAnimName);
			PiranhaAIList.back().setSpeed(1.0f);
			PiranhaAIList.back().setStopTime(1.4f);
			PiranhaAIList.back().setDistanceAppear(80.0f);
			break;
	}
	PiranhaAIList.back().setTextureRect(sf::IntRect({0, 0}, {64, 64}), true);
	PiranhaAIList.back().setID(ID);
	PiranhaAIList.back().setDisabled(true);
	PiranhaAIList.back().setHitbox(sf::FloatRect({ 16, 17 }, { 31, 47 }));
	PiranhaAIList.back().setOrigin({32, 63});
	PiranhaAIList.back().setPreviousPosition(PiranhaAIList.back().getPosition());
	PiranhaAIList.back().setCurrentPosition(PiranhaAIList.back().getPosition());
	PiranhaAIList.back().setPositionLimit(64.0f);
	PiranhaAIList.back().setPositionTemporary(64.0f);
	PiranhaAIList.back().setState(true);
	PiranhaAIList.back().setStop(false);
}
void PiranhaAIMovementUpdate(const float deltaTime) {
	for (auto & i : PiranhaAIList) {
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
		if (!isOutScreen(i.getCurrentPosition().x, i.getCurrentPosition().y, 64, 64)) {
			if (i.isDisabled()) i.setDisabled(false);
		}
		else continue;
		if (sf::FloatRect PiranhaHitbox = getGlobalHitbox(i.getHitbox(), i.getCurrentPosition(), i.getOrigin()); isCollide(PiranhaHitbox, playerHitbox)) PowerDown();
	}
}
void PiranhaAIUpdate() {
	for (auto & i : PiranhaAIList) {
		if (!isOutScreen(i.getPosition().x, i.getPosition().y, 64, 64) && !i.isDisabled()) {
			i.m_animation.AnimationUpdate(i.getPosition(), i.getOrigin());
			i.m_animation.AnimationDraw(window);
			//i.m_animation.getCurrentAnimationName();
			//i.setTexture(ImageManager::GetTexture(i.m_animation.getCurrentAnimationName()));
			//i.setTextureRect(i.m_animation.getAnimationTextureRect());
			//i.m_animation.silentupdate();
			//window.draw(i);
		}
		else if (isOutScreen(i.getPosition().x, i.getPosition().y, 64, 64) && !i.isDisabled()) {
			i.m_animation.AnimationUpdate(i.getPosition(), i.getOrigin());
		}
	}
}