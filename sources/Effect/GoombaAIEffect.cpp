#include "Effect/GoombaAIEffect.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Scroll.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Logging.hpp"
#include "Core/Class/CollisionObjectClass.hpp"
#include "Core/Class/GoombaAIEffectClass.hpp"
#include "Core/ExternalHeaders/plf_colony.h"

plf::colony<MFCPP::GoombaAIEffect> GoombaAIEffectList;
static bool GoombaAIEffectDeleteGate = false;
void GoombaAIEffectInit() {
	ImageManager::AddTexture("DEAD_Goomba_1", "data/resources/Goomba/DEAD_Goomba.png", sf::IntRect({0, 0}, {31, 32}));
	ImageManager::AddTexture("DEAD_Goomba_2", "data/resources/Goomba/DEAD_Goomba.png", sf::IntRect({31, 0}, {31, 32}));
	ImageManager::AddTexture("DEAD_GreenKoopa", "data/resources/Koopa/DEAD_GreenKoopa.png");
	ImageManager::AddTexture("DEAD_RedKoopa", "data/resources/Koopa/DEAD_RedKoopa.png");
	ImageManager::AddTexture("DEAD_Spiny_Red", "data/resources/Spiny/DEAD_RedSpiny.png");
	ImageManager::AddTexture("DEAD_Spiny_Green", "data/resources/Spiny/DEAD_GreenSpiny.png");
}
void SetPrevGoombaAIEffectPos() {
	for (auto & i : GoombaAIEffectList) {
		if (i.willBeDestroyed()) continue;

		i.setPreviousPosition(i.getCurrentPosition());
	}
}
void InterpolateGoombaAIEffectPos(const float alpha) {
	for (auto & i : GoombaAIEffectList) {
		if (i.willBeDestroyed()) continue;

		i.setInterpolatedPosition(linearInterpolation(i.getPreviousPosition(), i.getCurrentPosition(), alpha));
	}
}
void AddGoombaAIEffect(const GoombaAIType type, const GoombaAIEffectID id, const int SkinID, const float x, const float y) {
	plf::colony<MFCPP::GoombaAIEffect>::colony_iterator<false> it;
	if (type == GOOMBA) {
		if (id == COLLIDE) {
			it = GoombaAIEffectList.emplace(id, SkinID, 0.f, sf::FloatRect({0.f, 0.f}, {31.f, 32.f}), sf::Vector2f(x, y), sf::Vector2f(16, 31));
			it->setTexture("DEAD_Goomba_1");
		}
		else if (id == NONE) {
			it = GoombaAIEffectList.emplace(id, SkinID, -3.f, sf::FloatRect({0.f, 0.f}, {31.f, 32.f}), sf::Vector2f(x, y), sf::Vector2f(16, 31));
			it->setTexture("DEAD_Goomba_2");
		}
	}
	else if (type == KOOPA || type == SHELL || type == SHELL_MOVING) {
		if (id == NONE) {
			switch (SkinID) {
				case 0:
					it = GoombaAIEffectList.emplace(id, SkinID, -3.f, sf::FloatRect({0.f, 0.f}, {33.f, 28.f}), sf::Vector2f(x, y), sf::Vector2f(16, 27));
					it->setTexture("DEAD_GreenKoopa");
					break;
				case 1:
					it = GoombaAIEffectList.emplace(id, SkinID, -3.f, sf::FloatRect({0.f, 0.f}, {33.f, 28.f}), sf::Vector2f(x, y), sf::Vector2f(16, 27));
					it->setTexture("DEAD_RedKoopa");
					break;
				default: ;
			}
		}
	}
	else if (type == SPINY) {
		if (id == NONE) {
			switch (SkinID) {
				case 0:
					it = GoombaAIEffectList.emplace(id, SkinID, -3.f, sf::FloatRect({0.f, 0.f}, {33.f, 32.f}), sf::Vector2f(x, y), sf::Vector2f(15, 22));
					it->setTexture("DEAD_Spiny_Red");
					break;
				case 1:
					it = GoombaAIEffectList.emplace(id, SkinID, -3.f, sf::FloatRect({0.f, 0.f}, {33.f, 32.f}), sf::Vector2f(x, y), sf::Vector2f(15, 22));
					it->setTexture("DEAD_Spiny_Green");
					break;
				default: ;
			}
		}
	}
}

