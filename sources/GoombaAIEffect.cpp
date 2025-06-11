#include <SFML/Graphics.hpp>
#include <array>
#include <vector>

#include "Effect/GoombaAIEffect.hpp"
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

std::vector<MovableObject> GoombaAIEffectList;
std::vector<GoombaAIEffectID> GoombaAIEffectTypeList;
std::vector<float> GoombaAIEffectYveloList;
std::vector<std::pair<float, float>> GoombaAIEffectHitboxList;
std::vector<std::array<float, 4>> GoombaAIEffectDefinationList;
std::vector<float> GoombaAIEffectAlphaList;
std::vector<sf::Clock> GoombaATEffectFadeOutList;
std::vector<int> GoombaAIEffectSkinIDList;

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
		i.prev = i.curr;
	}
}
void InterpolateGoombaAIEffectPos(const float alpha) {
	for (auto & i : GoombaAIEffectList) {
		i.property.setPosition(linearInterpolation(i.prev, i.curr, alpha));
	}
}
void AddGoombaAIEffect(const GoombaAIType type, const GoombaAIEffectID id, const int SkinID, const float x, float y) {
	MovableObject Init;
	GoombaAIEffectTypeList.push_back(id);
	if (type == GOOMBA) {
		Init.property.setOrigin({ 16, 31 });
		if (id == COLLIDE) {
			Init.property.setTexture(ImageManager::GetTexture("DEAD_Goomba_1"), true);
			GoombaAIEffectHitboxList.emplace_back( 31.0f, 16.0f );
			GoombaAIEffectDefinationList.push_back({ 31.0f, 16.0f, 0.0f, 16.0f });
			GoombaAIEffectYveloList.push_back(0.0f);
		}
		else if (id == NONE) {
			Init.property.setTexture(ImageManager::GetTexture("DEAD_Goomba_2"), true);
			GoombaAIEffectHitboxList.emplace_back( 31.0f, 32.0f );
			GoombaAIEffectDefinationList.push_back({ 31.0f, 32.0f, 0.0f, 0.0f });
			GoombaAIEffectYveloList.push_back(-3.0f);
		}
	}
	else if (type == KOOPA || type == SHELL || type == SHELL_MOVING) {
		Init.property.setOrigin({ 16, 27 });
		if (id == NONE) {
			Init.property.setTexture(ImageManager::GetTexture("DEAD_Koopa"), true);
			GoombaAIEffectHitboxList.emplace_back( 33.0f, 28.0f );
			GoombaAIEffectDefinationList.push_back({ 33.0f, 28.0f, 0.0f, 0.0f });
			GoombaAIEffectYveloList.push_back(-3.0f);
		}
	}
	else if (type == SPINY) {
		Init.property.setOrigin({ 15, 22 });
		if (id == NONE) {
			Init.property.setTexture(ImageManager::GetTexture("DEAD_Spiny_Red"), true);
			GoombaAIEffectHitboxList.emplace_back( 33.0f, 32.0f );
			GoombaAIEffectDefinationList.push_back({ 33.0f, 32.0f, 0.0f, 0.0f });
			GoombaAIEffectYveloList.push_back(-3.0f);
		}
	}
	setHitbox(Init.hitboxMain, sf::FloatRect({ 0.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][2], 0.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][3] }, { GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].first, GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].second }));
	setHitbox(Init.hitboxTop, sf::FloatRect({ 1.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][2], 0.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][3] }, { GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].first - 2.0f, 2.0f }));
	setHitbox(Init.hitboxBot, sf::FloatRect({ 1.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][2], GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].second - 2.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][3] }, { GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].first - 2.0f, 2.0f }));
	setHitbox(Init.hitboxRight, sf::FloatRect({ GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].first - 2.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][2], 2.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][3] }, { 2.0f, GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].second - 9.0f }));
	setHitbox(Init.hitboxLeft, sf::FloatRect({ 0.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][2], 2.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][3] }, { 2.0f, GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].second - 9.0f }));
	Init.property.setPosition({ round(x), y });
	Init.curr = Init.prev = Init.property.getPosition();
	GoombaAIEffectList.push_back(Init);
	GoombaATEffectFadeOutList.emplace_back();
	GoombaATEffectFadeOutList[GoombaATEffectFadeOutList.size() - 1].restart();
	GoombaAIEffectAlphaList.push_back(255);
	GoombaAIEffectSkinIDList.push_back(SkinID);
}

