#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "../headers/enum.hpp"
#include "../headers/BrickParticle.hpp"
#include "../headers/Loading.hpp"
#include "../headers/Scroll.hpp"
#include "../headers/WindowFrame.hpp"

#include "../resource.h"

std::vector<sf::Sprite> BrickParticleList;
std::vector<BrickID> BrickParticleID;
std::vector<BrickParticleDirection> BrickParticleDirectionList;
std::vector<std::pair<float, float>> BrickParticleVelo;
std::vector<float> BrickParticleClock;
sf::Clock BrickParticleTimer;
sf::Texture BrickParticleTexture;

int BrickParticleInit() {
	LoadTexture(BrickParticleTexture, BRICKPARTICLE_TEXTURE, IMAGEFILE);
	BrickParticleTimer.restart().asSeconds();
	return 6;
}
int iniBrickParticle = BrickParticleInit();
void AddBrickParticle(BrickID id, BrickParticleDirection direction, float x, float y, float vx, float vy) {
	sf::Sprite Init;
	Init.setTexture(BrickParticleTexture);
	if (id == BRICK_NORMAL) Init.setTextureRect(sf::IntRect(0, 0, 16, 16));
	else if (id == BRICK_GRAY) Init.setTextureRect(sf::IntRect(16, 0, 16, 16));
	Init.setPosition(x + 8, y + 8);
	Init.setOrigin(8, 8);
	BrickParticleList.push_back(Init);
	BrickParticleID.push_back(id);
	BrickParticleDirectionList.push_back(direction);
	BrickParticleVelo.push_back(std::make_pair(vx, vy));
	BrickParticleClock.push_back(0.0f);
}
void DeleteBrickParticle(float x, float y) {
	for (int i = 0; i < BrickParticleList.size(); ++i) {
		if (BrickParticleList[i].getPosition().x == x && BrickParticleList[i].getPosition().y == y) {
			BrickParticleList.erase(BrickParticleList.begin() + i);
			BrickParticleID.erase(BrickParticleID.begin() + i);
			BrickParticleDirectionList.erase(BrickParticleDirectionList.begin() + i);
			BrickParticleVelo.erase(BrickParticleVelo.begin() + i);
			BrickParticleClock.erase(BrickParticleClock.begin() + i);
			break;
		}
	}
}
void BrickParticleStatusUpdate() {
	//std::cout << BrickParticleTimer.getElapsedTime().asMilliseconds() << "\n";
	for (int i = 0; i < BrickParticleList.size(); ++i) {
		if (isOutScreen(BrickParticleList[i].getPosition().x, BrickParticleList[i].getPosition().y, 32, 32)) DeleteBrickParticle(BrickParticleList[i].getPosition().x, BrickParticleList[i].getPosition().y);
		BrickParticleList[i].move(BrickParticleVelo[i].first * deltaTime, BrickParticleVelo[i].second * deltaTime);
		if (BrickParticleDirectionList[i] == RIGHT) BrickParticleList[i].rotate(10.0f * deltaTime);
		else if (BrickParticleDirectionList[i] == LEFT) BrickParticleList[i].rotate(-10.0f * deltaTime);
		if (BrickParticleClock[i] > 17.0f * deltaTime) {
			BrickParticleClock[i] = 0.0f;
			BrickParticleVelo[i].second += 1.0f * deltaTime;
		}
		else BrickParticleClock[i] += std::max(1, BrickParticleTimer.getElapsedTime().asMilliseconds());
	}
	BrickParticleTimer.restart().asMilliseconds();
}
void BrickParticleUpdate() {
	for (int i = 0; i < BrickParticleList.size(); ++i) {
		window.draw(BrickParticleList[i]);
	}
}