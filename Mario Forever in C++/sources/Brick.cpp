#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../headers/Brick.hpp"
#include "../headers/Obstacles.hpp"
#include "../headers/WindowFrame.hpp"
#include "../headers/Coin.hpp"
#include "../headers/Scroll.hpp"
#include "../headers/CoinEffect.hpp"
#include "../headers/enum.hpp"

#include <vector>
#include <iostream>

std::vector<Obstacles> Bricks;
std::vector<bool> BrickState;
std::vector<float> BrickStateCount;
std::vector<bool> UpDown;
std::vector<std::pair<float, float>> BrickSaveList;
std::vector<BrickID> BrickIDList;

sf::SoundBuffer BrickSoundBuffer;
sf::Sound BrickSound;
sf::Texture BrickTexture;
int LoadBricks() {
	if (!BrickTexture.loadFromFile("data/resources/brick.png")) {
		std::cout << "Failed to load brick.png" << "\n";
	}
	if (!BrickSoundBuffer.loadFromFile("data/sounds/bump.wav")) {
		std::cout << "Cannot load data/sounds/bump.wav" << "\n";
	}
	BrickSound.setBuffer(BrickSoundBuffer);
	return 6;
}
int BrickInit = LoadBricks();
void AddBrick(BrickID ID, float x, float y) {
	if (ID == BRICK_GRAY) Bricks.push_back(Obstacles{ 0, sf::Sprite(BrickTexture, sf::IntRect(32, 0, 32, 32)) });
	else if (ID == BRICK_NORMAL) Bricks.push_back(Obstacles{ 0, sf::Sprite(BrickTexture, sf::IntRect(0, 0, 32, 32)) });
	BrickIDList.push_back(ID);
	BrickState.push_back(false);
	BrickStateCount.push_back(0);
	BrickSaveList.push_back({ x, y });
	UpDown.push_back(false);
	Bricks[Bricks.size() - 1].property.setPosition({ x, y });
	Bricks[Bricks.size() - 1].setHitbox({ 0.f, 0.f, 32.f, 32.f });
}
void BrickUpdate() {
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
					AddCoinEffect(CoinIDList[j], CoinList[j].property.getPosition().x - 3, CoinList[j].property.getPosition().y);
					DeleteCoin(CoinList[j].property.getPosition().x, CoinList[j].property.getPosition().y);
					CoinSound.play();
					++CoinCount;
				}
			}
			BrickState[i] = true;
			UpDown[i] = false;
			BrickStateCount[i] = 0;
			BrickSound.play();
			break;
		}
	}
}
void deleteBrick(float x, float y) {
	for (int i = 0; i < Bricks.size(); i++) {
		if (Bricks[i].property.getPosition().x == x && Bricks[i].property.getPosition().y == y) {
			Bricks.erase(Bricks.begin() + i);
			BrickIDList.erase(BrickIDList.begin() + i);
			BrickState.erase(BrickState.begin() + i);
			BrickStateCount.erase(BrickStateCount.begin() + i);
			BrickSaveList.erase(BrickSaveList.begin() + i);
			UpDown.erase(UpDown.begin() + i);
			break;
		}
	}
}