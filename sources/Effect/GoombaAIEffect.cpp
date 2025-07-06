#include "Effect/GoombaAIEffect.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Scroll.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Class/CollisionObjectClass.hpp"
#include "Core/Class/GoombaAIEffectClass.hpp"

std::vector<MFCPP::GoombaAIEffect> GoombaAIEffectList;
static bool GoombaAIEffectDeleteGate = false;
void GoombaAIEffectInit() {
	ImageManager::AddTexture("DEAD_Goomba_1", "data/resources/Goomba/DEAD_Goomba.png", sf::IntRect({0, 0}, {31, 32}));
	ImageManager::AddTexture("DEAD_Goomba_2", "data/resources/Goomba/DEAD_Goomba.png", sf::IntRect({31, 0}, {31, 32}));
	ImageManager::AddTexture("DEAD_Koopa", "data/resources/Koopa/DEAD_GreenKoopa.png");
	ImageManager::AddTexture("DEAD_Spiny_Red", "data/resources/Spiny/DEAD_RedSpiny.png");
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
	if (type == GOOMBA) {
		if (id == COLLIDE) {
			GoombaAIEffectList.emplace_back(id, SkinID, 0.f, sf::FloatRect({0.f, 0.f}, {31.f, 32.f}), sf::Vector2f(x, y), sf::Vector2f(16, 31));
			GoombaAIEffectList.back().restartClock();
			GoombaAIEffectList.back().setTexture("DEAD_Goomba_1");
		}
		else if (id == NONE) {
			GoombaAIEffectList.emplace_back(id, SkinID, -3.f, sf::FloatRect({0.f, 0.f}, {31.f, 32.f}), sf::Vector2f(x, y), sf::Vector2f(16, 31));
			GoombaAIEffectList.back().restartClock();
			GoombaAIEffectList.back().setTexture("DEAD_Goomba_2");
		}
	}
	else if (type == KOOPA || type == SHELL || type == SHELL_MOVING) {
		if (id == NONE) {
			GoombaAIEffectList.emplace_back(id, SkinID, -3.f, sf::FloatRect({0.f, 0.f}, {33.f, 28.f}), sf::Vector2f(x, y), sf::Vector2f(16, 27));
			GoombaAIEffectList.back().restartClock();
			GoombaAIEffectList.back().setTexture("DEAD_Koopa");
		}
	}
	else if (type == SPINY) {
		if (id == NONE) {
			GoombaAIEffectList.emplace_back(id, SkinID, -3.f, sf::FloatRect({0.f, 0.f}, {33.f, 32.f}), sf::Vector2f(x, y), sf::Vector2f(15, 22));
			GoombaAIEffectList.back().restartClock();
			GoombaAIEffectList.back().setTexture("DEAD_Spiny_Red");
		}
	}
}

void DeleteGoombaAIEffectIndex(const int i) {
	GoombaAIEffectDeleteGate = true;
	GoombaAIEffectList[i].willDestroy(true);
	//GoombaAIEffectList.erase(GoombaAIEffectList.begin() + i);
}
void DeleteGoombaAIEffect(const float x, const float y) {
	for (int i = 0; i < GoombaAIEffectList.size(); ++i) {
		if (GoombaAIEffectList[i].getCurrentPosition().x == x && GoombaAIEffectList[i].getCurrentPosition().y == y) {
			DeleteGoombaAIEffectIndex(i);
			break;
		}
	}
}
void DeleteAllGoombaAIEffect() {
	GoombaAIEffectList.clear();
}
void GoombaAIEffectStatusUpdate(const float deltaTime) {
	if (GoombaAIEffectList.empty()) return;
	for (int i = 0; i < GoombaAIEffectList.size(); ++i) {
		if (GoombaAIEffectList[i].willBeDestroyed()) continue;

		if (GoombaAIEffectList[i].getID() == NONE) {
			if (isOutScreenYBottom(GoombaAIEffectList[i].getInterpolatedPosition().y, 64)) DeleteGoombaAIEffectIndex(i);
		}
		else if (GoombaAIEffectList[i].getID() == COLLIDE) {
			if (GoombaAIEffectList[i].getClock().getElapsedTime().asSeconds() >= 4.0f) {
				if (GoombaAIEffectList[i].getAlpha() > 0) {
					GoombaAIEffectList[i].setAlpha(GoombaAIEffectList[i].getAlpha() - 7.5f * deltaTime);
					GoombaAIEffectList[i].setColor(sf::Color(255, 255, 255, std::max(0, static_cast<int>(GoombaAIEffectList[i].getAlpha()))));
				}
				else DeleteGoombaAIEffectIndex(i);
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
	float CurrPosYCollide;
	for (auto & i : GoombaAIEffectList) {
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
		if (QuickCheckBotCollision(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.getHitbox()), CurrPosYCollide)) {
			if (i.getYVelo() >= 0.0f) {
				i.setYVelo(0.f);
				i.setCurrentPosition(sf::Vector2f(i.getCurrentPosition().x, CurrPosYCollide - (i.getHitbox().size.y - i.getOrigin().y)));
			}
			else {
				i.setYVelo(0.f);
			}
		}
		// top update
		if (QuickCheckTopCollision(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.getHitbox()), CurrPosYCollide)) {
			i.setYVelo(0.f);
			i.setCurrentPosition(sf::Vector2f(i.getCurrentPosition().x, CurrPosYCollide + (32.0f + i.getOrigin().y)));
		}
	}
}
void GoombaAIEffectCleanup() {
	if (!GoombaAIEffectDeleteGate) return;
	int i = 0;
	while (i < GoombaAIEffectList.size()) {
		if (!GoombaAIEffectList[i].willBeDestroyed()) ++i;
		else GoombaAIEffectList.erase(GoombaAIEffectList.begin() + i);
	}
	GoombaAIEffectDeleteGate = false;
}