void DeleteGoombaAIEffectIndex(const plf::colony<MFCPP::GoombaAIEffect>::colony_iterator<false>& it) {
	GoombaAIEffectDeleteGate = true;
	it->willDestroy(true);
	//GoombaAIEffectList.erase(GoombaAIEffectList.begin() + i);
}
void DeleteGoombaAIEffect(const float x, const float y) {
	for (auto it = GoombaAIEffectList.begin(); it != GoombaAIEffectList.end(); ++it) {
		if (it->getCurrentPosition().x == x && it->getCurrentPosition().y == y) {
			DeleteGoombaAIEffectIndex(it);
			break;
		}
	}
}
void DeleteAllGoombaAIEffect() {
	GoombaAIEffectList.clear();
}
void GoombaAIEffectStatusUpdate(const float deltaTime) {
	if (GoombaAIEffectList.empty()) return;
	for (auto it = GoombaAIEffectList.begin(); it != GoombaAIEffectList.end(); ++it) {
		if (it->willBeDestroyed()) continue;

		if (it->getID() == NONE) {
			if (isOutScreenYBottom(it->getInterpolatedPosition().y, 64)) DeleteGoombaAIEffectIndex(it);
		}
		else if (it->getID() == COLLIDE) {
			if (it->getClock() >= 4.0f * 50.f) {
				if (it->getAlpha() > 0) {
					it->setAlpha(it->getAlpha() - 7.5f * deltaTime);
					it->setColor(sf::Color(255, 255, 255, std::max(0, static_cast<int>(it->getAlpha()))));
				}
				else DeleteGoombaAIEffectIndex(it);
			}
			else {
				it->setClock(it->getClock() + 1.f * deltaTime);
			}
		}
	}
}
void GoombaAIEffectDraw() {
	if (GoombaAIEffectList.empty()) return;
	for (auto & i : GoombaAIEffectList) {
		if (i.willBeDestroyed()) continue;

		if (!isOutScreen(i.getInterpolatedPosition().x, i.getInterpolatedPosition().y, 64, 64)) {
			i.AnimationUpdate(i.getInterpolatedPosition(), i.getOrigin());
			i.AnimationDraw(window);
		}
	}
}
void GoombaAIEffectVertYUpdate(const float deltaTime) {
	for (auto & i : GoombaAIEffectList) {
		float CurrPosYCollide, CurrPosXCollide;
		if (i.willBeDestroyed()) continue;

		if (i.getID() == GoombaAIEffectID::NONE) {
			i.move(sf::Vector2f(0.f, i.getYVelo() * deltaTime));
			i.setYVelo(i.getYVelo() + 1.f * deltaTime * 0.15f);
			continue;
		}
		// bottom update

		//ObstacleCheck = isCollideBot(GoombaAIList[i], ObstaclesList);
		//BrickCheck = isCollideBot(GoombaAIList[i], Bricks);
		//LuckyCheck = isCollideBot(GoombaAIList[i], LuckyBlock);
		//if (!ObstacleCheck && !BrickCheck && !LuckyCheck) {
		i.move(sf::Vector2f(0.f,i.getYVelo() * deltaTime));
		i.setYVelo(i.getYVelo() + (i.getYVelo() >= 10.0f ? 0.0f : 1.0f * deltaTime * 0.3f));
		//}
		if (QuickCheckBotCollision(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.getHitbox()), CurrPosXCollide, CurrPosYCollide)) {
			if (i.getYVelo() >= 0.0f) {
				const float floorY = GetCurrFloorY(i.getCurrentPosition(), CurrPosXCollide, CurrPosYCollide);
				if (i.getCurrentPosition().y < CurrPosYCollide + floorY - 3.f) continue;
				i.setYVelo(0.f);
				i.setCurrentPosition(sf::Vector2f(i.getCurrentPosition().x, CurrPosYCollide + floorY - (i.getHitbox().size.y - i.getOrigin().y)));
				continue;
			}
		}
		// top update
		if (QuickCheckTopCollision(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.getHitbox()), CurrPosXCollide, CurrPosYCollide)) {
			i.setYVelo(0.f);
			i.setCurrentPosition(sf::Vector2f(i.getCurrentPosition().x, CurrPosYCollide + (32.0f + i.getOrigin().y)));
		}
	}
}
void GoombaAIEffectCleanup() {
	if (!GoombaAIEffectDeleteGate) return;
	auto it = GoombaAIEffectList.begin();
	while (it != GoombaAIEffectList.end()) {
		if (!it->willBeDestroyed()) ++it;
		else it = GoombaAIEffectList.erase(it);
	}
	GoombaAIEffectDeleteGate = false;
}