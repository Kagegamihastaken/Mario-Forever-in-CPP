#include <SFML/Graphics.hpp>
#include <array>
#include <vector>

#include "../headers/Effect/GoombaAIEffect.hpp"
#include "../headers/Core/Loading/Loading.hpp"
#include "../headers/Object/Mario.hpp"
#include "../headers/Core/Loading/enum.hpp"
#include "../headers/Core/Collision/Collide.hpp"
#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Block/Brick.hpp"
#include "../headers/Block/LuckyBlock.hpp"
#include "../headers/Core/Scroll.hpp"
#include "../headers/Core/TextureManager.hpp"

#include "../resource.h"

std::vector<MovableObject> GoombaAIEffectList;
std::vector<GoombaAIEffectID> GoombaAIEffectTypeList;
std::vector<float> GoombaAIEffectYveloList;
std::vector<std::pair<float, float>> GoombaAIEffectHitboxList;
std::vector<std::array<float, 4>> GoombaAIEffectDefinationList;
std::vector<float> GoombaAIEffectAlphaList;
std::vector<sf::Clock> GoombaATEffectFadeOutList;
std::vector<int> GoombaAIEffectSkinIDList;

TextureManager GoombaAIEffectTextureManager;
void GoombaAIEffectInit() {
	GoombaAIEffectTextureManager.Loadingtexture(DEAD_GOOMBA_TEXTURE, "DEAD_Goomba_1", 0, 0, 31, 32);
	GoombaAIEffectTextureManager.Loadingtexture(DEAD_GOOMBA_TEXTURE, "DEAD_Goomba_2", 31, 0, 31, 32);
	GoombaAIEffectTextureManager.Loadingtexture(DEAD_GREEN_KOOPA_TEXTURE, "DEAD_Koopa", 0, 0, 33, 28);
	GoombaAIEffectTextureManager.Loadingtexture(DEAD_RED_SPINY_TEXTURE, "DEAD_Spiny_Red", 0, 0, 33, 32);
}
void AddGoombaAIEffect(GoombaAIType type, GoombaAIEffectID id, int SkinID, float x, float y) {
	MovableObject Init;
	GoombaAIEffectTypeList.push_back(id);
	if (type == GOOMBA) {
		Init.property.setOrigin({ 16, 31 });
		if (id == COLLIDE) {
			Init.property.setTexture(*GoombaAIEffectTextureManager.GetTexture("DEAD_Goomba_1"), true);
			GoombaAIEffectHitboxList.push_back({ 31, 16 });
			GoombaAIEffectDefinationList.push_back({ 31.0f, 16.0f, 0.0f, 16.0f });
			GoombaAIEffectYveloList.push_back(0.0f);
		}
		else if (id == NONE) {
			Init.property.setTexture(*GoombaAIEffectTextureManager.GetTexture("DEAD_Goomba_2"), true);
			GoombaAIEffectHitboxList.push_back({ 31, 32 });
			GoombaAIEffectDefinationList.push_back({ 31.0f, 32.0f, 0.0f, 0.0f });
			GoombaAIEffectYveloList.push_back(-3.0f);
		}
	}
	else if (type == KOOPA || type == SHELL || type == SHELL_MOVING) {
		Init.property.setOrigin({ 16, 19 });
		if (id == NONE) {
			Init.property.setTexture(*GoombaAIEffectTextureManager.GetTexture("DEAD_Koopa"), true);
			GoombaAIEffectHitboxList.push_back({ 33, 28 });
			GoombaAIEffectDefinationList.push_back({ 33.0f, 28.0f, 0.0f, 0.0f });
			GoombaAIEffectYveloList.push_back(-3.0f);
		}
	}
	else if (type == SPINY) {
		Init.property.setOrigin({ 15, 22 });
		if (id == NONE) {
			Init.property.setTexture(*GoombaAIEffectTextureManager.GetTexture("DEAD_Spiny_Red"), true);
			GoombaAIEffectHitboxList.push_back({ 33, 32 });
			GoombaAIEffectDefinationList.push_back({ 33.0f, 32.0f, 0.0f, 0.0f });
			GoombaAIEffectYveloList.push_back(-3.0f);
		}
	}
	setHitbox(Init.hitboxMain, sf::FloatRect({ 0.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][2], 0.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][3] }, { GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].first, GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].second }));
	setHitbox(Init.hitboxTop, sf::FloatRect({ 1.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][2], 0.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][3] }, { GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].first - 2.0f, 2.0f }));
	setHitbox(Init.hitboxBot, sf::FloatRect({ 1.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][2], GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].second - 2.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][3] }, { GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].first - 2.0f, 2.0f }));
	//setHitbox(Init.hitboxRight2, sf::FloatRect({ GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].first - 2.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][2], 2.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][3] }, { 3.0f, GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].second - 9.0f }));
	//setHitbox(Init.hitboxLeft2, sf::FloatRect({ -1.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][2], 2.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][3] }, { 3.0f, GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].second - 9.0f }));
	setHitbox(Init.hitboxRight, sf::FloatRect({ GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].first - 2.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][2], 2.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][3] }, { 2.0f, GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].second - 9.0f }));
	setHitbox(Init.hitboxLeft, sf::FloatRect({ 0.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][2], 2.0f + GoombaAIEffectDefinationList[GoombaAIEffectDefinationList.size() - 1][3] }, { 2.0f, GoombaAIEffectHitboxList[GoombaAIEffectHitboxList.size() - 1].second - 9.0f }));
	Init.property.setPosition({ round(x), y });
	GoombaAIEffectList.push_back(Init);
	GoombaATEffectFadeOutList.push_back(sf::Clock());
	GoombaATEffectFadeOutList[GoombaATEffectFadeOutList.size() - 1].restart();
	GoombaAIEffectAlphaList.push_back(255);
	GoombaAIEffectSkinIDList.push_back(SkinID);
}
void DeleteGoombaAIEffect(float x, float y) {
	for (int i = 0; i < GoombaAIEffectList.size(); ++i) {
		if (GoombaAIEffectList[i].property.getPosition().x == x && GoombaAIEffectList[i].property.getPosition().y == y) {
			GoombaAIEffectList.erase(GoombaAIEffectList.begin() + i);
			GoombaAIEffectDefinationList.erase(GoombaAIEffectDefinationList.begin() + i);
			GoombaAIEffectHitboxList.erase(GoombaAIEffectHitboxList.begin() + i);
			GoombaAIEffectTypeList.erase(GoombaAIEffectTypeList.begin() + i);
			GoombaAIEffectYveloList.erase(GoombaAIEffectYveloList.begin() + i);
			GoombaATEffectFadeOutList.erase(GoombaATEffectFadeOutList.begin() + i);
			GoombaAIEffectAlphaList.erase(GoombaAIEffectAlphaList.begin() + i);
			GoombaAIEffectSkinIDList.erase(GoombaAIEffectSkinIDList.begin() + i);
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
void GoombaAIEffectStatusUpdate(float deltaTime) {
	if (GoombaAIEffectList.size() == 0) return;
	for (int i = 0; i < GoombaAIEffectList.size(); ++i) {
		if (GoombaAIEffectTypeList[i] == NONE) {
			if (isOutScreenYBottom(GoombaAIEffectList[i].property.getPosition().y, 64)) DeleteGoombaAIEffect(GoombaAIEffectList[i].property.getPosition().x, GoombaAIEffectList[i].property.getPosition().y);
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
	if (GoombaAIEffectList.size() == 0) return;
	for (int i = 0; i < GoombaAIEffectList.size(); ++i) {
		if (!isOutScreen(GoombaAIEffectList[i].property.getPosition().x, GoombaAIEffectList[i].property.getPosition().y, 64, 64)) rTexture.draw(GoombaAIEffectList[i].property);
	}
}
void GoombaAIEffectVertYUpdate(float deltaTime) {
	bool ObstacleCollide, BrickCollide, LuckyCollide;
	bool isLanding;
	float CurrPosYCollide;
	bool NoAdd;
	for (int i = 0; i < GoombaAIEffectList.size(); ++i) {
		if (GoombaAIEffectTypeList[i] == NONE) {
			GoombaAIEffectYveloList[i] += 0.5f * deltaTime * 0.15f;
			GoombaAIEffectList[i].property.move({ 0.0f, GoombaAIEffectYveloList[i] * deltaTime });
			GoombaAIEffectYveloList[i] += 0.5f * deltaTime * 0.15f;
			continue;
		}
		// bottom update

		//ObstacleCheck = isCollideBot(GoombaAIList[i], ObstaclesList);
		//BrickCheck = isCollideBot(GoombaAIList[i], Bricks);
		//LuckyCheck = isCollideBot(GoombaAIList[i], LuckyBlock);
		//if (!ObstacleCheck && !BrickCheck && !LuckyCheck) {
		GoombaAIEffectYveloList[i] += (GoombaAIEffectYveloList[i] >= 10.0f ? 0.0f : 0.5f * deltaTime * 0.3f);
		GoombaAIEffectList[i].property.move({ 0.0f, GoombaAIEffectYveloList[i] * deltaTime });
		GoombaAIEffectYveloList[i] += (GoombaAIEffectYveloList[i] >= 10.0f ? 0.0f : 0.5f * deltaTime * 0.3f);
		//}

		NoAdd = false;
		int be, nd;
		be = find_min_iny(GoombaAIEffectList[i], ObstaclesVertPosList);
		nd = find_max_iny_dist(GoombaAIEffectList[i], ObstaclesVertPosList, 64.0f + (GoombaAIEffectYveloList[i]) * 16.0f);
		ObstacleCollide = isAccurateCollideBott(GoombaAIEffectList[i], ObstaclesVertPosList, CurrPosYCollide, NoAdd, be, nd, 80.0f);
		be = find_min_iny(GoombaAIEffectList[i], BricksVertPosList);
		nd = find_max_iny_dist(GoombaAIEffectList[i], BricksVertPosList, 64.0f + (GoombaAIEffectYveloList[i]) * 16.0f);
		BrickCollide = isAccurateCollideBott(GoombaAIEffectList[i], BricksVertPosList, CurrPosYCollide, NoAdd, be, nd, 80.0f);
		be = find_min_iny(GoombaAIEffectList[i], LuckyVertPosList);
		nd = find_max_iny_dist(GoombaAIEffectList[i], LuckyVertPosList, 64.0f + (GoombaAIEffectYveloList[i]) * 16.0f);
		LuckyCollide = isAccurateCollideBott(GoombaAIEffectList[i], LuckyVertPosList, CurrPosYCollide, NoAdd, be, nd, 80.0f);
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
				GoombaAIEffectList[i].property.setPosition({ GoombaAIEffectList[i].property.getPosition().x, CurrPosYCollide - (GoombaAIEffectDefinationList[i][1] - GoombaAIEffectList[i].property.getOrigin().y + GoombaAIEffectDefinationList[i][3]) });
				//if (ObstacleCollide.second || BrickCollide.second || LuckyCollide.second) {
				//	GoombaAIEffectList[i].property.setPosition({ GoombaAIEffectList[i].property.getPosition().x, CurrPosYCollide - (GoombaAIEffectDefinationList[i][1] - GoombaAIEffectList[i].property.getOrigin().y + GoombaAIEffectDefinationList[i][3]) });
				//}
				//if (!ObstacleCollide.second && !BrickCollide.second && !LuckyCollide.second) {
					//GoombaAIEffectList[i].property.setPosition({ GoombaAIEffectList[i].property.getPosition().x, CurrPosYCollide - (GoombaAIEffectDefinationList[i][1] - GoombaAIEffectList[i].property.getOrigin().y + GoombaAIEffectDefinationList[i][3]) });
				//}
			}
		}
		// top update
		NoAdd = false;
		be = find_max_iny(GoombaAIEffectList[i], ObstaclesVertPosList);
		nd = find_min_iny_dist(GoombaAIEffectList[i], ObstaclesVertPosList, 64.0f - (GoombaAIEffectYveloList[i]) * 16.0f);
		ObstacleCollide = isAccurateCollideTopt(GoombaAIEffectList[i], ObstaclesVertPosList, CurrPosYCollide, NoAdd, nd, be, 80.0f);
		be = find_max_iny(GoombaAIEffectList[i], BricksVertPosList);
		nd = find_min_iny_dist(GoombaAIEffectList[i], BricksVertPosList, 64.0f - (GoombaAIEffectYveloList[i]) * 16.0f);
		BrickCollide = isAccurateCollideTopt(GoombaAIEffectList[i], BricksVertPosList, CurrPosYCollide, NoAdd, nd, be, 80.0f);
		be = find_max_iny(GoombaAIEffectList[i], LuckyVertPosList);
		nd = find_min_iny_dist(GoombaAIEffectList[i], LuckyVertPosList, 64.0f - (GoombaAIEffectYveloList[i]) * 16.0f);
		LuckyCollide = isAccurateCollideTopt(GoombaAIEffectList[i], LuckyVertPosList, CurrPosYCollide, NoAdd, nd, be, 80.0f);
		//ObstacleCollide = isAccurateCollideTop(GoombaAIList[i], ObstaclesList, CurrPosYCollide, NoAdd, {});
		//BrickCollide = isAccurateCollideTop(GoombaAIList[i], Bricks, CurrPosYCollide, NoAdd, BrickSaveList);
		//LuckyCollide = isAccurateCollideTop(GoombaAIList[i], LuckyBlock, CurrPosYCollide, NoAdd, LuckyBlockSaveList);
		if ((ObstacleCollide || BrickCollide || LuckyCollide) && GoombaAIEffectYveloList[i] < 0.0f) {
			GoombaAIEffectYveloList[i] = 0.0f;
			GoombaAIEffectList[i].property.setPosition({ GoombaAIEffectList[i].property.getPosition().x, CurrPosYCollide + (31.0f + GoombaAIEffectList[i].property.getOrigin().y - GoombaAIEffectDefinationList[i][3]) });
			//if (ObstacleCollide.second || BrickCollide.second || LuckyCollide.second) {
			//	GoombaAIEffectList[i].property.setPosition({ GoombaAIEffectList[i].property.getPosition().x, CurrPosYCollide + (31.0f + GoombaAIEffectList[i].property.getOrigin().y - GoombaAIEffectDefinationList[i][3]) });
			//}
			//if (!ObstacleCollide.second && !BrickCollide.second && !LuckyCollide.second) {
				//GoombaAIEffectList[i].property.setPosition({ GoombaAIEffectList[i].property.getPosition().x, CurrPosYCollide + (31.0f + GoombaAIEffectList[i].property.getOrigin().y - GoombaAIEffectDefinationList[i][3]) });
			//}
		}
	}
}
/* void GoombaAIEffectVertYUpdate() {
	std::pair<bool, bool> ObstacleCollide, BrickCollide, LuckyCollide;
	bool ObstacleCheck, BrickCheck, LuckyCheck, isLanding;
	float CurrPosYCollide;
	bool NoAdd;
	for (int i = 0; i < GoombaAIEffectList.size(); ++i) {
		if (GoombaAIEffectTypeList[i] == NONE) {
			GoombaAIEffectYveloList[i] += 0.5f * deltaTime * 0.15f;
			GoombaAIEffectList[i].property.move({ 0.0f, GoombaAIEffectYveloList[i] * deltaTime });
			GoombaAIEffectYveloList[i] += 0.5f * deltaTime * 0.15f;
			continue;
		}
		// bottom update
		ObstacleCheck = isCollideBot(GoombaAIEffectList[i], ObstaclesList);
		BrickCheck = isCollideBot(GoombaAIEffectList[i], Bricks);
		LuckyCheck = isCollideBot(GoombaAIEffectList[i], LuckyBlock);
		if (!ObstacleCheck && !BrickCheck && !LuckyCheck) {
			GoombaAIEffectYveloList[i] += 0.5f * deltaTime * 0.3f;
			GoombaAIEffectList[i].property.move({ 0.0f, GoombaAIEffectYveloList[i] * deltaTime });
			GoombaAIEffectYveloList[i] += 0.5f * deltaTime * 0.3f;
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
				if (ObstacleCollide.second || BrickCollide.second || LuckyCollide.second) {
					GoombaAIEffectList[i].property.setPosition({ GoombaAIEffectList[i].property.getPosition().x, CurrPosYCollide - (GoombaAIEffectDefinationList[i][1] - GoombaAIEffectList[i].property.getOrigin().y + GoombaAIEffectDefinationList[i][3]) });
				}
				if (!ObstacleCollide.second && !BrickCollide.second && !LuckyCollide.second) {
					GoombaAIEffectList[i].property.setPosition({ GoombaAIEffectList[i].property.getPosition().x, CurrPosYCollide - (GoombaAIEffectDefinationList[i][1] - GoombaAIEffectList[i].property.getOrigin().y + GoombaAIEffectDefinationList[i][3]) });
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
				if (ObstacleCollide.second || BrickCollide.second || LuckyCollide.second) {
					GoombaAIEffectList[i].property.setPosition({ GoombaAIEffectList[i].property.getPosition().x, CurrPosYCollide + (31.0f + GoombaAIEffectList[i].property.getOrigin().y - GoombaAIEffectDefinationList[i][3]) });
				}
				if (!ObstacleCollide.second && !BrickCollide.second && !LuckyCollide.second) {
					GoombaAIEffectList[i].property.setPosition({ GoombaAIEffectList[i].property.getPosition().x, CurrPosYCollide + (31.0f + GoombaAIEffectList[i].property.getOrigin().y - GoombaAIEffectDefinationList[i][3]) });
				}
			}
		}
	}
}
*/