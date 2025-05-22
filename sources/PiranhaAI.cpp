#include "Object/PiranhaAI.hpp"
#include "Core/TextureManager.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/Animate/LocalAnimationManager.hpp"
#include "Core/Collision/Collide.hpp"
#include "Object/Mario.hpp"
#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Interpolation.hpp"
#include <vector>
#include "Class/PiranhaAIClass.hpp"

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
	TextureManager::Loadingtexture("data/resources/Piranha/PiranhaGreen.png", "PiranhaGreen", 0, 0, 128, 64);
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
	PiranhaAIList.push_back(sf::Vector2f({x, y + 64.0f}));
	LocalAnimationManager InitAnimation;
	switch (ID) {
		case GREEN:
			PiranhaAIList.back().m_animation.setAnimation(0, 1, 64, 64, 0, 14);
			PiranhaAIList.back().setTexture(*TextureManager::GetTexture("PiranhaGreen"));
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
	for (int i = 0; i < PiranhaAIList.size(); ++i) {
		if (!isOutScreen(PiranhaAIList[i].getCurrentPosition().x, PiranhaAIList[i].getCurrentPosition().y, 64, 64) && !PiranhaAIList[i].isDisabled()) {
			if (!PiranhaAIList[i].getStop()) {
				if (!PiranhaAIList[i].getState()) {
					if (PiranhaAIList[i].getPositionTemporary() <= PiranhaAIList[i].getPositionLimit()) {
						PiranhaAIList[i].setCurrentPosition({ PiranhaAIList[i].getCurrentPosition().x, PiranhaAIList[i].getCurrentPosition().y + PiranhaAIList[i].getSpeed() * deltaTime });
						PiranhaAIList[i].setPositionTemporary(PiranhaAIList[i].getPositionTemporary() + PiranhaAIList[i].getSpeed() * deltaTime);
					}
					else {
						PiranhaAIList[i].setState(true);
						PiranhaAIList[i].setCurrentPosition({ PiranhaAIList[i].getCurrentPosition().x, PiranhaAIList[i].getCurrentPosition().y + PiranhaAIList[i].getPositionLimit() - PiranhaAIList[i].getPositionTemporary() });
						PiranhaAIList[i].setPositionTemporary(PiranhaAIList[i].getPositionLimit());
						PiranhaAIList[i].setStop(true);
						PiranhaAIList[i].restartStopClock();
					}
				}
				else {
					if (PiranhaAIList[i].getPositionTemporary() >= 0.0f) {
						PiranhaAIList[i].setCurrentPosition({ PiranhaAIList[i].getCurrentPosition().x, PiranhaAIList[i].getCurrentPosition().y - PiranhaAIList[i].getSpeed() * deltaTime });
						PiranhaAIList[i].setPositionTemporary(PiranhaAIList[i].getPositionTemporary() - PiranhaAIList[i].getSpeed() * deltaTime);
					}
					else {
						PiranhaAIList[i].setState(false);
						PiranhaAIList[i].setCurrentPosition({ PiranhaAIList[i].getCurrentPosition().x, PiranhaAIList[i].getCurrentPosition().y - PiranhaAIList[i].getPositionTemporary() });
						PiranhaAIList[i].setPositionTemporary(0.0f);
						PiranhaAIList[i].setStop(true);
						PiranhaAIList[i].restartStopClock();
					}
				}
			}
			else {
				if (PiranhaAIList[i].getStopClock().getElapsedTime().asSeconds() > PiranhaAIList[i].getStopTime() && f_abs(player.curr.x - PiranhaAIList[i].getCurrentPosition().x) > PiranhaAIList[i].getDistanceAppear() && PiranhaAIList[i].getState()) PiranhaAIList[i].setStop(false);
				else if (PiranhaAIList[i].getStopClock().getElapsedTime().asSeconds() > PiranhaAIList[i].getStopTime() && !PiranhaAIList[i].getState()) PiranhaAIList[i].setStop(false);
			}
		}
	}
}
void PiranhaAIStatusUpdate() {
	if (PiranhaAIList.empty()) return;
	const sf::FloatRect playerHitbox = getGlobalHitbox(player.hitboxMain, player.curr, player.property.getOrigin());
	for (int i = 0; i < PiranhaAIList.size(); ++i) {
		if (!isOutScreen(PiranhaAIList[i].getCurrentPosition().x, PiranhaAIList[i].getCurrentPosition().y, 64, 64)) {
			if (PiranhaAIList[i].isDisabled()) PiranhaAIList[i].setDisabled(false);
		}
		else continue;
		if (sf::FloatRect PiranhaHitbox = getGlobalHitbox(PiranhaAIList[i].getHitbox(), PiranhaAIList[i].getCurrentPosition(), PiranhaAIList[i].getOrigin()); isCollide(PiranhaHitbox, playerHitbox)) PowerDown();
	}
}
void PiranhaAIUpdate() {
	for (int i = 0; i < PiranhaAIList.size(); ++i) {
		if (!isOutScreen(PiranhaAIList[i].getPosition().x, PiranhaAIList[i].getPosition().y, 64, 64) && !PiranhaAIList[i].isDisabled()) {
			PiranhaAIList[i].setTextureRect(PiranhaAIList[i].m_animation.getAnimationTextureRect());
			PiranhaAIList[i].m_animation.silentupdate();
			rObject.draw(PiranhaAIList[i]);
		}
		else if (isOutScreen(PiranhaAIList[i].getPosition().x, PiranhaAIList[i].getPosition().y, 64, 64) && !PiranhaAIList[i].isDisabled()) {
			PiranhaAIList[i].m_animation.silentupdate();
		}
	}
}