#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Block/Brick.hpp"
#include "Block/Obstacles.hpp"
#include "Core/WindowFrame.hpp"
#include "Object/Coin.hpp"
#include "Core/Scroll.hpp"
#include "Effect/CoinEffect.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/Loading/Loading.hpp"
#include "Object/Mario.hpp"
#include "Effect/BrickParticle.hpp"
#include "Object/GoombaAI.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Sound.hpp"
#include "Effect/GoombaAIEffect.hpp"
#include "Core/TextureManager.hpp"
#include "Core/Interpolation.hpp"
#include <vector>

std::vector<Obstacles> Bricks;
std::vector<std::pair<sf::FloatRect, sf::Vector2f>> BricksVertPosList;
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
void LoadBricks() {
	// Loading Texture
	BrickTextureManager.Loadingtexture("data/resources/Brick.png", "Bricks", 0, 0, 64, 64);
}
void SetPrevBricksPos() {
	for (int i = 0; i < Bricks.size(); i++) {
		Bricks[i].prev = Bricks[i].curr;
	}
}
void InterpolateBricksPos(const float alpha) {
	for (int i = 0; i < Bricks.size(); i++) {
		Bricks[i].property.setPosition(linearInterpolation(Bricks[i].prev, Bricks[i].curr, alpha));
	}
}
void AddBrick(const BrickID ID, const BrickAtt att, const float x, const float y) {
	switch (ID) {
	case BRICK_GRAY:
		Bricks.push_back(Obstacles{ 0, sf::Sprite(*BrickTextureManager.GetTexture("Bricks"), sf::IntRect({32, 0}, {32, 32})) });
		break;
	case BRICK_NORMAL:
		Bricks.push_back(Obstacles{ 0, sf::Sprite(*BrickTextureManager.GetTexture("Bricks"), sf::IntRect({0, 0}, {32, 32})) });
		break;
	}
	BrickAttList.push_back(att);
	BrickIDList.push_back(ID);
	BrickState.push_back(false);
	BrickStateCount.push_back(0);
	BrickSaveList.push_back({ x, y });
	UpDown.push_back(false);
	Bricks.back().property.setPosition({ x, y });
	Bricks.back().curr = Bricks.back().prev = Bricks.back().property.getPosition();
	setHitbox(Bricks[Bricks.size() - 1].hitbox, sf::FloatRect({ 0.f, 0.f }, { 32.f, 32.f }));
	//multicoin attribute
	BrickClock.push_back(sf::Clock());
	BrickHitted.push_back(false);
	DisabledBrick.push_back(false);
	BricksVertPosList.push_back({ sf::FloatRect({ 0.0f, 0.0f }, { 32.f, 32.f }), sf::Vector2f(x, y) });
}
void BricksSort() {
	if (Bricks.size() == 0) return;
	std::ranges::sort(BricksVertPosList, [](const std::pair<sf::FloatRect, sf::Vector2f>& A, const std::pair<sf::FloatRect, sf::Vector2f>& B) {
		if (A.second.y < B.second.y) return true;
		else if (A.second.y == B.second.y) return A.second.x < B.second.x;
		else return false;
		});
}
inline BrickID GetIDBrick(const float x, const float y) {
	for (int i = 0; i < Bricks.size(); i++) {
		if (Bricks[i].property.getPosition().x == x && Bricks[i].property.getPosition().y == y) {
			return BrickIDList[i];
		}
	}
	return BRICK_NORMAL;
}

