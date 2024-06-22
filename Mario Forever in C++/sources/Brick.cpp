#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../headers/Brick.hpp"
#include "../headers/Obstacles.hpp"
#include "../headers/WindowFrame.hpp"

#include <vector>
#include <iostream>

std::vector<Obstacles> Bricks;
std::vector<bool> BrickState;
std::vector<float> BrickStateCount;
std::vector<bool> UpDown;
std::vector<std::pair<float, float>> BrickSaveList;

sf::SoundBuffer BrickSoundBuffer;
sf::Sound BrickSound;
sf::Texture BrickTexture;
int LoadBricks() {
	if (!BrickTexture.loadFromFile("data/resources/brick.png")) {
		std::cout << "Failed to load brick.png" << std::endl;
	}
	if (!BrickSoundBuffer.loadFromFile("data/sounds/bump.wav")) {
		std::cout << "Cannot load data/sounds/bump.wav" << "\n";
	}
	BrickSound.setBuffer(BrickSoundBuffer);
	return 6;
}
int BrickInit = LoadBricks();
void AddBrick(float x, float y) {
	Bricks.push_back(Obstacles{ 0, sf::Sprite(BrickTexture, sf::IntRect(0, 0, 32, 32)) });
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
					Bricks[i].property.move(0, -2.5f * deltaTime);
					BrickStateCount[i] += 2.5f * deltaTime;
				}
				else {
					BrickStateCount[i] = 11.0f;
					UpDown[i] = true;
				}
			}
			else {
				if (BrickStateCount[i] > 0.0f) {
					Bricks[i].property.move(0, 2.5f * deltaTime);
					BrickStateCount[i] -= 2.5f * deltaTime;
				}
				else {
					Bricks[i].property.setPosition(BrickSaveList[i].first, BrickSaveList[i].second);
					BrickStateCount[i] = 0.0f;
					UpDown[i] = false;
					BrickState[i] = false;
				}
			}
		}

		window.draw(Bricks[i].property);
	}
}
void HitEvent(float x, float y) {
	for (int i = 0; i < Bricks.size(); i++) {
		if (Bricks[i].property.getPosition().x == x && Bricks[i].property.getPosition().y == y && !BrickState[i]) {
			BrickState[i] = true;
			UpDown[i] = false;
			BrickStateCount[i] = 0;
			BrickSound.play();
			break;
		}
	}
}