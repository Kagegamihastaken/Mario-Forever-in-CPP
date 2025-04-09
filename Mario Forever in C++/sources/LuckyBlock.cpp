#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../headers/Block/LuckyBlock.hpp"
#include "../headers/Block/Obstacles.hpp"
#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Object/Coin.hpp"
#include "../headers/Core/Scroll.hpp"
#include "../headers/Effect/CoinEffect.hpp"
#include "../headers/Core/Loading/enum.hpp"
#include "../headers/Core/Loading/Loading.hpp"
#include "../headers/Object/GoombaAI.hpp"
#include "../headers/Effect/ScoreEffect.hpp"
#include "../headers/Core/Collision/Collide.hpp"
#include "../headers/Effect/GoombaAIEffect.hpp"
#include "../headers/Core/Sound.hpp"
#include "../headers/Core/TextureManager.hpp"

#include "../resource.h"

#include <vector>
#include <iostream>

std::vector<Obstacles> LuckyBlock;
std::vector<std::pair<sf::FloatRect, sf::Vector2f>> LuckyVertPosList;
std::vector<LuckyBlockID> LuckyBlockIDList;
std::vector<LuckyBlockAtt> LuckyBlockAttList;
std::vector<bool> LuckyBlockState;
std::vector<float> LuckyBlockStateCount;
std::vector<bool> LuckyUpDown;
std::vector<std::pair<float, float>> LuckyBlockSaveList;
std::vector<LocalAnimationManager> LuckyIdle;
std::vector<bool> LuckyBlockHitted;

TextureManager LuckyBlockTextureManager;
std::vector<sf::Texture*> LuckyBlockTextureList;