void DeleteGoombaAIEffectIndex(const int i) {
	GoombaAIEffectList.erase(GoombaAIEffectList.begin() + i);
	GoombaAIEffectDefinationList.erase(GoombaAIEffectDefinationList.begin() + i);
	GoombaAIEffectHitboxList.erase(GoombaAIEffectHitboxList.begin() + i);
	GoombaAIEffectTypeList.erase(GoombaAIEffectTypeList.begin() + i);
	GoombaAIEffectYveloList.erase(GoombaAIEffectYveloList.begin() + i);
	GoombaATEffectFadeOutList.erase(GoombaATEffectFadeOutList.begin() + i);
	GoombaAIEffectAlphaList.erase(GoombaAIEffectAlphaList.begin() + i);
	GoombaAIEffectSkinIDList.erase(GoombaAIEffectSkinIDList.begin() + i);
}
void DeleteGoombaAIEffect(const float x, const float y) {
	for (int i = 0; i < GoombaAIEffectList.size(); ++i) {
		if (GoombaAIEffectList[i].curr.x == x && GoombaAIEffectList[i].curr.y == y) {
			DeleteGoombaAIEffectIndex(i);
			break;
		}
	}
}
void DeleteAllGoombaAIEffect() {
	GoombaAIEffectList.clear();
	GoombaAIEffectDefinationList.clear();
	GoombaAIEffectHitboxList.clear();
	GoombaAIEffectTypeList.clear();
	GoombaAIEffectYveloList.clear();
	GoombaATEffectFadeOutList.clear();
	GoombaAIEffectAlphaList.clear();
	GoombaAIEffectSkinIDList.clear();
}
void GoombaAIEffectStatusUpdate(const float deltaTime) {
	if (GoombaAIEffectList.empty()) return;
	for (int i = 0; i < GoombaAIEffectList.size(); ++i) {
		if (GoombaAIEffectTypeList[i] == NONE) {
			if (isOutScreenYBottom(GoombaAIEffectList[i].property.getPosition().y, 64)) DeleteGoombaAIEffect(GoombaAIEffectList[i].curr.x, GoombaAIEffectList[i].curr.y);
		}
		else if (GoombaAIEffectTypeList[i] == COLLIDE) {
			if (GoombaATEffectFadeOutList[i].getElapsedTime().asSeconds() >= 4.0f) {
				if (GoombaAIEffectAlphaList[i] > 0) {
					GoombaAIEffectAlphaList[i] -= 7.5f * deltaTime;
					GoombaAIEffectList[i].property.setColor(sf::Color(255, 255, 255, std::max(0, static_cast<int>(GoombaAIEffectAlphaList[i]))));
				}
				else DeleteGoombaAIEffect(GoombaAIEffectList[i].curr.x, GoombaAIEffectList[i].curr.y);
			}
		}
	}
}
void GoombaAIEffectUpdate() {
	if (GoombaAIEffectList.empty()) return;
	for (auto & i : GoombaAIEffectList) {
		if (!isOutScreen(i.property.getPosition().x, i.property.getPosition().y, 64, 64)) window.draw(i.property);
	}
}
void GoombaAIEffectVertYUpdate(const float deltaTime) {
	bool isLanding;
	float CurrPosYCollide;
	for (int i = 0; i < GoombaAIEffectList.size(); ++i) {
		if (GoombaAIEffectTypeList[i] == NONE) {
			GoombaAIEffectYveloList[i] += 0.5f * deltaTime * 0.15f;
			GoombaAIEffectList[i].curr = { GoombaAIEffectList[i].curr.x,GoombaAIEffectList[i].curr.y + GoombaAIEffectYveloList[i] * deltaTime };
			GoombaAIEffectYveloList[i] += 0.5f * deltaTime * 0.15f;
			continue;
		}
		// bottom update

		//ObstacleCheck = isCollideBot(GoombaAIList[i], ObstaclesList);
		//BrickCheck = isCollideBot(GoombaAIList[i], Bricks);
		//LuckyCheck = isCollideBot(GoombaAIList[i], LuckyBlock);
		//if (!ObstacleCheck && !BrickCheck && !LuckyCheck) {
		GoombaAIEffectYveloList[i] += (GoombaAIEffectYveloList[i] >= 10.0f ? 0.0f : 0.5f * deltaTime * 0.3f);
		GoombaAIEffectList[i].curr = { GoombaAIEffectList[i].curr.x, GoombaAIEffectList[i].curr.y + GoombaAIEffectYveloList[i] * deltaTime };
		GoombaAIEffectYveloList[i] += (GoombaAIEffectYveloList[i] >= 10.0f ? 0.0f : 0.5f * deltaTime * 0.3f);
		//}

		bool NoAdd = false;
		int be = find_min_iny(GoombaAIEffectList[i].curr, ObstaclesVertPosList);
		int nd = find_max_iny_dist(GoombaAIEffectList[i].curr, ObstaclesVertPosList,
		                           64.0f + (GoombaAIEffectYveloList[i]) * 16.0f);
		bool ObstacleCollide = isAccurateCollideBott(MFCPP::CollisionObject(GoombaAIEffectList[i].curr, GoombaAIEffectList[i].property.getOrigin(), GoombaAIEffectList[i].hitboxBot),
		                                             ObstaclesVertPosList, CurrPosYCollide, NoAdd, be, nd, 80.0f);
		be = find_min_iny(GoombaAIEffectList[i].curr, BricksVertPosList);
		nd = find_max_iny_dist(GoombaAIEffectList[i].curr, BricksVertPosList, 64.0f + (GoombaAIEffectYveloList[i]) * 16.0f);
		bool BrickCollide = isAccurateCollideBott(MFCPP::CollisionObject(GoombaAIEffectList[i].curr, GoombaAIEffectList[i].property.getOrigin(), GoombaAIEffectList[i].hitboxBot), BricksVertPosList,
		                                          CurrPosYCollide, NoAdd, be, nd, 80.0f);
		be = find_min_iny(GoombaAIEffectList[i].curr, LuckyVertPosList);
		nd = find_max_iny_dist(GoombaAIEffectList[i].curr, LuckyVertPosList, 64.0f + (GoombaAIEffectYveloList[i]) * 16.0f);
		bool LuckyCollide = isAccurateCollideBott(MFCPP::CollisionObject(GoombaAIEffectList[i].curr, GoombaAIEffectList[i].property.getOrigin(), GoombaAIEffectList[i].hitboxBot), LuckyVertPosList,
		                                          CurrPosYCollide, NoAdd, be, nd, 80.0f);
		if (ObstacleCollide || BrickCollide || LuckyCollide) {
			if (GoombaAIEffectYveloList[i] >= 0.0f) {
				GoombaAIEffectYveloList[i] = 0.0f;
				isLanding = true;
			}
			else {
				GoombaAIEffectYveloList[i] = 0.0f;
				isLanding = false;
			}
			//recolide
			if (isLanding) {
				GoombaAIEffectList[i].curr = { GoombaAIEffectList[i].curr.x, CurrPosYCollide - (GoombaAIEffectDefinationList[i][1] - GoombaAIEffectList[i].property.getOrigin().y + GoombaAIEffectDefinationList[i][3]) };
			}
		}
		// top update
		NoAdd = false;
		be = find_max_iny(GoombaAIEffectList[i].curr, ObstaclesVertPosList);
		nd = find_min_iny_dist(GoombaAIEffectList[i].curr, ObstaclesVertPosList, 64.0f - (GoombaAIEffectYveloList[i]) * 16.0f);
		ObstacleCollide = isAccurateCollideTopt(MFCPP::CollisionObject(GoombaAIEffectList[i].curr, GoombaAIEffectList[i].property.getOrigin(), GoombaAIEffectList[i].hitboxTop), ObstaclesVertPosList, CurrPosYCollide, NoAdd, nd, be, 80.0f);
		be = find_max_iny(GoombaAIEffectList[i].curr, BricksVertPosList);
		nd = find_min_iny_dist(GoombaAIEffectList[i].curr, BricksVertPosList, 64.0f - (GoombaAIEffectYveloList[i]) * 16.0f);
		BrickCollide = isAccurateCollideTopt(MFCPP::CollisionObject(GoombaAIEffectList[i].curr, GoombaAIEffectList[i].property.getOrigin(), GoombaAIEffectList[i].hitboxTop), BricksVertPosList, CurrPosYCollide, NoAdd, nd, be, 80.0f);
		be = find_max_iny(GoombaAIEffectList[i].curr, LuckyVertPosList);
		nd = find_min_iny_dist(GoombaAIEffectList[i].curr, LuckyVertPosList, 64.0f - (GoombaAIEffectYveloList[i]) * 16.0f);
		LuckyCollide = isAccurateCollideTopt(MFCPP::CollisionObject(GoombaAIEffectList[i].curr, GoombaAIEffectList[i].property.getOrigin(), GoombaAIEffectList[i].hitboxTop), LuckyVertPosList, CurrPosYCollide, NoAdd, nd, be, 80.0f);
		if ((ObstacleCollide || BrickCollide || LuckyCollide) && GoombaAIEffectYveloList[i] < 0.0f) {
			GoombaAIEffectYveloList[i] = 0.0f;
			GoombaAIEffectList[i].curr = { GoombaAIEffectList[i].curr.x, CurrPosYCollide + (31.0f + GoombaAIEffectList[i].property.getOrigin().y - GoombaAIEffectDefinationList[i][3]) };
		}
	}
}