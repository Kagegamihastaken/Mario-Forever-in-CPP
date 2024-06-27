#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "../headers/enum.hpp"
#include "../headers/BrickParticle.hpp"
#include "../headers/Loading.hpp"
#include "../headers/Scroll.hpp"
#include "../headers/WindowFrame.hpp"

#include "../resource.h"

std::vector<std::vector<sf::Sprite>> BrickParticleList;
std::vector<BrickID> BrickParticleID;
std::vector<std::vector<std::pair<float, float>>> BrickParticleVelo;
std::vector<sf::Clock> BrickParticleClock;
sf::Texture BrickParticleTexture;

int BrickParticleInit() {
	LoadTexture(BrickParticleTexture, BRICKPARTICLE_TEXTURE, IMAGEFILE);
	return 6;
}
int iniBrickParticle = BrickParticleInit();
void AddBrickParticle(BrickID id, float ori_x, float ori_y) {
	std::vector<sf::Sprite> it;
	std::vector<std::pair<float, float>> veloIt;
	sf::Sprite Init;
	for (int i = 0; i < 4; ++i) {
		Init.setTexture(BrickParticleTexture);
		if (id == BRICK_NORMAL) Init.setTextureRect(sf::IntRect(0, 0, 16, 16));
		else if (id == BRICK_GRAY) Init.setTextureRect(sf::IntRect(16, 0, 16, 16));
		Init.setOrigin(8, 8);
		if (i == 0) Init.setPosition(ori_x + 8.0f, ori_y + 8.0f);
		else if (i == 1) Init.setPosition(ori_x + 8.0f + 16.0f, ori_y + 8.0f);
		else if (i == 2) Init.setPosition(ori_x + 8.0f, ori_y + 8.0f + 16.0f);
		else if (i == 3) Init.setPosition(ori_x + 8.0f + 16.0f, ori_y + 8.0f + 16.0f);
		it.push_back(Init);
		if (i == 0) veloIt.push_back(std::make_pair(-2, -8));
		else if (i == 1) veloIt.push_back(std::make_pair(2, -8));
		else if (i == 2) veloIt.push_back(std::make_pair(-4, -7));
		else if (i == 3) veloIt.push_back(std::make_pair(4, -7));
	}
	BrickParticleList.push_back(it);
	BrickParticleVelo.push_back(veloIt);
	BrickParticleID.push_back(id);
	BrickParticleClock.push_back(sf::Clock());
	BrickParticleClock[BrickParticleClock.size() - 1].restart().asMilliseconds();
}
void DeleteSubBrickParticle(float x, float y) {
	bool Itbreak = false;
	for (int i = 0; i < BrickParticleList.size(); ++i) {
		for (int j = 0; j < BrickParticleList[i].size(); ++j) {
			if (BrickParticleList[i][j].getPosition().x == x && BrickParticleList[i][j].getPosition().y == y) {
				BrickParticleList[i].erase(BrickParticleList[i].begin() + j);
				BrickParticleVelo[i].erase(BrickParticleVelo[i].begin() + j);
				if (BrickParticleList[i].size() == 0) {
					BrickParticleList.erase(BrickParticleList.begin() + i);
					BrickParticleVelo.erase(BrickParticleVelo.begin() + i);
					BrickParticleID.erase(BrickParticleID.begin() + i);
					BrickParticleClock.erase(BrickParticleClock.begin() + i);
				}
				Itbreak = true;
				break;
			}
		}
		if (Itbreak) break;
	}
}
void BrickParticleStatusUpdate() {
	bool reUpdate = false;
	//std::cout << BrickParticleTimer.getElapsedTime().asMilliseconds() << "\n";
	for (int i = 0; i < BrickParticleList.size(); ++i) {
		for (int j = 0; j < BrickParticleList[i].size(); ++j) {
			if (isOutScreen(BrickParticleList[i][j].getPosition().x, BrickParticleList[i][j].getPosition().y, 32, 32)) DeleteSubBrickParticle(BrickParticleList[i][j].getPosition().x, BrickParticleList[i][j].getPosition().y);
			if (BrickParticleList[i].size() > 0) {
				BrickParticleList[i][j].move(BrickParticleVelo[i][j].first * deltaTime, BrickParticleVelo[i][j].second * deltaTime);
				if (BrickParticleVelo[i][j].first > 0) BrickParticleList[i][j].rotate(10.0f * deltaTime);
				else if (BrickParticleVelo[i][j].first < 0) BrickParticleList[i][j].rotate(-10.0f * deltaTime);
				if (BrickParticleClock[i].getElapsedTime().asMilliseconds() > 25.0f * deltaTime) {
					BrickParticleVelo[i][j].second += 1.0f * deltaTime;
					if (!reUpdate) reUpdate = true;
				}
			}
		}
		if (reUpdate && BrickParticleList[i].size() > 0) {
			BrickParticleClock[i].restart().asMilliseconds();
			reUpdate = false;
		}
	}
}
void BrickParticleUpdate() {
	for (int i = 0; i < BrickParticleList.size(); ++i) {
		for (int j = 0; j < BrickParticleList[i].size(); ++j) {
			window.draw(BrickParticleList[i][j]);
		}
	}
}