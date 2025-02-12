#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../headers/Block/Brick.hpp"
#include "../headers/Block/Obstacles.hpp"
#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Object/Coin.hpp"
#include "../headers/Core/Scroll.hpp"
#include "../headers/Effect/CoinEffect.hpp"
#include "../headers/Core/Loading/enum.hpp"
#include "../headers/Core/Loading/Loading.hpp"
#include "../headers/Object/Mario.hpp"
#include "../headers/Effect/BrickParticle.hpp"
#include "../headers/Object/GoombaAI.hpp"
#include "../headers/Effect/ScoreEffect.hpp"
#include "../headers/Core/Collision/Collide.hpp"
#include "../headers/Core/Sound.hpp"
#include "../headers/Effect/GoombaAIEffect.hpp"
#include "../headers/Core/TextureManager.hpp"

#include "../resource.h"

#include <fstream>
#include <streambuf>
#include <vector>
#include <iostream>

std::vector<Obstacles> Bricks;
std::vector<bool> BrickState;
std::vector<float> BrickStateCount;
std::vector<bool> UpDown;
std::vector<std::pair<float, float>> BrickSaveList;
std::vector<BrickID> BrickIDList;
std::vector<BrickAtt> BrickAttList;
//multicoin attribute
std::vector<sf::Clock> BrickClock;
std::vector<bool> BrickHitted;
std::vector<bool> DisabledBrick;

