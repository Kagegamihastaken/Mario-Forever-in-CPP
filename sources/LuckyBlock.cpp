#include <SFML/Graphics.hpp>
#include "Block/LuckyBlock.hpp"
#include "Block/Obstacles.hpp"
#include "Core/WindowFrame.hpp"
#include "Object/Coin.hpp"
#include "Core/Scroll.hpp"
#include "Effect/CoinEffect.hpp"
#include "Core/Loading/enum.hpp"
#include "Object/GoombaAI.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Core/Collision/Collide.hpp"
#include "Effect/GoombaAIEffect.hpp"
#include "Core/Sound.hpp"
#include "Core/TextureManager.hpp"
#include "Core/Interpolation.hpp"

#include <vector>

std::vector<Obstacles> LuckyBlock;
std::vector<std::pair<sf::FloatRect, sf::Vector2f>> LuckyVertPosList;
std::vector<LuckyBlockID> LuckyBlockIDList;
std::vector<LuckyBlockAtt> LuckyBlockAttList;
std::vector<bool> LuckyBlockState;
std::vector<float> LuckyBlockStateCount;
std::vector<bool> LuckyUpDown;
std::vector<std::pair<sf::FloatRect, sf::Vector2f>> LuckyHorzPosList;
std::vector<LocalAnimationManager> LuckyIdle;
std::vector<bool> LuckyBlockHitted;

TextureManager LuckyBlockTextureManager;
std::vector<sf::Texture*> LuckyBlockTextureList;