void LoadLuckyBlock() {
	LuckyBlockTextureManager.LoadingAnimatedTexture(LUCKYBLOCK_TEXTURE, "LuckyBlockAnimated", 0, 2, 0, 32, 32);
	LuckyBlockTextureManager.Loadingtexture(LUCKYBLOCK_TEXTURE, "LuckyBlockHitted", 96, 0, 32, 32);
	//Tree
	LuckyBlockTextureManager.Loadingtexture(LUCKYBLOCK_TEXTURE, "LuckyBlockTree", 64, 32, 32, 32);
	LuckyBlockTextureManager.Loadingtexture(LUCKYBLOCK_TEXTURE, "LuckyBlockTreeHitted", 96, 32, 32, 32);
}
void AddLuckyBlock(LuckyBlockID ID, LuckyBlockAtt Att, float x, float y) {
	switch (ID) {
	case LUCKY_BLOCK:
		LuckyBlock.push_back(Obstacles{ 0, sf::Sprite(*LuckyBlockTextureManager.GetTexture("LuckyBlockHitted")) });
		break;
	case TREE_LUCKY_BLOCK:
		LuckyBlock.push_back(Obstacles{ 0, sf::Sprite(*LuckyBlockTextureManager.GetTexture("LuckyBlockTree")) });
		break;
	}
	LuckyBlockAttList.push_back(Att);
	LuckyBlockIDList.push_back(ID);
	LuckyBlockState.push_back(false);
	LuckyBlockStateCount.push_back(0);
	LuckyBlockSaveList.push_back({ x, y });
	LuckyBlockHitted.push_back(false);
	LuckyUpDown.push_back(false);
	LuckyBlock[LuckyBlock.size() - 1].property.setPosition({ x, y });
	setHitbox(LuckyBlock[LuckyBlock.size() - 1].hitbox, sf::FloatRect({ 0.f, 0.f }, { 32.f, 32.f }));
	LuckyIdle.push_back(LocalAnimationManager());
	LuckyIdle.back().setAnimation(0, 2, 9);
	LuckyVertPosList.push_back({ sf::FloatRect({ 0.0f, 0.0f }, { 32.f, 32.f }), sf::Vector2f(x, y) });
	//LuckyIdle[LuckyIdle.size() - 1].setAnimation({ 32,32 }, { 0,0 }, { 3,0 }, 9);
}
void LuckyBlockSort() {
	if (LuckyBlock.size() == 0) return;
	sort(LuckyVertPosList.begin(), LuckyVertPosList.end(), [](std::pair<sf::FloatRect, sf::Vector2f>& A, std::pair<sf::FloatRect, sf::Vector2f>& B) {
		if (A.second.y < B.second.y) return true;
		else if (A.second.y == B.second.y) return A.second.x < B.second.x;
		else return false;
		});
}
inline void LuckyBlockUpdate() {
	for (int i = 0; i < LuckyBlock.size(); i++) {
		if (LuckyBlockState[i]) {
			if (LuckyBlockAttList[i] == LUCKY_COIN) {
				if (!LuckyUpDown[i]) {
					if (LuckyBlockStateCount[i] < 11.0f) {
						LuckyBlock[i].property.move({ 0, 0 - (LuckyBlockStateCount[i] < 6.0f ? 3.0f : (LuckyBlockStateCount[i] < 10.0f ? 2.0f : 1.0f)) * deltaTime });
						LuckyBlockStateCount[i] += (LuckyBlockStateCount[i] < 6.0f ? 3.0f : (LuckyBlockStateCount[i] < 10.0f ? 2.0f : 1.0f)) * deltaTime;
					}
					else {
						LuckyBlockStateCount[i] = 11.0f;
						LuckyUpDown[i] = true;
					}
				}
				else {
					if (LuckyBlockStateCount[i] > 0.0f) {
						LuckyBlock[i].property.move({ 0, (LuckyBlockStateCount[i] > 10.0f ? 1.0f : (LuckyBlockStateCount[i] > 6.0f ? 2.0f : 3.0f)) * deltaTime });
						LuckyBlockStateCount[i] -= (LuckyBlockStateCount[i] > 10.0f ? 1.0f : (LuckyBlockStateCount[i] > 6.0f ? 2.0f : 3.0f)) * deltaTime;
					}
					else {
						LuckyBlock[i].property.setPosition({ LuckyBlockSaveList[i].first, LuckyBlockSaveList[i].second });
						LuckyBlockStateCount[i] = 0.0f;
						LuckyUpDown[i] = false;
						LuckyBlockState[i] = false;
					}
				}
			}
			else {
				if (!LuckyUpDown[i]) {
					if (LuckyBlockStateCount[i] < 4.0f) {
						LuckyBlock[i].property.move({ 0, -1.0f * deltaTime });
						LuckyBlockStateCount[i] += 1.0f * deltaTime;
					}
					else {
						LuckyBlockStateCount[i] = 4.0f;
						LuckyUpDown[i] = true;
					}
				}
				else {
					if (LuckyBlockStateCount[i] > 0.0f) {
						LuckyBlock[i].property.move({ 0, 1.0f * deltaTime });
						LuckyBlockStateCount[i] -= 1.0f * deltaTime;
					}
					else {
						LuckyBlock[i].property.setPosition({ LuckyBlockSaveList[i].first, LuckyBlockSaveList[i].second });
						LuckyBlockStateCount[i] = 0.0f;
						LuckyUpDown[i] = false;
						LuckyBlockState[i] = false;
					}
				}
			}
		}
		if (!isOutScreen(LuckyBlock[i].property.getPosition().x, LuckyBlock[i].property.getPosition().y, 32, 32)) {
			window.draw(LuckyBlock[i].property);
		}
	}
}
inline void LuckyAnimationUpdate() {
	if (LuckyBlock.size() == 0) return;
	for (int i = 0; i < LuckyBlock.size(); i++) {
		if (LuckyBlockIDList[i] == LUCKY_BLOCK) {
			if (!LuckyBlockHitted[i]) LuckyIdle[i].update(LuckyBlock[i].property, LuckyBlockTextureManager.GetAnimatedTexture("LuckyBlockAnimated"));
			else {
				LuckyBlock[i].property.setTexture(*LuckyBlockTextureManager.GetTexture("LuckyBlockHitted"), true);
			}
		}
		else if (LuckyBlockIDList[i] == TREE_LUCKY_BLOCK) {
			if (LuckyBlockHitted[i]) LuckyBlock[i].property.setTexture(*LuckyBlockTextureManager.GetTexture("LuckyBlockTreeHitted"), true);
		}
	}
}
void LuckyHit(float x, float y, int i) {
	if (!LuckyBlockState[i] && !LuckyBlockHitted[i]) {
		switch (LuckyBlockAttList[i]) {
		case LUCKY_COIN:
			Sounds.PlaySound("Coin");
			AddCoinEffect(COIN_NORMAL, ONE_COIN, x - 3, y);
			++CoinCount;
			break;
		case LUCKY_MUSHROOM:
			Sounds.PlaySound("Vine");
			AddGoombaAI(MUSHROOM, 0, x + 16.0f, y + 63.0f, RIGHT);
			break;
		}
		LuckyBlockState[i] = true;
		LuckyUpDown[i] = false;
		LuckyBlockStateCount[i] = 0;
		LuckyBlockHitted[i] = true;
	}
}
int getLuckyIndex(float x, float y) {
	for (int i = 0; i < LuckyBlock.size(); i++) {
		if (LuckyBlockSaveList[i].first == x && LuckyBlockSaveList[i].second == y) {
			return i;
		}
	}
	return -1;
}
void LuckyHitEvent(float x, float y) {
	sf::FloatRect LuckyLoop;
	for (int i = 0; i < LuckyBlock.size(); i++) {
		if (LuckyBlock[i].property.getPosition().x == x && LuckyBlock[i].property.getPosition().y == y && !LuckyBlockState[i] && !LuckyBlockHitted[i]) {
			LuckyLoop = getGlobalHitbox(LuckyBlock[i].hitbox, LuckyBlock[i].property);
			LuckyLoop.position.y -= 32.0f;
			for (int j = 0; j < CoinList.size(); ++j) {
				if (isCollide(CoinList[j].hitbox, CoinList[j].property, LuckyLoop)) {
					AddCoinEffect(CoinIDList[j], CoinAttList[j], CoinList[j].property.getPosition().x - 3, CoinList[j].property.getPosition().y);
					DeleteCoin(CoinList[j].property.getPosition().x, CoinList[j].property.getPosition().y);
					Sounds.PlaySound("Coin");
					++CoinCount;
				}
			}
			for (int j = 0; j < GoombaAIList.size(); ++j) {
				if (isCollide(GoombaAIList[j].hitboxMain, GoombaAIList[j].property, LuckyLoop)) {
					if (GoombaAITypeList[j] != MUSHROOM) {
						if (GoombaAITypeList[j] == GOOMBA || GoombaAITypeList[j] == KOOPA || GoombaAITypeList[j] == SHELL || GoombaAITypeList[j] == SHELL_MOVING) AddScoreEffect(SCORE_100, GoombaAIList[j].property.getPosition().x - 15.0f, GoombaAIList[j].property.getPosition().y - GoombaAIHitboxList[j].second);
						AddGoombaAIEffect(GoombaAITypeList[j], NONE, GoombaAISkinIDList[j], GoombaAIList[j].property.getPosition().x, GoombaAIList[j].property.getPosition().y);
						DeleteGoombaAI(GoombaAITypeList[j], GoombaAIList[j].property.getPosition().x, GoombaAIList[j].property.getPosition().y);
						Sounds.PlaySound("Kick2");
					}
				}
			}
			LuckyHit(LuckyLoop.position.x, LuckyLoop.position.y, i);
			break;
		}
	}
}
void DeleteLuckyBlock(float x, float y) {
	for (int i = 0; i < LuckyVertPosList.size(); ++i) {
		if (LuckyVertPosList[i].second.x == x && LuckyVertPosList[i].second.y == y) {
			LuckyVertPosList.erase(LuckyVertPosList.begin() + i);
			break;
		}
	}
	for (int i = 0; i < LuckyBlock.size(); i++) {
		if (LuckyBlock[i].property.getPosition().x == x && LuckyBlock[i].property.getPosition().y == y) {
			LuckyBlock.erase(LuckyBlock.begin() + i);
			LuckyBlockAttList.erase(LuckyBlockAttList.begin() + i);
			LuckyBlockIDList.erase(LuckyBlockIDList.begin() + i);
			LuckyBlockState.erase(LuckyBlockState.begin() + i);
			LuckyBlockStateCount.erase(LuckyBlockStateCount.begin() + i);
			LuckyBlockSaveList.erase(LuckyBlockSaveList.begin() + i);
			LuckyBlockHitted.erase(LuckyBlockHitted.begin() + i);
			LuckyUpDown.erase(LuckyUpDown.begin() + i);
			LuckyIdle.erase(LuckyIdle.begin() + i);
			break;
		}
	}
}
void DeleteAllLuckyBlock() {
	LuckyBlock.clear();
	LuckyBlockAttList.clear();
	LuckyBlockIDList.clear();
	LuckyBlockState.clear();
	LuckyBlockStateCount.clear();
	LuckyBlockSaveList.clear();
	LuckyBlockHitted.clear();
	LuckyUpDown.clear();
	LuckyIdle.clear();
	LuckyVertPosList.clear();
}