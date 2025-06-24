#include <SFML/Graphics.hpp>
#include <array>
#include <vector>

#include "Effect/GoombaAIEffect.hpp"

#include <iostream>

#include "Object/Mario.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/WindowFrame.hpp"
#include "Block/Brick.hpp"
#include "Block/LuckyBlock.hpp"
#include "Core/Scroll.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Interpolation.hpp"
#include "Class/CollisionObjectClass.hpp"
#include "Class/GoombaAIEffectClass.hpp"

std::vector<MFCPP::GoombaAIEffect> GoombaAIEffectList;
//std::vector<GoombaAIEffectID> GoombaAIEffectTypeList;
//std::vector<float> GoombaAIEffectYveloList;
//std::vector<std::pair<float, float>> GoombaAIEffectHitboxList;
//std::vector<std::array<float, 4>> GoombaAIEffectDefinationList;
//std::vector<float> GoombaAIEffectAlphaList;
//std::vector<sf::Clock> GoombaATEffectFadeOutList;
//std::vector<int> GoombaAIEffectSkinIDList;

ImageManager GoombaAIEffectTextureManager;
void GoombaAIEffectInit() {
	ImageManager::AddImage("DEAD_GoombaImage", "data/resources/Goomba/DEAD_Goomba.png");
	ImageManager::AddTexture("DEAD_GoombaImage", sf::IntRect({0, 0}, {31, 32}), "DEAD_Goomba_1");
	ImageManager::AddTexture("DEAD_GoombaImage", sf::IntRect({31, 0}, {31, 32}), "DEAD_Goomba_2");
	ImageManager::AddImage("DEAD_KoopaImage", "data/resources/Koopa/DEAD_GreenKoopa.png");
	ImageManager::AddTexture("DEAD_KoopaImage", "DEAD_Koopa");
	ImageManager::AddImage("DEAD_SpinyImage", "data/resources/Spiny/DEAD_RedSpiny.png");
	ImageManager::AddTexture("DEAD_SpinyImage", "DEAD_Spiny_Red");
}
void SetPrevGoombaAIEffectPos() {
	for (auto & i : GoombaAIEffectList) {
		i.setPreviousPosition(i.getCurrentPosition());
	}
}
void InterpolateGoombaAIEffectPos(const float alpha) {
	for (auto & i : GoombaAIEffectList) {
		i.setInterpolatedPosition(linearInterpolation(i.getPreviousPosition(), i.getCurrentPosition(), alpha));
	}
}
void AddGoombaAIEffect(const GoombaAIType type, const GoombaAIEffectID id, const int SkinID, const float x, float y) {
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
	GoombaAIEffectList.erase(GoombaAIEffectList.begin() + i);
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
		if (GoombaAIEffectList[i].getID() == NONE) {
			if (isOutScreenYBottom(GoombaAIEffectList[i].getInterpolatedPosition().y, 64)) DeleteGoombaAIEffect(
				GoombaAIEffectList[i].getCurrentPosition().x, GoombaAIEffectList[i].getCurrentPosition().y);
		}
		else if (GoombaAIEffectList[i].getID() == COLLIDE) {
			if (GoombaAIEffectList[i].getClock().getElapsedTime().asSeconds() >= 4.0f) {
				if (GoombaAIEffectList[i].getAlpha() > 0) {
					GoombaAIEffectList[i].setAlpha(GoombaAIEffectList[i].getAlpha() - 7.5f * deltaTime);
					GoombaAIEffectList[i].setColor(sf::Color(255, 255, 255, std::max(0, static_cast<int>(GoombaAIEffectList[i].getAlpha()))));
				}
				else DeleteGoombaAIEffect(GoombaAIEffectList[i].getCurrentPosition().x, GoombaAIEffectList[i].getCurrentPosition().y);
			}
		}
	}
}
void GoombaAIEffectDraw() {
	if (GoombaAIEffectList.empty()) return;
	for (auto & i : GoombaAIEffectList) {
		if (!isOutScreen(i.getInterpolatedPosition().x, i.getInterpolatedPosition().y, 64, 64)) {
			i.AnimationUpdate(i.getInterpolatedPosition(), i.getOrigin());
			i.AnimationDraw(window);
		}
	}
}
void GoombaAIEffectVertYUpdate(const float deltaTime) {
	bool isLanding;
	float CurrPosYCollide;
	for (int i = 0; i < GoombaAIEffectList.size(); ++i) {
		if (GoombaAIEffectList[i].getID() == GoombaAIEffectID::NONE) {
			GoombaAIEffectList[i].setYVelo(GoombaAIEffectList[i].getYVelo() + 0.5f * deltaTime * 0.15f);
			GoombaAIEffectList[i].setCurrentPosition(sf::Vector2f(GoombaAIEffectList[i].getCurrentPosition().x,
			                                                      GoombaAIEffectList[i].getCurrentPosition().y +
			                                                      GoombaAIEffectList[i].getYVelo() * deltaTime));
			GoombaAIEffectList[i].setYVelo(GoombaAIEffectList[i].getYVelo() + 0.5f * deltaTime * 0.15f);
			continue;
		}
		// bottom update

		//ObstacleCheck = isCollideBot(GoombaAIList[i], ObstaclesList);
		//BrickCheck = isCollideBot(GoombaAIList[i], Bricks);
		//LuckyCheck = isCollideBot(GoombaAIList[i], LuckyBlock);
		//if (!ObstacleCheck && !BrickCheck && !LuckyCheck) {
		GoombaAIEffectList[i].setYVelo(GoombaAIEffectList[i].getYVelo() + (GoombaAIEffectList[i].getYVelo() >= 10.0f ? 0.0f : 0.5f * deltaTime * 0.3f));
		GoombaAIEffectList[i].setCurrentPosition(sf::Vector2f(GoombaAIEffectList[i].getCurrentPosition().x,GoombaAIEffectList[i].getCurrentPosition().y + GoombaAIEffectList[i].getYVelo() * deltaTime));
		GoombaAIEffectList[i].setYVelo(GoombaAIEffectList[i].getYVelo() + (GoombaAIEffectList[i].getYVelo() >= 10.0f ? 0.0f : 0.5f * deltaTime * 0.3f));
		//}

		bool NoAdd = false;
		int be = find_min_iny(GoombaAIEffectList[i].getCurrentPosition(), ObstaclesVertPosList);
		int nd = find_max_iny_dist(GoombaAIEffectList[i].getCurrentPosition(), ObstaclesVertPosList,
		                           64.0f + (GoombaAIEffectList[i].getYVelo()) * 16.0f);
		bool ObstacleCollide = isAccurateCollideBot(MFCPP::CollisionObject(GoombaAIEffectList[i].getCurrentPosition(), GoombaAIEffectList[i].getOrigin(), GoombaAIEffectList[i].getHitbox()),
		                                             ObstaclesVertPosList, CurrPosYCollide, NoAdd, be, nd, 80.0f);
		be = find_min_iny(GoombaAIEffectList[i].getCurrentPosition(), BricksVertPosList);
		nd = find_max_iny_dist(GoombaAIEffectList[i].getCurrentPosition(), BricksVertPosList, 64.0f + (GoombaAIEffectList[i].getYVelo()) * 16.0f);
		bool BrickCollide = isAccurateCollideBot(MFCPP::CollisionObject(GoombaAIEffectList[i].getCurrentPosition(), GoombaAIEffectList[i].getOrigin(), GoombaAIEffectList[i].getHitbox()), BricksVertPosList,
		                                          CurrPosYCollide, NoAdd, be, nd, 80.0f);
		be = find_min_iny(GoombaAIEffectList[i].getCurrentPosition(), LuckyVertPosList);
		nd = find_max_iny_dist(GoombaAIEffectList[i].getCurrentPosition(), LuckyVertPosList, 64.0f + (GoombaAIEffectList[i].getYVelo()) * 16.0f);
		bool LuckyCollide = isAccurateCollideBot(MFCPP::CollisionObject(GoombaAIEffectList[i].getCurrentPosition(), GoombaAIEffectList[i].getOrigin(), GoombaAIEffectList[i].getHitbox()), LuckyVertPosList,
		                                          CurrPosYCollide, NoAdd, be, nd, 80.0f);
		if (ObstacleCollide || BrickCollide || LuckyCollide) {
			if (GoombaAIEffectList[i].getYVelo() >= 0.0f) {
				GoombaAIEffectList[i].setYVelo(0.f);
				isLanding = true;
			}
			else {
				GoombaAIEffectList[i].setYVelo(0.f);
				isLanding = false;
			}
			//recolide
			if (isLanding) {
				GoombaAIEffectList[i].setCurrentPosition(sf::Vector2f(GoombaAIEffectList[i].getCurrentPosition().x, CurrPosYCollide - (GoombaAIEffectList[i].getHitbox().size.y - GoombaAIEffectList[i].getOrigin().y)));
			}
		}
		// top update
		NoAdd = false;
		be = find_max_iny(GoombaAIEffectList[i].getCurrentPosition(), ObstaclesVertPosList);
		nd = find_min_iny_dist(GoombaAIEffectList[i].getCurrentPosition(), ObstaclesVertPosList, 64.0f - (GoombaAIEffectList[i].getYVelo()) * 16.0f);
		ObstacleCollide = isAccurateCollideTop(MFCPP::CollisionObject(GoombaAIEffectList[i].getCurrentPosition(), GoombaAIEffectList[i].getOrigin(), GoombaAIEffectList[i].getHitbox()), ObstaclesVertPosList, CurrPosYCollide, NoAdd, nd, be, 80.0f);
		be = find_max_iny(GoombaAIEffectList[i].getCurrentPosition(), BricksVertPosList);
		nd = find_min_iny_dist(GoombaAIEffectList[i].getCurrentPosition(), BricksVertPosList, 64.0f - (GoombaAIEffectList[i].getYVelo()) * 16.0f);
		BrickCollide = isAccurateCollideTop(MFCPP::CollisionObject(GoombaAIEffectList[i].getCurrentPosition(), GoombaAIEffectList[i].getOrigin(), GoombaAIEffectList[i].getHitbox()), BricksVertPosList, CurrPosYCollide, NoAdd, nd, be, 80.0f);
		be = find_max_iny(GoombaAIEffectList[i].getCurrentPosition(), LuckyVertPosList);
		nd = find_min_iny_dist(GoombaAIEffectList[i].getCurrentPosition(), LuckyVertPosList, 64.0f - (GoombaAIEffectList[i].getYVelo()) * 16.0f);
		LuckyCollide = isAccurateCollideTop(MFCPP::CollisionObject(GoombaAIEffectList[i].getCurrentPosition(), GoombaAIEffectList[i].getOrigin(), GoombaAIEffectList[i].getHitbox()), LuckyVertPosList, CurrPosYCollide, NoAdd, nd, be, 80.0f);
		if (ObstacleCollide || BrickCollide || LuckyCollide) {
			GoombaAIEffectList[i].setYVelo(0.f);
			GoombaAIEffectList[i].setCurrentPosition(sf::Vector2f(GoombaAIEffectList[i].getCurrentPosition().x, CurrPosYCollide + (32.0f + GoombaAIEffectList[i].getOrigin().y)));
		}
	}
}