void LoadLuckyBlock() {
	LuckyBlockTextureManager.Loadingtexture("data/resources/luckyblock.png", "LuckyBlock", 0, 0, 128, 64);
}
void SetPrevLuckyBlockPos() {
	for (int i = 0; i < LuckyBlock.size(); i++) {
		LuckyBlock[i].prev = LuckyBlock[i].curr;
	}
}
void InterpolateLuckyBlockPos(const float alpha) {
	for (int i = 0; i < LuckyBlock.size(); i++) {
		LuckyBlock[i].property.setPosition(linearInterpolation(LuckyBlock[i].prev, LuckyBlock[i].curr, alpha));
	}
}
void AddLuckyBlock(const LuckyBlockID ID, const LuckyBlockAtt Att, float x, float y) {
	LuckyIdle.push_back(LocalAnimationManager());
	LuckyIdle.back().setAnimation(0, 2, 32, 32, 0, 9);
	switch (ID) {
	case LUCKY_BLOCK:
		LuckyBlock.push_back(Obstacles{ 0, sf::Sprite(*LuckyBlockTextureManager.GetTexture("LuckyBlock"), sf::IntRect({ 0, 0 }, { 32, 32 })) });
		LuckyIdle.back().setTexture(LuckyBlock.back().property, LuckyBlockTextureManager.GetTexture("LuckyBlock"));
		break;
	case TREE_LUCKY_BLOCK:
		LuckyBlock.push_back(Obstacles{ 0, sf::Sprite(*LuckyBlockTextureManager.GetTexture("LuckyBlock"), sf::IntRect({ 64, 32 }, { 32, 32 })) });
		break;
	}
	LuckyBlockAttList.push_back(Att);
	LuckyBlockIDList.push_back(ID);
	LuckyBlockState.push_back(false);
	LuckyBlockStateCount.push_back(0);
	LuckyHorzPosList.push_back({ sf::FloatRect({ 0.0f, 0.0f }, { 32.f, 32.f }), sf::Vector2f(x, y) });
	LuckyBlockHitted.push_back(false);
	LuckyUpDown.push_back(false);
	LuckyBlock.back().property.setPosition({ x, y });
	LuckyBlock.back().curr = LuckyBlock.back().prev = LuckyBlock.back().property.getPosition();
	setHitbox(LuckyBlock[LuckyBlock.size() - 1].hitbox, sf::FloatRect({ 0.f, 0.f }, { 32.f, 32.f }));
	//LuckyIdle.back().setAnimation(0, 2, 9);
	LuckyVertPosList.push_back({ sf::FloatRect({ 0.0f, 0.0f }, { 32.f, 32.f }), sf::Vector2f(x, y) });
	//LuckyIdle[LuckyIdle.size() - 1].setAnimation({ 32,32 }, { 0,0 }, { 3,0 }, 9);
}
void LuckyBlockSort() {
	if (LuckyBlock.size() == 0) return;
	std::ranges::sort(LuckyVertPosList, [](const std::pair<sf::FloatRect, sf::Vector2f>& A, const std::pair<sf::FloatRect, sf::Vector2f>& B) {
		if (A.second.y < B.second.y) return true;
		else if (A.second.y == B.second.y) return A.second.x < B.second.x;
		else return false;
		});
}
void LuckyBlockDraw() {
	for (int i = 0; i < LuckyBlock.size(); i++) {
		if (!isOutScreen(LuckyBlock[i].property.getPosition().x, LuckyBlock[i].property.getPosition().y, 32, 32)) {
			rObject.draw(LuckyBlock[i].property);
		}
	}
}
void LuckyBlockUpdate(const float deltaTime) {
	for (int i = 0; i < LuckyBlock.size(); i++) {
		if (LuckyBlockState[i]) {
			if (LuckyBlockAttList[i] == LUCKY_COIN) {
				if (!LuckyUpDown[i]) {
					if (LuckyBlockStateCount[i] < 11.0f) {
						LuckyBlock[i].curr = { LuckyBlock[i].curr.x, LuckyBlock[i].curr.y - (LuckyBlockStateCount[i] < 6.0f ? 3.0f : (LuckyBlockStateCount[i] < 10.0f ? 2.0f : 1.0f)) * deltaTime };
						LuckyBlockStateCount[i] += (LuckyBlockStateCount[i] < 6.0f ? 3.0f : (LuckyBlockStateCount[i] < 10.0f ? 2.0f : 1.0f)) * deltaTime;
					}
					else {
						LuckyBlockStateCount[i] = 11.0f;
						LuckyUpDown[i] = true;
					}
				}
				else {
					if (LuckyBlockStateCount[i] > 0.0f) {
						LuckyBlock[i].curr = { LuckyBlock[i].curr.x, LuckyBlock[i].curr.y + (LuckyBlockStateCount[i] > 10.0f ? 1.0f : (LuckyBlockStateCount[i] > 6.0f ? 2.0f : 3.0f)) * deltaTime };
						LuckyBlockStateCount[i] -= (LuckyBlockStateCount[i] > 10.0f ? 1.0f : (LuckyBlockStateCount[i] > 6.0f ? 2.0f : 3.0f)) * deltaTime;
					}
					else {
						LuckyBlock[i].curr = { LuckyHorzPosList[i].second.x, LuckyHorzPosList[i].second.y };
						LuckyBlockStateCount[i] = 0.0f;
						LuckyUpDown[i] = false;
						LuckyBlockState[i] = false;
					}
				}
			}
			else {
				if (!LuckyUpDown[i]) {
					if (LuckyBlockStateCount[i] < 4.0f) {
						LuckyBlock[i].curr = { LuckyBlock[i].curr.x,LuckyBlock[i].curr.y - 1.0f * deltaTime };
						LuckyBlockStateCount[i] += 1.0f * deltaTime;
					}
					else {
						LuckyBlockStateCount[i] = 4.0f;
						LuckyUpDown[i] = true;
					}
				}
				else {
					if (LuckyBlockStateCount[i] > 0.0f) {
						LuckyBlock[i].curr = { LuckyBlock[i].curr.x, LuckyBlock[i].curr.y + 1.0f * deltaTime };
						LuckyBlockStateCount[i] -= 1.0f * deltaTime;
					}
					else {
						LuckyBlock[i].curr = { LuckyHorzPosList[i].second.x, LuckyHorzPosList[i].second.y };
						LuckyBlockStateCount[i] = 0.0f;
						LuckyUpDown[i] = false;
						LuckyBlockState[i] = false;
					}
				}
			}
		}
	}
}
void LuckyAnimationUpdate() {
	if (LuckyBlock.size() == 0) return;
	for (int i = 0; i < LuckyBlock.size(); i++) {
		if (LuckyBlockIDList[i] == LUCKY_BLOCK) {
			if (!LuckyBlockHitted[i]) LuckyIdle[i].update(LuckyBlock[i].property);
			else {
				LuckyBlock[i].property.setTextureRect(sf::IntRect({ 96, 0 }, { 32, 32 }));
			}
		}
		else if (LuckyBlockIDList[i] == TREE_LUCKY_BLOCK) {
			if (LuckyBlockHitted[i]) LuckyBlock[i].property.setTextureRect(sf::IntRect({ 96, 32 }, { 32, 32 }));
		}
	}
}
void LuckyHit(const float x, const float y, const int i) {
	if (!LuckyBlockState[i] && !LuckyBlockHitted[i]) {
		switch (LuckyBlockAttList[i]) {
		case LUCKY_COIN:
			Sounds.PlaySound("Coin");
			AddCoinEffect(COIN_NORMAL, ONE_COIN, x + 15.0f, y + 32.0f);
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
int getLuckyIndex(const float x, const float y) {
	for (int i = 0; i < LuckyBlock.size(); i++) {
		if (LuckyHorzPosList[i].second.x == x && LuckyHorzPosList[i].second.y == y) {
			return i;
		}
	}
	return -1;
}
void LuckyHitEvent(const float x, const float y) {
	for (int i = 0; i < LuckyBlock.size(); i++) {
		if (LuckyBlock[i].curr.x == x && LuckyBlock[i].curr.y == y && !LuckyBlockState[i] && !LuckyBlockHitted[i]) {
			sf::FloatRect LuckyLoop = getGlobalHitbox(LuckyBlock[i].hitbox, LuckyBlock[i].property);
			LuckyLoop.position.y -= 32.0f;
			for (int j = 0; j < CoinList.size(); ++j) {
				if (isCollide(CoinList[j].hitbox, CoinList[j].property, LuckyLoop)) {
					AddCoinEffect(CoinIDList[j], CoinAttList[j], CoinList[j].property.getPosition().x + 15.0f, CoinList[j].property.getPosition().y + 32.0f);
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
						DeleteGoombaAI(GoombaAITypeList[j], GoombaAIList[j].curr.x, GoombaAIList[j].curr.y);
						Sounds.PlaySound("Kick2");
					}
				}
			}
			LuckyHit(LuckyLoop.position.x, LuckyLoop.position.y, i);
			break;
		}
	}
}
void DeleteLuckyBlock(const float x, const float y) {
	for (int i = 0; i < LuckyVertPosList.size(); ++i) {
		if (LuckyVertPosList[i].second.x == x && LuckyVertPosList[i].second.y == y) {
			LuckyVertPosList.erase(LuckyVertPosList.begin() + i);
			break;
		}
	}
	for (int i = 0; i < LuckyBlock.size(); i++) {
		if (LuckyBlock[i].curr.x == x && LuckyBlock[i].curr.y == y) {
			LuckyBlock.erase(LuckyBlock.begin() + i);
			LuckyBlockAttList.erase(LuckyBlockAttList.begin() + i);
			LuckyBlockIDList.erase(LuckyBlockIDList.begin() + i);
			LuckyBlockState.erase(LuckyBlockState.begin() + i);
			LuckyBlockStateCount.erase(LuckyBlockStateCount.begin() + i);
			LuckyHorzPosList.erase(LuckyHorzPosList.begin() + i);
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
	LuckyHorzPosList.clear();
	LuckyBlockHitted.clear();
	LuckyUpDown.clear();
	LuckyIdle.clear();
	LuckyVertPosList.clear();
}