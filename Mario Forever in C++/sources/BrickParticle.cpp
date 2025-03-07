#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <array>

#include "../headers/Core/Loading/enum.hpp"
#include "../headers/Effect/BrickParticle.hpp"
#include "../headers/Core/Loading/Loading.hpp"
#include "../headers/Core/Scroll.hpp"
#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Core/TextureManager.hpp"

#include "../resource.h"

std::vector<std::array<sf::Sprite, 4>> BrickParticleList;
std::vector<std::array<bool, 4>> BrickParticleDisabledList;
std::vector<BrickID> BrickParticleID;
std::vector<std::array<std::pair<float, float>, 4>> BrickParticleVelo;
TextureManager BrickParticleTexture;
std::vector<int> BrickParticleDisabledAM;

void BrickParticleInit() {
	BrickParticleTexture.Loadingtexture(BRICKPARTICLE_TEXTURE, "BrickParticleNormal", 0, 0, 16, 16);
	BrickParticleTexture.Loadingtexture(BRICKPARTICLE_TEXTURE, "BrickParticleGray", 16, 0, 16, 16);
}
void AddBrickParticle(BrickID id, float ori_x, float ori_y) {
	std::array<sf::Sprite, 4> it = { sf::Sprite(tempTex), sf::Sprite(tempTex), sf::Sprite(tempTex), sf::Sprite(tempTex) };
	std::array<bool, 4> itn;
	std::array<std::pair<float, float>, 4> veloIt;
	for (int i = 0; i < 4; ++i) {
		switch (id) {
		case BRICK_NORMAL:
			it[i].setTexture(*BrickParticleTexture.GetTexture("BrickParticleNormal"), true);
			break;
		case BRICK_GRAY:
			it[i].setTexture(*BrickParticleTexture.GetTexture("BrickParticleGray"), true);
			break;
		}
		it[i].setOrigin({ 8, 8 });
		if (i == 0) it[i].setPosition({ ori_x + 8.0f, ori_y + 8.0f });
		else if (i == 1) it[i].setPosition({ ori_x + 8.0f + 16.0f, ori_y + 8.0f });
		else if (i == 2) it[i].setPosition({ ori_x + 8.0f, ori_y + 8.0f + 16.0f });
		else if (i == 3) it[i].setPosition({ ori_x + 8.0f + 16.0f, ori_y + 8.0f + 16.0f });
		itn[i] = false;
		if (i == 0) veloIt[i] = std::make_pair(-2, -8);
		else if (i == 1) veloIt[i] = std::make_pair(2, -8);
		else if (i == 2) veloIt[i] = std::make_pair(-4, -7);
		else if (i == 3) veloIt[i] = std::make_pair(4, -7);
	}
	BrickParticleDisabledList.push_back(itn);
	BrickParticleList.push_back(it);
	BrickParticleVelo.push_back(veloIt);
	BrickParticleID.push_back(id);
	BrickParticleDisabledAM.push_back(0);
}
void DeleteSubBrickParticle(float x, float y) {
	bool Itbreak = false;
	for (int i = 0; i < BrickParticleList.size(); ++i) {
		for (int j = 0; j < BrickParticleList[i].size(); ++j) {
			if (BrickParticleList[i][j].getPosition().x == x && BrickParticleList[i][j].getPosition().y == y && !BrickParticleDisabledList[i][j]) {
				BrickParticleDisabledList[i][j] = true;
				Itbreak = true;
				break;
			}
		}
		if (Itbreak) break;
	}
}
void DeleteSubBrickParticleIndex(int i, int j) {
	if (!BrickParticleDisabledList[i][j]) {
		++BrickParticleDisabledAM[i];
		BrickParticleDisabledList[i][j] = true;
	}
}
void BrickParticleStatusUpdate() {
	//std::cout << BrickParticleList.size() << "\n";
	if (BrickParticleList.size() == 0) return;
	bool reUpdate = false, canDelete = false, AllTrue = false;
	//std::cout << BrickParticleTimer.getElapsedTime().asMilliseconds() << "\n";
	for (int i = 0; i < BrickParticleList.size(); ++i) {
		for (int j = 0; j < BrickParticleList[i].size(); ++j) {
			if (!BrickParticleDisabledList[i][j]) {
				BrickParticleVelo[i][j].second += 0.5f * deltaTime * 0.3f;
				BrickParticleList[i][j].move({ BrickParticleVelo[i][j].first * deltaTime, BrickParticleVelo[i][j].second * deltaTime });
				if (BrickParticleVelo[i][j].first > 0) BrickParticleList[i][j].rotate(sf::degrees(10.0f * deltaTime));
				else if (BrickParticleVelo[i][j].first < 0) BrickParticleList[i][j].rotate(sf::degrees(-10.0f * deltaTime));
				BrickParticleVelo[i][j].second += 0.5f * deltaTime * 0.3f;
			}
		}
		while (true) {
			canDelete = false;
			for (int j = 0; j < BrickParticleList[i].size(); ++j) {
				if (isOutScreen(BrickParticleList[i][j].getPosition().x, BrickParticleList[i][j].getPosition().y, 32 + 8, 32 + 8) && !BrickParticleDisabledList[i][j]) {
					DeleteSubBrickParticleIndex(i, j);
					//DeleteSubBrickParticle(BrickParticleList[i][j].getPosition().x, BrickParticleList[i][j].getPosition().y);
					canDelete = true;
					break;
				}
			}
			if (!canDelete) break;
		}
	}
	while (true) {
		canDelete = false;
		for (int i = 0; i < BrickParticleList.size(); ++i) {
			//AllTrue = true;
			//for (int j = 0; j < BrickParticleList[i].size(); ++j) {
			//	if (!BrickParticleDisabledList[i][j]) {
			//		AllTrue = false;
			//		break;
			//	}
			//}
			//if (AllTrue) {
			if (BrickParticleDisabledAM[i] >= 4) {
				BrickParticleList.erase(BrickParticleList.begin() + i);
				BrickParticleVelo.erase(BrickParticleVelo.begin() + i);
				BrickParticleDisabledList.erase(BrickParticleDisabledList.begin() + i);
				BrickParticleID.erase(BrickParticleID.begin() + i);
				BrickParticleDisabledAM.erase(BrickParticleDisabledAM.begin() + i);
				canDelete = true;
				break;
			}
		}
		if (!canDelete) break;
	}
}
void DeleteAllBrickParticle() {
	BrickParticleList.clear();
	BrickParticleVelo.clear();
	BrickParticleDisabledList.clear();
	BrickParticleID.clear();
	BrickParticleDisabledAM.clear();
}
void BrickParticleUpdate() {
	if (BrickParticleList.size() == 0) return;
	for (int i = 0; i < BrickParticleList.size(); ++i) {
		for (int j = 0; j < BrickParticleList[i].size(); ++j) {
			if (!BrickParticleDisabledList[i][j]) window.draw(BrickParticleList[i][j]);
		}
	}
}