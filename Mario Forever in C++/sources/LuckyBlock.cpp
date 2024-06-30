#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../headers/LuckyBlock.hpp"
#include "../headers/Obstacles.hpp"
#include "../headers/WindowFrame.hpp"
#include "../headers/Coin.hpp"
#include "../headers/Scroll.hpp"
#include "../headers/CoinEffect.hpp"
#include "../headers/enum.hpp"
#include "../headers/AnimationManager.hpp"
#include "../headers/Loading.hpp"
#include "../headers/GoombaAI.hpp"
#include "../headers/ScoreEffect.hpp"
#include "../headers/Collide.hpp"

#include "../resource.h"

#include <vector>
#include <iostream>

std::vector<Obstacles> LuckyBlock;
std::vector<LuckyBlockID> LuckyBlockIDList;
std::vector<LuckyBlockAtt> LuckyBlockAttList;
std::vector<bool> LuckyBlockState;
std::vector<float> LuckyBlockStateCount;
std::vector<bool> LuckyUpDown;
std::vector<std::pair<float, float>> LuckyBlockSaveList;
std::vector<LocalAnimationManager> LuckyIdle;
std::vector<bool> LuckyBlockHitted;

sf::Texture LuckyBlockTexture;
AnimationManager LuckyBlockAnimationManager;
int LoadLuckyBlock() {
	LoadTexture(LuckyBlockTexture, LUCKYBLOCK_TEXTURE, IMAGEFILE);
	return 6;
}
int LuckyBlockInit = LoadLuckyBlock();
void AddLuckyBlock(LuckyBlockID ID, LuckyBlockAtt Att, float x, float y) {
	if (ID == LUCKY_COIN) LuckyBlock.push_back(Obstacles{ 0, sf::Sprite(LuckyBlockTexture, sf::IntRect(0, 0, 32, 32)) });
	else if (ID == TREE_LUCKY_COIN) LuckyBlock.push_back(Obstacles{ 0, sf::Sprite(LuckyBlockTexture, sf::IntRect(64, 32, 32, 32)) });
	LuckyBlockAttList.push_back(Att);
	LuckyBlockIDList.push_back(ID);
	LuckyBlockState.push_back(false);
	LuckyBlockStateCount.push_back(0);
	LuckyBlockSaveList.push_back({ x, y });
	LuckyBlockHitted.push_back(false);
	LuckyUpDown.push_back(false);
	LuckyBlock[LuckyBlock.size() - 1].property.setPosition({ x, y });
	setHitbox(LuckyBlock[LuckyBlock.size() - 1].hitbox, { 0.f, 0.f, 32.f, 32.f });
	LuckyIdle.push_back(LocalAnimationManager());
	LuckyIdle[LuckyIdle.size() - 1].setAnimation({ 32,32 }, { 0,0 }, { 3,0 }, 9);
}
inline void LuckyBlockUpdate() {
	for (int i = 0; i < LuckyBlock.size(); i++) {
		if (LuckyBlockState[i]) {
			if (!LuckyUpDown[i]) {
				if (LuckyBlockStateCount[i] < 11.0f) {
					LuckyBlock[i].property.move(0, 0 - (LuckyBlockStateCount[i] < 6.0f ? 3.0f : (LuckyBlockStateCount[i] < 10.0f ? 2.0f : 1.0f)) * deltaTime);
					LuckyBlockStateCount[i] += (LuckyBlockStateCount[i] < 6.0f ? 3.0f : (LuckyBlockStateCount[i] < 10.0f ? 2.0f : 1.0f)) * deltaTime;
				}
				else {
					LuckyBlockStateCount[i] = 11.0f;
					LuckyUpDown[i] = true;
				}
			}
			else {
				if (LuckyBlockStateCount[i] > 0.0f) {
					LuckyBlock[i].property.move(0, (LuckyBlockStateCount[i] > 10.0f ? 1.0f : (LuckyBlockStateCount[i] > 6.0f ? 2.0f : 3.0f)) * deltaTime);
					LuckyBlockStateCount[i] -= (LuckyBlockStateCount[i] > 10.0f ? 1.0f : (LuckyBlockStateCount[i] > 6.0f ? 2.0f : 3.0f)) * deltaTime;
				}
				else {
					LuckyBlock[i].property.setPosition(LuckyBlockSaveList[i].first, LuckyBlockSaveList[i].second);
					LuckyBlockStateCount[i] = 0.0f;
					LuckyUpDown[i] = false;
					LuckyBlockState[i] = false;
				}
			}
		}
		if (!isOutScreen(LuckyBlock[i].property.getPosition().x, LuckyBlock[i].property.getPosition().y, 32, 32)) {
			window.draw(LuckyBlock[i].property);
		}
	}
}
inline void LuckyAnimationUpdate() {
	for (int i = 0; i < LuckyBlock.size(); i++) {
		if (LuckyBlockIDList[i] == LUCKY_COIN) {
			if (!LuckyBlockHitted[i]) LuckyIdle[i].update(LuckyBlock[i].property, LuckyBlockTexture);
			else {
				LuckyBlock[i].property.setTextureRect(sf::IntRect(96, 0, 32, 32));
			}
		}
		else if (LuckyBlockIDList[i] == TREE_LUCKY_COIN) {
			if (LuckyBlockHitted[i]) LuckyBlock[i].property.setTextureRect(sf::IntRect(96, 32, 32, 32));
		}
	}
}
void LuckyHitEvent(float x, float y) {
	sf::FloatRect LuckyLoop;
	for (int i = 0; i < LuckyBlock.size(); i++) {
		if (LuckyBlock[i].property.getPosition().x == x && LuckyBlock[i].property.getPosition().y == y && !LuckyBlockState[i] && !LuckyBlockHitted[i]) {
			LuckyLoop = getGlobalHitbox(LuckyBlock[i].hitbox, LuckyBlock[i].property);
			LuckyLoop.top -= 32.0f;
			for (int j = 0; j < CoinList.size(); ++j) {
				if (isCollide(CoinList[j].hitbox, CoinList[j].property, LuckyLoop)) {
					AddCoinEffect(CoinIDList[j], CoinAttList[j], CoinList[j].property.getPosition().x - 3, CoinList[j].property.getPosition().y);
					DeleteCoin(CoinList[j].property.getPosition().x, CoinList[j].property.getPosition().y);
					CoinSound.play();
					++CoinCount;
				}
			}
			for (int j = 0; j < GoombaAIList.size(); ++j) {
				if (isCollide(GoombaAIList[j].hitboxMain, GoombaAIList[j].property, LuckyLoop)) {
					if (GoombaAITypeList[j] == GOOMBA) AddScoreEffect(SCORE_100, GoombaAIList[j].property.getPosition().x - 15.0f, GoombaAIList[j].property.getPosition().y - GoombaAIHitboxList[0].second);
					DeleteGoombaAI(GoombaAITypeList[j], GoombaAIList[j].property.getPosition().x, GoombaAIList[j].property.getPosition().y);
					Kick2Sound.play();
				}
			}
			if (LuckyBlockAttList[i] == COIN) {
				CoinSound.play();
				AddCoinEffect(COIN_NORMAL, ONE_COIN, LuckyLoop.getPosition().x - 3, LuckyLoop.getPosition().y);
				++CoinCount;
			}
			LuckyBlockState[i] = true;
			LuckyUpDown[i] = false;
			LuckyBlockStateCount[i] = 0;
			LuckyBlockHitted[i] = true;
			break;
		}
	}
}
void deleteLuckyBlock(float x, float y) {
	for (int i = 0; i < LuckyBlock.size(); i++) {
		if (LuckyBlock[i].property.getPosition().x == x && LuckyBlock[i].property.getPosition().y == y) {
			LuckyBlock.erase(LuckyBlock.begin() + i);
			LuckyBlockAttList.erase(LuckyBlockAttList.begin() + i);
			LuckyBlockIDList.erase(LuckyBlockIDList.begin() + i);
			LuckyBlockState.erase(LuckyBlockState.begin() + i);
			LuckyBlockStateCount.erase(LuckyBlockStateCount.begin() + i);
			LuckyBlockSaveList.erase(LuckyBlockSaveList.begin() + i);
			LuckyUpDown.erase(LuckyUpDown.begin() + i);
			break;
		}
	}
}