TextureManager BrickTextureManager;
// TODO: Load Texture from TextureManager
void LoadBricks() {
	// Loading Texture
	BrickTextureManager.Loadingtexture(BRICK_TEXTURE, "Brick_Normal", 0, 0, 32, 32);
	BrickTextureManager.Loadingtexture(BRICK_TEXTURE, "Brick_Gray", 32, 0, 32, 32);
	BrickTextureManager.Loadingtexture(BRICK_TEXTURE, "Brick_Normal_Hitted", 0, 32, 32, 32);
	BrickTextureManager.Loadingtexture(BRICK_TEXTURE, "Brick_Gray_Hitted", 32, 32, 32, 32);
}
void AddBrick(BrickID ID, BrickAtt att, float x, float y) {
	switch (ID) {
	case BRICK_GRAY:
		Bricks.push_back(Obstacles{ 0, sf::Sprite(*BrickTextureManager.GetTexture("Brick_Gray")) });
		break;
	case BRICK_NORMAL:
		Bricks.push_back(Obstacles{ 0, sf::Sprite(*BrickTextureManager.GetTexture("Brick_Normal")) });
		break;
	}
	BrickAttList.push_back(att);
	BrickIDList.push_back(ID);
	BrickState.push_back(false);
	BrickStateCount.push_back(0);
	BrickSaveList.push_back({ x, y });
	UpDown.push_back(false);
	Bricks[Bricks.size() - 1].property.setPosition({ x, y });
	setHitbox(Bricks[Bricks.size() - 1].hitbox, sf::FloatRect({ 0.f, 0.f }, { 32.f, 32.f }));
	//multicoin attribute
	BrickClock.push_back(sf::Clock());
	BrickHitted.push_back(false);
	DisabledBrick.push_back(false);
}
inline BrickID GetIDBrick(float x, float y) {
	for (int i = 0; i < Bricks.size(); i++) {
		if (Bricks[i].property.getPosition().x == x && Bricks[i].property.getPosition().y == y) {
			return BrickIDList[i];
		}
	}
}
inline BrickAtt GetBrickAtt(float x, float y) {
	for (int i = 0; i < Bricks.size(); i++) {
		if (Bricks[i].property.getPosition().x == x && Bricks[i].property.getPosition().y == y) {
			return BrickAttList[i];
		}
	}
}
inline void BrickStatusUpdate() {
	if (Bricks.size() == 0) return;
	for (int i = 0; i < Bricks.size(); ++i) {
		if (DisabledBrick[i] && BrickAttList[i] == MULTICOIN) {
			if (BrickIDList[i] == BRICK_GRAY) Bricks[i].property.setTexture(*BrickTextureManager.GetTexture("Brick_Gray_Hitted"), true);
			else if (BrickIDList[i] == BRICK_NORMAL) Bricks[i].property.setTexture(*BrickTextureManager.GetTexture("Brick_Normal_Hitted"), true);
		}
	}
}
inline void BrickUpdate() {
	for (int i = 0; i < Bricks.size(); i++) {
		if (BrickState[i]) {
			if (!UpDown[i]) {
				if (BrickStateCount[i] < 11.0f) {
					Bricks[i].property.move({ 0, 0 - (BrickStateCount[i] < 6.0f ? 3.0f : (BrickStateCount[i] < 10.0f ? 2.0f : 1.0f)) * deltaTime });
					BrickStateCount[i] += (BrickStateCount[i] < 6.0f ? 3.0f : (BrickStateCount[i] < 10.0f ? 2.0f : 1.0f)) * deltaTime;
				}
				else {
					BrickStateCount[i] = 11.0f;
					UpDown[i] = true;
				}
			}
			else {
				if (BrickStateCount[i] > 0.0f) {
					Bricks[i].property.move({ 0, (BrickStateCount[i] > 10.0f ? 1.0f : (BrickStateCount[i] > 6.0f ? 2.0f : 3.0f)) * deltaTime });
					BrickStateCount[i] -= (BrickStateCount[i] > 10.0f ? 1.0f : (BrickStateCount[i] > 6.0f ? 2.0f : 3.0f)) * deltaTime;
				}
				else {
					Bricks[i].property.setPosition({ BrickSaveList[i].first, BrickSaveList[i].second });
					BrickStateCount[i] = 0.0f;
					UpDown[i] = false;
					BrickState[i] = false;
				}
			}
		}
		if (!isOutScreen(Bricks[i].property.getPosition().x, Bricks[i].property.getPosition().y, 32, 32)) {
			window.draw(Bricks[i].property);
		}
	}
}
int getBrickIndex(float x, float y) {
	for (int i = 0; i < Bricks.size(); i++) {
		if (BrickSaveList[i].first == x && BrickSaveList[i].second == y) {
			return i;
		}
	}
}
void MultiBrickCoin(float x, float y, int i) {
	if (BrickAttList[i] == MULTICOIN && !DisabledBrick[i]) {
		if (!BrickHitted[i]) {
			BrickHitted[i] = true;
			BrickClock[i].restart();
		}
		else {
			if (BrickClock[i].getElapsedTime().asSeconds() > 6.0f && BrickAttList[i] == MULTICOIN) {
				DisabledBrick[i] = true;
				BrickState[i] = true;
				UpDown[i] = false;
				BrickStateCount[i] = 0;
			}
		}
		Sounds.PlaySound("Coin");
		AddCoinEffect(COIN_NORMAL, ONE_COIN, x - 3, y);
		++CoinCount;
		BrickState[i] = true;
		UpDown[i] = false;
		BrickStateCount[i] = 0;
	}
}
void HitEvent(float x, float y) {
	sf::FloatRect BrickLoop;
	for (int i = 0; i < Bricks.size(); i++) {
		if (Bricks[i].property.getPosition().x == x && Bricks[i].property.getPosition().y == y && !BrickState[i]) {
			BrickLoop = getGlobalHitbox(Bricks[i].hitbox, Bricks[i].property);
			BrickLoop.position.y -= 32.0f;
			for (int j = 0; j < CoinList.size(); ++j) {
				if (isCollide(CoinList[j].hitbox, CoinList[j].property, BrickLoop)) {
					AddCoinEffect(CoinIDList[j], CoinAttList[j], CoinList[j].property.getPosition().x - 3, CoinList[j].property.getPosition().y);
					DeleteCoin(CoinList[j].property.getPosition().x, CoinList[j].property.getPosition().y);
					Sounds.PlaySound("Coin");
					++CoinCount;
				}
			}
			for (int j = 0; j < GoombaAIList.size(); ++j) {
				if (isCollide(GoombaAIList[j].hitboxMain, GoombaAIList[j].property, BrickLoop)) {
					if (GoombaAITypeList[j] != MUSHROOM) {
						AddScoreEffect(SCORE_100, GoombaAIList[j].property.getPosition().x - 15.0f, GoombaAIList[j].property.getPosition().y - GoombaAIHitboxList[j].second);
						AddGoombaAIEffect(GoombaAITypeList[j], NONE, GoombaAISkinIDList[j], GoombaAIList[j].property.getPosition().x, GoombaAIList[j].property.getPosition().y);
						DeleteGoombaAI(GoombaAITypeList[j], GoombaAIList[j].property.getPosition().x, GoombaAIList[j].property.getPosition().y);
						Sounds.PlaySound("Kick2");
					}
				}
			}
			MultiBrickCoin(BrickLoop.position.x, BrickLoop.position.y, i);
			if (BrickAttList[i] == NORMAL && PowerState == 0) {
				BrickState[i] = true;
				UpDown[i] = false;
				BrickStateCount[i] = 0;
				Sounds.PlaySound("Bump");
				break;
			}
			else if (BrickAttList[i] == NORMAL && PowerState > 0) {
				Sounds.PlaySound("Break");
				AddBrickParticle(BrickIDList[i], Bricks[i].property.getPosition().x, Bricks[i].property.getPosition().y);
				DeleteBrick(Bricks[i].property.getPosition().x, Bricks[i].property.getPosition().y);
				Score += 50;
			}
		}
	}
}
void DeleteBrick(float x, float y) {
	for (int i = 0; i < Bricks.size(); i++) {
		if (Bricks[i].property.getPosition().x == x && Bricks[i].property.getPosition().y == y) {
			Bricks.erase(Bricks.begin() + i);
			BrickAttList.erase(BrickAttList.begin() + i);
			BrickIDList.erase(BrickIDList.begin() + i);
			BrickState.erase(BrickState.begin() + i);
			BrickStateCount.erase(BrickStateCount.begin() + i);
			BrickSaveList.erase(BrickSaveList.begin() + i);
			UpDown.erase(UpDown.begin() + i);
			BrickClock.erase(BrickClock.begin() + i);
			BrickHitted.erase(BrickHitted.begin() + i);
			DisabledBrick.erase(DisabledBrick.begin() + i);
			break;
		}
	}
}
void DeleteAllBrick() {
	Bricks.clear();
	BrickAttList.clear();
	BrickIDList.clear();
	BrickState.clear();
	BrickStateCount.clear();
	BrickSaveList.clear();
	BrickHitted.clear();
	UpDown.clear();
	BrickClock.clear();
	BrickHitted.clear();
	DisabledBrick.clear();
}