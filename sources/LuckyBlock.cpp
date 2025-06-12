#include <SFML/Graphics.hpp>
#include "Block/LuckyBlock.hpp"

#include <iostream>

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
#include "Core/ImageManager.hpp"
#include "Core/Interpolation.hpp"

#include <vector>
#include <set>

std::vector<Obstacles> LuckyBlock;
std::vector<std::pair<sf::FloatRect, sf::Vector2f>> LuckyVertPosList;
std::vector<LuckyBlockID> LuckyBlockIDList;
std::vector<LuckyBlockAtt> LuckyBlockAttList;
std::vector<bool> LuckyBlockState;
std::vector<float> LuckyBlockStateCount;
std::vector<bool> LuckyUpDown;
std::vector<std::pair<sf::FloatRect, sf::Vector2f>> LuckyHorzPosList;
std::vector<SingleAnimationObject> LuckyIdle;
std::vector<bool> LuckyBlockHitted;
static std::vector<std::string> NormLuckyBlockAnimName;

void LoadLuckyBlock() {
	ImageManager::AddImage("LuckyBlockImage", "data/resources/luckyblock.png");
	for (int i = 0; i < 3; ++i) {
		ImageManager::AddTexture("LuckyBlockImage", sf::IntRect({i * 32, 0}, {32, 32}), "NormalLuckyBlock_" + std::to_string(i));
		NormLuckyBlockAnimName.emplace_back("NormalLuckyBlock_" + std::to_string(i));
	}
	ImageManager::AddTexture("LuckyBlockImage", sf::IntRect({96, 0}, {32, 32}), "NormalLuckyBlockHit");
	ImageManager::AddTexture("LuckyBlockImage", sf::IntRect({64, 32}, {32, 32}), "TreeLuckyBlock");
	ImageManager::AddTexture("LuckyBlockImage", sf::IntRect({96, 32}, {32, 32}), "TreeLuckyBlockHit");
	//LuckyBlockTextureManager.Loadingtexture("data/resources/luckyblock.png", "LuckyBlock", 0, 0, 128, 64);
}
void SetPrevLuckyBlockPos() {
	for (auto & i : LuckyBlock) {
		i.prev = i.curr;
	}
}
void InterpolateLuckyBlockPos(const float alpha) {
	for (auto & i : LuckyBlock) {
		i.property.setPosition(linearInterpolation(i.prev, i.curr, alpha));
	}
}
void AddLuckyBlock(const LuckyBlockID ID, const LuckyBlockAtt Att, float x, float y) {
	LuckyIdle.emplace_back();
	LuckyIdle.back().setAnimation(0, 2, 9);
	switch (ID) {
	case LUCKY_BLOCK:
		LuckyBlock.push_back(Obstacles{ 0, sf::Sprite(ImageManager::GetTexture("NormalLuckyBlock_0")) });
		LuckyIdle.back().SetAnimationSequence(NormLuckyBlockAnimName, NormLuckyBlockAnimName);
		break;
	case TREE_LUCKY_BLOCK:
		LuckyBlock.push_back(Obstacles{ 0, sf::Sprite(ImageManager::GetTexture("TreeLuckyBlock")) });
		break;
	}
	LuckyBlockAttList.push_back(Att);
	LuckyBlockIDList.push_back(ID);
	LuckyBlockState.push_back(false);
	LuckyBlockStateCount.push_back(0);
	LuckyHorzPosList.emplace_back( sf::FloatRect({ 0.0f, 0.0f }, { 32.f, 32.f }), sf::Vector2f(x, y) );
	LuckyBlockHitted.push_back(false);
	LuckyUpDown.push_back(false);
	LuckyBlock.back().property.setPosition({ x, y });
	LuckyBlock.back().curr = LuckyBlock.back().prev = LuckyBlock.back().property.getPosition();
	setHitbox(LuckyBlock[LuckyBlock.size() - 1].hitbox, sf::FloatRect({ 0.f, 0.f }, { 32.f, 32.f }));
	//LuckyIdle.back().setAnimation(0, 2, 9);
	LuckyVertPosList.emplace_back( sf::FloatRect({ 0.0f, 0.0f }, { 32.f, 32.f }), sf::Vector2f(x, y) );
	//LuckyIdle[LuckyIdle.size() - 1].setAnimation({ 32,32 }, { 0,0 }, { 3,0 }, 9);
}
void LuckyBlockSort() {
	if (LuckyBlock.empty()) return;
	std::ranges::sort(LuckyVertPosList, [](const std::pair<sf::FloatRect, sf::Vector2f>& A, const std::pair<sf::FloatRect, sf::Vector2f>& B) {
		if (A.second.y < B.second.y) return true;
		else if (A.second.y == B.second.y) return A.second.x < B.second.x;
		else return false;
		});
}
void LuckyBlockDraw() {
	for (int i = 0; i < LuckyBlock.size(); i++) {
		if (!isOutScreen(LuckyBlock[i].property.getPosition().x, LuckyBlock[i].property.getPosition().y, 32, 32)) {
			if (LuckyBlockIDList[i] == LUCKY_BLOCK && !LuckyBlockHitted[i]) {
				LuckyIdle[i].AnimationUpdate(LuckyBlock[i].property.getPosition(), LuckyBlock[i].property.getOrigin());
				LuckyIdle[i].AnimationDraw(window);
			}
			else window.draw(LuckyBlock[i].property);
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
void LuckyBlockHittedUpdate(const int index) {
	if (LuckyBlockIDList[index] == LUCKY_BLOCK) {
		LuckyBlock[index].property.setTexture(ImageManager::GetTexture("NormalLuckyBlockHit"));
	}
	else if (LuckyBlockIDList[index] == TREE_LUCKY_BLOCK) {
		LuckyBlock[index].property.setTexture(ImageManager::GetTexture("TreeLuckyBlockHit"));
	}
}
void LuckyHit(const float x, const float y, const int i) {
	if (!LuckyBlockState[i] && !LuckyBlockHitted[i]) {
		switch (LuckyBlockAttList[i]) {
		case LUCKY_COIN:
			SoundManager::PlaySound("Coin");
			AddCoinEffect(COIN_NORMAL, ONE_COIN, x + 15.0f, y + 32.0f);
			++CoinCount;
			break;
		case LUCKY_MUSHROOM:
			SoundManager::PlaySound("Vine");
			AddGoombaAI(MUSHROOM, 0, x + 16.0f, y + 63.0f, RIGHT);
			break;
		}
		LuckyBlockHittedUpdate(i);
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
	std::set<std::pair<GoombaAIType, std::pair<float, float>>> GoombaAIDeleteSet;
	for (int i = 0; i < LuckyBlock.size(); i++) {
		if (LuckyBlock[i].curr.x == x && LuckyBlock[i].curr.y == y && !LuckyBlockState[i] && !LuckyBlockHitted[i]) {
			sf::FloatRect LuckyLoop = getGlobalHitbox(LuckyBlock[i].hitbox, LuckyBlock[i].property);
			LuckyLoop.position.y -= 32.0f;
			for (int j = 0; j < CoinList.size(); ++j) {
				if (isCollide(CoinList[j].hitbox, CoinList[j].property, LuckyLoop)) {
					AddCoinEffect(CoinIDList[j], CoinAttList[j], CoinList[j].property.getPosition().x + 15.0f, CoinList[j].property.getPosition().y + 32.0f);
					DeleteCoin(CoinList[j].property.getPosition().x, CoinList[j].property.getPosition().y);
					SoundManager::PlaySound("Coin");
					++CoinCount;
				}
			}
			for (auto &j : GoombaAIList) {
				if (sf::FloatRect GoombaAICollide = getGlobalHitbox(j.GetHitboxMain(), j.getCurrentPosition(), j.getOrigin()); isCollide(GoombaAICollide, LuckyLoop)) {
					j.DeathBehaviour(SCORE_100);
					if (j.IsCanDeath()) GoombaAIDeleteSet.insert({j.GetType(), {j.getCurrentPosition().x, j.getCurrentPosition().y}});
				}
			}
			LuckyHit(LuckyLoop.position.x, LuckyLoop.position.y, i);
			break;
		}
	}
	if (!GoombaAIDeleteSet.empty())
		for (const auto &[fst, snd] : GoombaAIDeleteSet)
			DeleteGoombaAI(fst, snd.first, snd.second);
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