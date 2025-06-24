#include <SFML/Graphics.hpp>
#include <vector>
#include <array>

#include "Core/Loading/enum.hpp"
#include "Effect/BrickParticle.hpp"
#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Interpolation.hpp"

std::vector<std::array<sf::Sprite, 4>> BrickParticleList;
std::vector<std::array<sf::Vector2f, 4>> BrickParticleCurr;
std::vector<std::array<sf::Vector2f, 4>> BrickParticlePrev;
std::vector<std::array<bool, 4>> BrickParticleDisabledList;
std::vector<BrickID> BrickParticleID;
std::vector<std::array<std::pair<float, float>, 4>> BrickParticleVelo;
std::vector<int> BrickParticleDisabledAM;

void BrickParticleInit() {
	ImageManager::AddImage("BrickParticleImage", "data/resources/BrickParticle.png");
	ImageManager::AddTexture("BrickParticleImage", sf::IntRect({0, 0}, {16, 16}), "NormalBrickParticle");
	ImageManager::AddTexture("BrickParticleImage", sf::IntRect({16, 0}, {16, 16}), "GrayBrickParticle");
}
void SetPrevBrickParticlePos() {
	for (int i = 0; i < BrickParticleList.size(); ++i) {
		for (int j = 0; j < BrickParticleList[i].size(); ++j) {
			BrickParticlePrev[i][j] = BrickParticleCurr[i][j];
		}
	}
}
void InterpolateBrickParticlePos(const float alpha) {
	for (int i = 0; i < BrickParticleList.size(); ++i) {
		for (int j = 0; j < BrickParticleList[i].size(); ++j) {
			BrickParticleList[i][j].setPosition(linearInterpolation(BrickParticlePrev[i][j], BrickParticleCurr[i][j], alpha));
		}
	}
}
void AddBrickParticle(const BrickID id, const float ori_x, const float ori_y) {
	std::array<sf::Sprite, 4> it = { sf::Sprite(tempTex), sf::Sprite(tempTex), sf::Sprite(tempTex), sf::Sprite(tempTex) };
	std::array<sf::Vector2f, 4> itc;
	std::array<bool, 4> itn{};
	std::array<std::pair<float, float>, 4> veloIt;
	for (int i = 0; i < 4; ++i) {
		switch (id) {
		case BRICK_NORMAL:
			it[i].setTexture(ImageManager::GetTexture("NormalBrickParticle"), true);
			break;
		case BRICK_GRAY:
			it[i].setTexture(ImageManager::GetTexture("GrayBrickParticle"), true);
			break;
		}
		it[i].setOrigin({ 8, 8 });
		if (i == 0) {
			it[i].setPosition({ ori_x + 8.0f, ori_y + 8.0f });
			itc[i] = { ori_x + 8.0f, ori_y + 8.0f };
		}
		else if (i == 1) {
			it[i].setPosition({ ori_x + 8.0f + 16.0f, ori_y + 8.0f });
			itc[i] = { ori_x + 8.0f + 16.0f, ori_y + 8.0f };
		}
		else if (i == 2) {
			it[i].setPosition({ ori_x + 8.0f, ori_y + 8.0f + 16.0f });
			itc[i] = { ori_x + 8.0f, ori_y + 8.0f + 16.0f };
		}
		else if (i == 3) {
			it[i].setPosition({ ori_x + 8.0f + 16.0f, ori_y + 8.0f + 16.0f });
			itc[i] = { ori_x + 8.0f + 16.0f, ori_y + 8.0f + 16.0f };
		}
		itn[i] = false;
		if (i == 0) veloIt[i] = std::make_pair(-2.0f, -8.0f);
		else if (i == 1) veloIt[i] = std::make_pair(2.0f, -8.0f);
		else if (i == 2) veloIt[i] = std::make_pair(-4.0f, -7.0f);
		else if (i == 3) veloIt[i] = std::make_pair(4.0f, -7.0f);
	}
	BrickParticleCurr.push_back(itc);
	BrickParticlePrev.push_back(itc);
	BrickParticleDisabledList.push_back(itn);
	BrickParticleList.push_back(it);
	BrickParticleVelo.push_back(veloIt);
	BrickParticleID.push_back(id);
	BrickParticleDisabledAM.push_back(0);
}
void DeleteSubBrickParticle(const float x, const float y) {
	bool Itbreak = false;
	for (int i = 0; i < BrickParticleList.size(); ++i) {
		for (int j = 0; j < BrickParticleList[i].size(); ++j) {
			if (BrickParticleCurr[i][j].x == x && BrickParticleCurr[i][j].y == y && !BrickParticleDisabledList[i][j]) {
				BrickParticleDisabledList[i][j] = true;
				Itbreak = true;
				break;
			}
		}
		if (Itbreak) break;
	}
}
void DeleteSubBrickParticleIndex(const int i, const int j) {
	if (!BrickParticleDisabledList[i][j]) {
		++BrickParticleDisabledAM[i];
		BrickParticleDisabledList[i][j] = true;
	}
}
void BrickParticleStatusUpdate(const float deltaTime) {
	if (BrickParticleList.empty()) return;
	bool canDelete = false;
	for (int i = 0; i < BrickParticleList.size(); ++i) {
		for (int j = 0; j < BrickParticleList[i].size(); ++j) {
			if (!BrickParticleDisabledList[i][j]) {
				BrickParticleVelo[i][j].second += 0.5f * deltaTime * 0.3f;
				BrickParticleCurr[i][j] = { BrickParticleCurr[i][j].x + BrickParticleVelo[i][j].first * deltaTime, BrickParticleCurr[i][j].y + BrickParticleVelo[i][j].second * deltaTime };
				// TODO: Add imterpolation to rotate
				if (BrickParticleVelo[i][j].first > 0) BrickParticleList[i][j].rotate(sf::degrees(10.0f * deltaTime));
				else if (BrickParticleVelo[i][j].first < 0) BrickParticleList[i][j].rotate(sf::degrees(-10.0f * deltaTime));
				BrickParticleVelo[i][j].second += 0.5f * deltaTime * 0.3f;
			}
		}
		while (true) {
			canDelete = false;
			for (int j = 0; j < BrickParticleList[i].size(); ++j) {
				if (isOutScreen(BrickParticleCurr[i][j].x, BrickParticleCurr[i][j].y, 32 + 16, 32 + 16) && !BrickParticleDisabledList[i][j]) {
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
				BrickParticleCurr.erase(BrickParticleCurr.begin() + i);
				BrickParticlePrev.erase(BrickParticlePrev.begin() + i);
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
	BrickParticleCurr.clear();
	BrickParticlePrev.clear();
}
void BrickParticleDraw() {
	if (BrickParticleList.empty()) return;
	for (int i = 0; i < BrickParticleList.size(); ++i) {
		for (int j = 0; j < BrickParticleList[i].size(); ++j) {
			if (!BrickParticleDisabledList[i][j]) window.draw(BrickParticleList[i][j]);
		}
	}
}