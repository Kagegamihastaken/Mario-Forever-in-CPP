#include <SFML/Graphics.hpp>
#include <array>
#include <vector>

#include "../headers/GoombaAIEffect.hpp"
#include "../headers/Loading.hpp"
#include "../headers/Mario.hpp"
#include "../headers/enum.hpp"
#include "../headers/Collide.hpp"
#include "../headers/WindowFrame.hpp"
#include "../headers/Brick.hpp"
#include "../headers/LuckyBlock.hpp"
#include "../headers/Scroll.hpp"

#include "../resource.h"

std::vector<MovableObject> GoombaAIEffectList;
std::array<sf::Texture, 1> GoombaAIEffectTexture;
std::vector<GoombaAIEffectID> GoombaAIEffectTypeList;
std::vector<float> GoombaAIEffectYveloList;
std::vector<std::pair<float, float>> GoombaAIEffectHitboxList;
std::vector<std::array<float, 4>> GoombaAIEffectDefinationList;
std::vector<sf::Clock> GoombaAIEffectClock;
std::vector<float> GoombaAIEffectAlphaList;
std::vector<sf::Clock> GoombaATEffectFadeOutList;

int GoombaAIEffectInit() {
	LoadTexture(GoombaAIEffectTexture[0], DEAD_GOOMBA_TEXTURE);

	return 6;
}
int iniGoombaAIEffect = GoombaAIEffectInit();
void AddGoombaAIEffect(GoombaAIType type, GoombaAIEffectID id, float x, float y) {
	MovableObject Init;
	GoombaAIEffectTypeList.push_back(id);
	if (type == GOOMBA) {
		Init.property.setOrigin(16, 31);
		Init.property.setTexture(GoombaAIEffectTexture[0]);
		if (id == COLLIDE) {
			Init.property.setTextureRect(sf::IntRect(0, 0, 31, 32));
			GoombaAIEffectHitboxList.push_back({ 31, 16 });
			GoombaAIEffectDefinationList.push_back({ 31.0f, 16.0f, 0.0f, 16.0f });
			GoombaAIEffectYveloList.push_back(0.0f);
		}
		else if (id == NONE) {
			Init.property.setTextureRect(sf::IntRect(31, 0, 31, 32));
			GoombaAIEffectHitboxList.push_back({ 31, 32 });
			GoombaAIEffectDefinationList.push_back({ 31.0f, 32.0f, 0.0f, 0.0f });
			GoombaAIEffectYveloList.push_back(-3.0f);
		}
	}
	setHitbox(Init.hitboxMain, { 0.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][2], 0.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][3], GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].first, GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].second });
	setHitbox(Init.hitboxTop, { 1.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][2], 0.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][3], GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].first - 2.0f, 2.0f });
	setHitbox(Init.hitboxBot, { 1.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][2], GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].second - 2.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][3], GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].first - 2.0f, 2.0f });
	setHitbox(Init.hitboxRight2, { GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].first - 2.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][2], 2.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][3], 3.0f, GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].second - 9.0f });
	setHitbox(Init.hitboxLeft2, { -1.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][2], 2.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][3], 3.0f, GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].second - 9.0f });
	setHitbox(Init.hitboxRight, { GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].first - 2.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][2], 2.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][3], 2.0f, GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].second - 9.0f });
	setHitbox(Init.hitboxLeft, { 0.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][2], 2.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][3], 2.0f, GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].second - 9.0f });
	Init.property.setPosition(static_cast<int>(round(x)), y);
	GoombaAIEffectList.push_back(Init);
	GoombaAIEffectClock.push_back(sf::Clock());
	GoombaAIEffectClock[GoombaAIEffectClock.size() - 1].restart().asMilliseconds();
	GoombaATEffectFadeOutList.push_back(sf::Clock());
	GoombaATEffectFadeOutList[GoombaATEffectFadeOutList.size() - 1].restart().asSeconds();
	GoombaAIEffectAlphaList.push_back(255);
}
void DeleteGoombaAIEffect(float x, float y) {
	for (int i = 0; i < GoombaAIEffectList.size(); ++i) {
		if (GoombaAIEffectList[i].property.getPosition().x == x && GoombaAIEffectList[i].property.getPosition().y == y) {
			GoombaAIEffectList.erase(GoombaAIEffectList.begin() + i);
			GoombaAIEffectDefinationList.erase(GoombaAIEffectDefinationList.begin() + i);
			GoombaAIEffectHitboxList.erase(GoombaAIEffectHitboxList.begin() + i);
			GoombaAIEffectTypeList.erase(GoombaAIEffectTypeList.begin() + i);
			GoombaAIEffectYveloList.erase(GoombaAIEffectYveloList.begin() + i);
			GoombaAIEffectClock.erase(GoombaAIEffectClock.begin() + i);
			GoombaATEffectFadeOutList.erase(GoombaATEffectFadeOutList.begin() + i);
			GoombaAIEffectAlphaList.erase(GoombaAIEffectAlphaList.begin() + i);
		}
	}
}
void DeleteAllGoombaAIEffect() {
	GoombaAIEffectList.clear();
	GoombaAIEffectDefinationList.clear();
	GoombaAIEffectHitboxList.clear();
	GoombaAIEffectTypeList.clear();
	GoombaAIEffectYveloList.clear();
	GoombaAIEffectClock.clear();
	GoombaATEffectFadeOutList.clear();
	GoombaAIEffectAlphaList.clear();
}
void GoombaAIEffectStatusUpdate() {
	for (int i = 0; i < GoombaAIEffectList.size(); ++i) {
		if (GoombaAIEffectTypeList[i] == NONE) {
			if (isOutScreen(GoombaAIEffectList[i].property.getPosition().x, GoombaAIEffectList[i].property.getPosition().y, 64, 64)) DeleteGoombaAIEffect(GoombaAIEffectList[i].property.getPosition().x, GoombaAIEffectList[i].property.getPosition().y);
		}
		else if (GoombaAIEffectTypeList[i] == COLLIDE) {
			if (GoombaATEffectFadeOutList[i].getElapsedTime().asSeconds() >= 4.0f) {
				if (GoombaAIEffectAlphaList[i] > 0) {
					GoombaAIEffectAlphaList[i] -= 7.5f * deltaTime;
					GoombaAIEffectList[i].property.setColor(sf::Color(255, 255, 255, std::max(0, static_cast<int>(GoombaAIEffectAlphaList[i]))));
				}
				else DeleteGoombaAIEffect(GoombaAIEffectList[i].property.getPosition().x, GoombaAIEffectList[i].property.getPosition().y);
			}
		}
	}
}
void GoombaAIEffectUpdate() {
	for (int i = 0; i < GoombaAIEffectList.size(); ++i) {
		window.draw(GoombaAIEffectList[i].property);
	}
}
void GoombaAIEffectVertYUpdate() {
	std::pair<bool, std::pair<bool, bool>> ObstacleCollide, BrickCollide, LuckyCollide;
	bool ObstacleCheck, BrickCheck, LuckyCheck, isLanding;
	float CurrPosYCollide;
	bool NoAdd;
	for (int i = 0; i < GoombaAIEffectList.size(); ++i) {
		if (GoombaAIEffectTypeList[i] == NONE) {
			if (GoombaAIEffectClock[i].getElapsedTime().asMilliseconds() >= 75.0f * deltaTime) {
				GoombaAIEffectYveloList[i] += 1.0f * deltaTime;
				GoombaAIEffectClock[i].restart().asMilliseconds();
			}
			GoombaAIEffectList[i].property.move(0.0f, GoombaAIEffectYveloList[i] * deltaTime);
			continue;
		}
		// bottom update
		ObstacleCheck = isCollideBot(GoombaAIEffectList[i], ObstaclesList);
		BrickCheck = isCollideBot(GoombaAIEffectList[i], Bricks);
		LuckyCheck = isCollideBot(GoombaAIEffectList[i], LuckyBlock);
		if (!ObstacleCheck && !BrickCheck && !LuckyCheck) {
			if (GoombaAIEffectClock[i].getElapsedTime().asMilliseconds() >= 12.5f * deltaTime) {
				GoombaAIEffectYveloList[i] += 1.0f * deltaTime;
				GoombaAIEffectList[i].property.move(0.0f, GoombaAIEffectYveloList[i] * deltaTime);
				GoombaAIEffectClock[i].restart().asMilliseconds();
			}
		}
		ObstacleCheck = isCollideBot(GoombaAIEffectList[i], ObstaclesList);
		BrickCheck = isCollideBot(GoombaAIEffectList[i], Bricks);
		LuckyCheck = isCollideBot(GoombaAIEffectList[i], LuckyBlock);
		//recolide
		if (ObstacleCheck || BrickCheck || LuckyCheck) {
			NoAdd = false;
			if (GoombaAIEffectYveloList[i] >= 0.0f) {
				GoombaAIEffectYveloList[i] = 0.0f;
				isLanding = true;
			}
			else {
				GoombaAIEffectYveloList[i] = 0.0f;
				isLanding = false;
			}
			ObstacleCollide = isAccurateCollideBot(GoombaAIEffectList[i], ObstaclesList, CurrPosYCollide, NoAdd);
			BrickCollide = isAccurateCollideBot(GoombaAIEffectList[i], Bricks, CurrPosYCollide, NoAdd);
			LuckyCollide = isAccurateCollideBot(GoombaAIEffectList[i], LuckyBlock, CurrPosYCollide, NoAdd);
			if ((ObstacleCollide.first || BrickCollide.first || LuckyCollide.first) && isLanding) {
				if (ObstacleCollide.second.first || ObstacleCollide.second.second || BrickCollide.second.first || BrickCollide.second.second || LuckyCollide.second.first || LuckyCollide.second.second) {
					GoombaAIEffectList[i].property.setPosition(GoombaAIEffectList[i].property.getPosition().x, CurrPosYCollide - (GoombaAIEffectDefinationList[i][1] - GoombaAIEffectList[i].property.getOrigin().y + GoombaAIEffectDefinationList[i][3]));
				}
				if (!ObstacleCollide.second.first && !ObstacleCollide.second.second && !BrickCollide.second.first && !BrickCollide.second.second && !LuckyCollide.second.first && !LuckyCollide.second.second) {
					GoombaAIEffectList[i].property.setPosition(GoombaAIEffectList[i].property.getPosition().x, CurrPosYCollide - (GoombaAIEffectDefinationList[i][1] - GoombaAIEffectList[i].property.getOrigin().y + GoombaAIEffectDefinationList[i][3]));
				}
			}
		}
		// top update
		ObstacleCheck = isCollideTop(GoombaAIEffectList[i], ObstaclesList, {});
		BrickCheck = isCollideTop(GoombaAIEffectList[i], Bricks, BrickSaveList);
		LuckyCheck = isCollideTop(GoombaAIEffectList[i], LuckyBlock, LuckyBlockSaveList);
		if ((ObstacleCheck || BrickCheck || LuckyCheck) && GoombaAIEffectYveloList[i] < 0.0f) {
			GoombaAIEffectYveloList[i] = 0.0f;
			ObstacleCollide = isAccurateCollideTop(GoombaAIEffectList[i], ObstaclesList, CurrPosYCollide, NoAdd, {});
			BrickCollide = isAccurateCollideTop(GoombaAIEffectList[i], Bricks, CurrPosYCollide, NoAdd, BrickSaveList);
			LuckyCollide = isAccurateCollideTop(GoombaAIEffectList[i], LuckyBlock, CurrPosYCollide, NoAdd, LuckyBlockSaveList);
			if ((ObstacleCollide.first || BrickCollide.first || LuckyCollide.first)) {
				if (ObstacleCollide.second.first || ObstacleCollide.second.second || BrickCollide.second.first || BrickCollide.second.second || LuckyCollide.second.first || LuckyCollide.second.second) {
					GoombaAIEffectList[i].property.setPosition(GoombaAIEffectList[i].property.getPosition().x, CurrPosYCollide + (31.0f + GoombaAIEffectList[i].property.getOrigin().y - GoombaAIEffectDefinationList[i][3]));
				}
				if (!ObstacleCollide.second.first && !ObstacleCollide.second.second && !BrickCollide.second.first && !BrickCollide.second.second && !LuckyCollide.second.first && !LuckyCollide.second.second) {
					GoombaAIEffectList[i].property.setPosition(GoombaAIEffectList[i].property.getPosition().x, CurrPosYCollide + (31.0f + GoombaAIEffectList[i].property.getOrigin().y - GoombaAIEffectDefinationList[i][3]));
				}
			}
		}
	}
}