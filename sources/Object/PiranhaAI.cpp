#include "Object/PiranhaAI.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Collision/Collide.hpp"
#include "Object/Mario.hpp"
#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Class/PiranhaAIClass.hpp"
static std::vector<std::string> GreenPiranhaUpAnimName;
static std::vector<std::string> GreenPiranhaDownAnimName;
static std::vector<std::string> GreenPiranhaRightAnimName;
static std::vector<std::string> GreenPiranhaLeftAnimName;
static constexpr int PIRANHA_IMAGE_WIDTH = 256;
static constexpr int PIRANHA_WIDTH = 64;
static constexpr int PIRANHA_HEIGHT = 64;
std::vector<MFCPP::PiranhaAI> PiranhaAIList;
static bool PiranhaAIDeleteGate = false;
void DeletePiranhaAIIndex(const int i) {
	PiranhaAIList[i].setDestroyed(true);
	PiranhaAIDeleteGate = true;
}
void ClearPiranhaAI() {
	PiranhaAIList.clear();
}
void PiranhaAIInit() {
	for (int i = 0; i < PIRANHA_IMAGE_WIDTH / PIRANHA_WIDTH; ++i) {
		ImageManager::AddTexture(fmt::format("GreenPiranhaGreenUp_{}", i), "data/resources/Piranha/GreenPiranhaGreenUp.png", sf::IntRect({i * PIRANHA_WIDTH, 0}, {PIRANHA_WIDTH, PIRANHA_HEIGHT}));
		GreenPiranhaUpAnimName.emplace_back(fmt::format("GreenPiranhaGreenUp_{}", i));
		ImageManager::AddTexture(fmt::format("GreenPiranhaGreenDown_{}", i), "data/resources/Piranha/GreenPiranhaGreenDown.png", sf::IntRect({i * PIRANHA_WIDTH, 0}, {PIRANHA_WIDTH, PIRANHA_HEIGHT}));
		GreenPiranhaDownAnimName.emplace_back(fmt::format("GreenPiranhaGreenDown_{}", i));
		ImageManager::AddTexture(fmt::format("GreenPiranhaGreenRight_{}", i), "data/resources/Piranha/GreenPiranhaGreenRight.png", sf::IntRect({i * PIRANHA_WIDTH, 0}, {PIRANHA_WIDTH, PIRANHA_HEIGHT}));
		GreenPiranhaRightAnimName.emplace_back(fmt::format("GreenPiranhaGreenRight_{}", i));
		ImageManager::AddTexture(fmt::format("GreenPiranhaGreenLeft_{}", i), "data/resources/Piranha/GreenPiranhaGreenLeft.png", sf::IntRect({i * PIRANHA_WIDTH, 0}, {PIRANHA_WIDTH, PIRANHA_HEIGHT}));
		GreenPiranhaLeftAnimName.emplace_back(fmt::format("GreenPiranhaGreenLeft_{}", i));
	}
}
void SetPrevPiranhaAIPos() {
	for (auto &i : PiranhaAIList) {
		if (i.isDestroyed()) continue;
		i.setPreviousPosition(i.getCurrentPosition());
	}
}
void InterpolatePiranhaAIPos(const float alpha) {
	for (auto &i : PiranhaAIList) {
		if (i.isDestroyed()) continue;
		i.setInterpolatedPosition(linearInterpolation(i.getPreviousPosition(), i.getCurrentPosition(), alpha));
	}
}
void AddPiranha(const PiranhaID ID, const PiranhaDirection dir, const float x, const float y) {
	switch (ID) {
		case GREEN:
			switch (dir) {
			case PIRANHA_UP:
				PiranhaAIList.emplace_back(ID, PIRANHA_UP, 1.f, 1.4f, sf::FloatRect({ 16, 17 }, { 31, 47 }), sf::Vector2f(x, y), sf::Vector2f(32, 63));
				PiranhaAIList.back().setAnimation(0, 3, 24);
				PiranhaAIList.back().setAnimationSequence(GreenPiranhaUpAnimName);
				break;
			case PIRANHA_DOWN:
				PiranhaAIList.emplace_back(ID, PIRANHA_DOWN, 1.f, 1.4f, sf::FloatRect({ 16, 0 }, { 31, 47 }), sf::Vector2f(x, y), sf::Vector2f(32, 0));
				PiranhaAIList.back().setAnimation(0, 3, 24);
				PiranhaAIList.back().setAnimationSequence(GreenPiranhaDownAnimName);
				break;
			case PIRANHA_RIGHT:
				PiranhaAIList.emplace_back(ID, PIRANHA_RIGHT, 1.f, 1.4f, sf::FloatRect({ 17, 17 }, { 47, 31 }), sf::Vector2f(x, y), sf::Vector2f(63, 32));
				PiranhaAIList.back().setAnimation(0, 3, 24);
				PiranhaAIList.back().setAnimationSequence(GreenPiranhaRightAnimName);
				break;
			case PIRANHA_LEFT:
				PiranhaAIList.emplace_back(ID, PIRANHA_LEFT, 1.f, 1.4f, sf::FloatRect({ 0, 17 }, { 47, 31 }), sf::Vector2f(x, y), sf::Vector2f(0, 32));
				PiranhaAIList.back().setAnimation(0, 3, 24);
				PiranhaAIList.back().setAnimationSequence(GreenPiranhaLeftAnimName);
				break;
			default: ;
			}
			break;
		default: ;
	}
}
void PiranhaAIMovementUpdate(const float deltaTime) {
	for (auto & i : PiranhaAIList) {
		if (i.isDestroyed()) continue;

		if (!isOutScreen(i.getCurrentPosition().x, i.getCurrentPosition().y, 64, 64) && !i.isDisabled()) {
			if (!i.getStop()) {
				if (!i.getState()) {
					if (i.getPositionTemporary() <= i.getPositionLimit()) {
						switch (i.getDirection()) {
							case PIRANHA_UP:
								i.setCurrentPosition({ i.getCurrentPosition().x, i.getCurrentPosition().y + i.getSpeed() * deltaTime });
								break;
							case PIRANHA_DOWN:
								i.setCurrentPosition({ i.getCurrentPosition().x, i.getCurrentPosition().y - i.getSpeed() * deltaTime });
								break;
							case PIRANHA_RIGHT:
								i.setCurrentPosition({ i.getCurrentPosition().x + i.getSpeed() * deltaTime, i.getCurrentPosition().y });
								break;
							case PIRANHA_LEFT:
								i.setCurrentPosition({ i.getCurrentPosition().x - i.getSpeed() * deltaTime, i.getCurrentPosition().y });
								break;
							default: ;
						}
						i.setPositionTemporary(i.getPositionTemporary() + i.getSpeed() * deltaTime);
					}
					else {
						i.setState(true);
						//Set back to original
						i.setCurrentPosition(i.getCurrentPosition() + sf::Vector2f((i.getPositionLimit() - i.getPositionTemporary()) * static_cast<float>(i.getDirection() == PIRANHA_RIGHT || i.getDirection() == PIRANHA_LEFT), (i.getPositionLimit() - i.getPositionTemporary()) * static_cast<float>(i.getDirection() == PIRANHA_UP || i.getDirection() == PIRANHA_DOWN)));
						i.setPositionTemporary(i.getPositionLimit());
						i.setStop(true);
						i.restartStopClock();
					}
				}
				else {
					if (i.getPositionTemporary() >= 0.0f) {
						switch (i.getDirection()) {
							case PIRANHA_UP:
								i.setCurrentPosition({ i.getCurrentPosition().x, i.getCurrentPosition().y - i.getSpeed() * deltaTime });
								break;
							case PIRANHA_DOWN:
								i.setCurrentPosition({ i.getCurrentPosition().x, i.getCurrentPosition().y + i.getSpeed() * deltaTime });
								break;
							case PIRANHA_RIGHT:
								i.setCurrentPosition({ i.getCurrentPosition().x - i.getSpeed() * deltaTime, i.getCurrentPosition().y });
								break;
							case PIRANHA_LEFT:
								i.setCurrentPosition({ i.getCurrentPosition().x + i.getSpeed() * deltaTime, i.getCurrentPosition().y });
								break;
							default: ;
						}
						i.setPositionTemporary(i.getPositionTemporary() - i.getSpeed() * deltaTime);
					}
					else {
						i.setState(false);
						//Set back to normal
						i.setCurrentPosition(i.getCurrentPosition() - sf::Vector2f(i.getPositionTemporary() * static_cast<float>(i.getDirection() == PIRANHA_RIGHT || i.getDirection() == PIRANHA_LEFT), i.getPositionTemporary() * static_cast<float>(i.getDirection() == PIRANHA_UP || i.getDirection() == PIRANHA_DOWN)));
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
		if (i.isDestroyed()) continue;

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
		if (!PiranhaAIList[i].isDestroyed()) ++i;
		else PiranhaAIList.erase(PiranhaAIList.begin() + i);
	}
	PiranhaAIDeleteGate = false;
}