#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../headers/Brick.hpp"
#include "../headers/Obstacles.hpp"
#include "../headers/WindowFrame.hpp"
#include "../headers/Coin.hpp"
#include "../headers/Scroll.hpp"
#include "../headers/CoinEffect.hpp"
#include "../headers/enum.hpp"
#include "../headers/Loading.hpp"
#include "../headers/Mario.hpp"
#include "../headers/BrickParticle.hpp"
#include "../headers/GoombaAI.hpp"
#include "../headers/ScoreEffect.hpp"

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

sf::SoundBuffer BrickSoundBuffer;
sf::SoundBuffer BirckBreakSoundBuffer;
sf::Sound BrickSound;
sf::Sound BrickBreakSound;
sf::Texture BrickTexture;
int LoadBricks() {
	LoadTexture(BrickTexture, BRICK_TEXTURE, IMAGEFILE);
	LoadAudio(BrickSoundBuffer, BUMP_SOUND, SOUNDFILE);
	BrickSound.setBuffer(BrickSoundBuffer);
	LoadAudio(BirckBreakSoundBuffer, BREAK_SOUND, SOUNDFILE);
	BrickBreakSound.setBuffer(BirckBreakSoundBuffer);
	return 6;
}
int BrickInit = LoadBricks();
void AddBrick(BrickID ID, BrickAtt att, float x, float y) {
	if (ID == BRICK_GRAY) Bricks.push_back(Obstacles{ 0, sf::Sprite(BrickTexture, sf::IntRect(32, 0, 32, 32)) });
	else if (ID == BRICK_NORMAL) Bricks.push_back(Obstacles{ 0, sf::Sprite(BrickTexture, sf::IntRect(0, 0, 32, 32)) });
	BrickAttList.push_back(att);
	BrickIDList.push_back(ID);
	BrickState.push_back(false);
	BrickStateCount.push_back(0);
	BrickSaveList.push_back({ x, y });
	UpDown.push_back(false);
	Bricks[Bricks.size() - 1].property.setPosition({ x, y });
	Bricks[Bricks.size() - 1].setHitbox({ 0.f, 0.f, 32.f, 32.f });
	//multicoin attribute
	BrickClock.push_back(sf::Clock());
	BrickHitted.push_back(false);
	DisabledBrick.push_back(false);
}
inline void BrickStatusUpdate() {
	for (int i = 0; i < Bricks.size(); ++i) {
		if (DisabledBrick[i] && BrickAttList[i] == MULTICOIN) {
			if (BrickIDList[i] == BRICK_GRAY) Bricks[i].property.setTextureRect(sf::IntRect(32, 32, 32, 32));
			else if (BrickIDList[i] == BRICK_NORMAL) Bricks[i].property.setTextureRect(sf::IntRect(0, 32, 32, 32));
		}
	}
}
inline void BrickUpdate() {
	for (int i = 0; i < Bricks.size(); i++) {
		if (BrickState[i]) {
			if (!UpDown[i]) {
				if (BrickStateCount[i] < 11.0f) {
					Bricks[i].property.move(0, 0 - (BrickStateCount[i] < 6.0f ? 3.0f : (BrickStateCount[i] < 10.0f ? 2.0f : 1.0f)) * deltaTime);
					BrickStateCount[i] += (BrickStateCount[i] < 6.0f ? 3.0f : (BrickStateCount[i] < 10.0f ? 2.0f : 1.0f)) * deltaTime;
				}
				else {
					BrickStateCount[i] = 11.0f;
					UpDown[i] = true;
				}
			}
			else {
				if (BrickStateCount[i] > 0.0f) {
					Bricks[i].property.move(0, (BrickStateCount[i] > 10.0f ? 1.0f : (BrickStateCount[i] > 6.0f ? 2.0f : 3.0f)) * deltaTime);
					BrickStateCount[i] -= (BrickStateCount[i] > 10.0f ? 1.0f : (BrickStateCount[i] > 6.0f ? 2.0f : 3.0f)) * deltaTime;
				}
				else {
					Bricks[i].property.setPosition(BrickSaveList[i].first, BrickSaveList[i].second);
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
void HitEvent(float x, float y) {
	sf::FloatRect BrickLoop;
	for (int i = 0; i < Bricks.size(); i++) {
		if (Bricks[i].property.getPosition().x == x && Bricks[i].property.getPosition().y == y && !BrickState[i]) {
			BrickLoop = Bricks[i].getGlobalHitbox();
			BrickLoop.top -= 32.0f;
			for (int j = 0; j < CoinList.size(); ++j) {
				if (CoinList[j].isCollide(BrickLoop)) {
					AddCoinEffect(CoinIDList[j], CoinAttList[j], CoinList[j].property.getPosition().x - 3, CoinList[j].property.getPosition().y);
					DeleteCoin(CoinList[j].property.getPosition().x, CoinList[j].property.getPosition().y);
					CoinSound.play();
					++CoinCount;
				}
			}
			for (int j = 0; j < GoombaAIList.size(); ++j) {
				if (GoombaAIList[j].isCollideMain(BrickLoop)) {
					if (GoombaAITypeList[j] == GOOMBA) AddScoreEffect(SCORE_100, GoombaAIList[j].property.getPosition().x - 15.0f, GoombaAIList[j].property.getPosition().y - GoombaAIHitboxList[0].second);
					DeleteGoombaAI(GoombaAITypeList[j], GoombaAIList[j].property.getPosition().x, GoombaAIList[j].property.getPosition().y);
					Kick2Sound.play();
				}
			}
			if (BrickAttList[i] == MULTICOIN && !DisabledBrick[i]) {
				if (!BrickHitted[i]) {
					BrickHitted[i] = true;
					BrickClock[i].restart().asSeconds();
				}
				else {
					if (BrickClock[i].getElapsedTime().asSeconds() > 6.0f && BrickAttList[i] == MULTICOIN) {
						DisabledBrick[i] = true;
						BrickState[i] = true;
						UpDown[i] = false;
						BrickStateCount[i] = 0;
					}
				}
				CoinSound.play();
				AddCoinEffect(COIN_NORMAL, ONE_COIN, BrickLoop.getPosition().x - 3, BrickLoop.getPosition().y);
				++CoinCount;
			}
			if ((BrickAttList[i] == NORMAL && PowerState == 0) || (BrickAttList[i] == MULTICOIN && !DisabledBrick[i])) {
				BrickState[i] = true;
				UpDown[i] = false;
				BrickStateCount[i] = 0;
				if (BrickAttList[i] != MULTICOIN) BrickSound.play();
				break;
			}
			else if (BrickAttList[i] == NORMAL && PowerState > 0) {
				BrickBreakSound.play();
				AddBrickParticle(BrickIDList[i], Bricks[i].property.getPosition().x, Bricks[i].property.getPosition().y);
				deleteBrick(Bricks[i].property.getPosition().x, Bricks[i].property.getPosition().y);
				Score += 50;
			}
		}
	}
}
void deleteBrick(float x, float y) {
	for (int i = 0; i < Bricks.size(); i++) {
		if (Bricks[i].property.getPosition().x == x && Bricks[i].property.getPosition().y == y) {
			Bricks.erase(Bricks.begin() + i);
			BrickAttList.erase(BrickAttList.begin() + i);
			BrickIDList.erase(BrickIDList.begin() + i);
			BrickState.erase(BrickState.begin() + i);
			BrickStateCount.erase(BrickStateCount.begin() + i);
			BrickSaveList.erase(BrickSaveList.begin() + i);
			UpDown.erase(UpDown.begin() + i);
			break;
		}
	}
}