inline BrickAtt GetBrickAtt(const float x, const float y) {
	for (int i = 0; i < Bricks.size(); i++) {
		if (Bricks[i].property.getPosition().x == x && Bricks[i].property.getPosition().y == y) {
			return BrickAttList[i];
		}
	}
	return NORMAL;
}
inline void BrickStatusUpdate() {
	if (Bricks.size() == 0) return;
	for (int i = 0; i < Bricks.size(); ++i) {
		if (DisabledBrick[i] && BrickAttList[i] == MULTICOIN) {
			if (BrickIDList[i] == BRICK_GRAY) Bricks[i].property.setTextureRect(sf::IntRect({ 32, 32 }, { 32, 32 }));
			else if (BrickIDList[i] == BRICK_NORMAL) Bricks[i].property.setTextureRect(sf::IntRect({ 0, 32 }, { 32, 32 }));
		}
	}
}
inline void BrickDraw() {
	for (auto & Brick : Bricks) {
		if (!isOutScreen(Brick.property.getPosition().x, Brick.property.getPosition().y, 32, 32)) {
			rObject.draw(Brick.property);
		}
	}
}
inline void BrickUpdate(const float deltaTime) {
	for (int i = 0; i < Bricks.size(); i++) {
		if (BrickState[i]) {
			if (!UpDown[i]) {
				if (BrickStateCount[i] < 11.0f) {
					Bricks[i].curr = { Bricks[i].curr.x, Bricks[i].curr.y - (BrickStateCount[i] < 6.0f ? 3.0f : (BrickStateCount[i] < 10.0f ? 2.0f : 1.0f)) * deltaTime };
					BrickStateCount[i] += (BrickStateCount[i] < 6.0f ? 3.0f : (BrickStateCount[i] < 10.0f ? 2.0f : 1.0f)) * deltaTime;
				}
				else {
					BrickStateCount[i] = 11.0f;
					UpDown[i] = true;
				}
			}
			else {
				if (BrickStateCount[i] > 0.0f) {
					Bricks[i].curr = { Bricks[i].curr.x, Bricks[i].curr.y + (BrickStateCount[i] > 10.0f ? 1.0f : (BrickStateCount[i] > 6.0f ? 2.0f : 3.0f)) * deltaTime };
					BrickStateCount[i] -= (BrickStateCount[i] > 10.0f ? 1.0f : (BrickStateCount[i] > 6.0f ? 2.0f : 3.0f)) * deltaTime;
				}
				else {
					Bricks[i].curr = { BrickSaveList[i].first, BrickSaveList[i].second };
					BrickStateCount[i] = 0.0f;
					UpDown[i] = false;
					BrickState[i] = false;
				}
			}
		}
	}
}
int getBrickIndex(const float x, const float y) {
	for (int i = 0; i < Bricks.size(); i++) {
		if (BrickSaveList[i].first == x && BrickSaveList[i].second == y) {
			return i;
		}
	}
	return 0;
}
void MultiBrickCoin(const float x, const float y, const int i) {
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
		AddCoinEffect(COIN_NORMAL, ONE_COIN, x + 15.0f, y + 32.0f);
		++CoinCount;
		BrickState[i] = true;
		UpDown[i] = false;
		BrickStateCount[i] = 0;
	}
}
void HitEvent(const float x, const float y) {
	for (int i = 0; i < Bricks.size(); i++) {
		if (Bricks[i].property.getPosition().x == x && Bricks[i].property.getPosition().y == y && !BrickState[i]) {
			sf::FloatRect BrickLoop = getGlobalHitbox(Bricks[i].hitbox, Bricks[i].property);
			BrickLoop.position.y -= 32.0f;
			for (int j = 0; j < CoinList.size(); ++j) {
				if (isCollide(CoinList[j].hitbox, CoinList[j].property, BrickLoop)) {
					AddCoinEffect(CoinIDList[j], CoinAttList[j], CoinList[j].property.getPosition().x + 15.0f, CoinList[j].property.getPosition().y + 32.0f);
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
						DeleteGoombaAI(GoombaAITypeList[j], GoombaAIList[j].curr.x, GoombaAIList[j].curr.y);
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
void DeleteBrick(const float x, const float y) {
	for (int i = 0; i < BricksVertPosList.size(); ++i) {
		if (BricksVertPosList[i].second.x == x && BricksVertPosList[i].second.y == y) {
			BricksVertPosList.erase(BricksVertPosList.begin() + i);
			break;
		}
	}
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
	BricksVertPosList.clear();
}