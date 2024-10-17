#include "../headers/Obstacles.hpp"
#include "../headers/Level.hpp"
#include "../headers/Coin.hpp"
#include "../headers/Brick.hpp"
#include "../headers/LuckyBlock.hpp"
#include "../headers/enum.hpp"
#include "../headers/Loading.hpp"
#include "../headers/Collide.hpp"
#include "../headers/GoombaAI.hpp"
#include "../headers/BrickParticle.hpp"
#include "../headers/CoinEffect.hpp"
#include "../headers/GoombaAIEffect.hpp"
#include "../headers/ScoreEffect.hpp"
#include "../headers/Slopes.hpp"

#include "../resource.h"

#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <array>
// Level data
float LevelWidth, LevelHeight;
std::vector<std::array<float, 3>> LevelData;
std::vector<std::array<float, 3>> SlopeData;
std::vector<std::array<float, 5>> BonusData;
std::vector<std::array<float, 4>> EnemyData;
std::array<float, 2> PlayerData;
void ReadData(int IDLevel) {
	std::string lvldat;
	LoadLvl(lvldat, IDLevel);
	float value;
	std::string DataStructure;
	std::vector<float> temp;
	int tm = 0;
	std::string numLoop;
	// Read the file
	while (true) {
		if (DataStructure != "--Level Data--" && DataStructure != "--Tile Data--" && DataStructure != "--Bonus Data--" && DataStructure != "--Enemy Data--" && DataStructure != "--Slope Data--") {
			tm = ReadStrLine(lvldat, DataStructure, tm);
		}
		if (tm == -1) break;
		if (DataStructure == "--Level Data--") {
			int C;
			for (int i = 0; i < 2; ++i) {
				while (true) {
					tm = ReadStrLine(lvldat, DataStructure, tm);
					if (DataStructure == "") continue;
					C = 0;
					numLoop = "";
					for (const auto& j : DataStructure) {
						if (j != ' ') numLoop += j;
						else {
							if (i == 0) {
								if (C == 0) LevelWidth = std::stof(numLoop);
								else if (C == 1) LevelHeight = std::stof(numLoop);
							}
							else if (i == 1) {
								PlayerData[C] = std::stof(numLoop);
							}
							++C;
							numLoop = "";
						}
					}
					if (numLoop != "") {
						if (i == 0) {
							if (C == 0) LevelWidth = std::stof(numLoop);
							else if (C == 1) LevelHeight = std::stof(numLoop);
						}
						else if (i == 1) {
							PlayerData[C] = std::stof(numLoop);
						}
					}
					break;
				}
			}
		}
		if (DataStructure == "--Tile Data--") {
			while (true) {
				tm = ReadStrLine(lvldat, DataStructure, tm);
				if (DataStructure == "") continue;
				temp.clear();
				numLoop = "";
				//if said Bonus Data break it
				if (DataStructure == "--Bonus Data--" || DataStructure == "--Enemy Data--" || DataStructure == "--Slope Data--") break;
				for (const auto& i : DataStructure) {
					if (i != ' ') numLoop += i;
					else {
						if (numLoop != "") temp.push_back(std::stof(numLoop));
						numLoop = "";
					}
				}
				if (numLoop != "") temp.push_back(std::stof(numLoop));
				LevelData.push_back({ temp[0], temp[1], temp[2] });
				if (tm == -1) break;
			}
		}
		if (DataStructure == "--Bonus Data--") {
			while (true) {
				tm = ReadStrLine(lvldat, DataStructure, tm);
				if (DataStructure == "") continue;
				temp.clear();
				numLoop = "";
				//if said Enemies Data break it
				if (DataStructure == "--Tile Data--" || DataStructure == "--Enemy Data--" || DataStructure == "--Slope Data--") break;
				for (const auto& i : DataStructure) {
					if (i != ' ') numLoop += i;
					else {
						if (numLoop != "") temp.push_back(std::stof(numLoop));
						numLoop = "";
					}
				}
				if (numLoop != "") temp.push_back(std::stof(numLoop));
				BonusData.push_back({ temp[0], temp[1], temp[2], temp[3], temp[4] });
				if (tm == -1) break;
			}
		}
		if (DataStructure == "--Enemy Data--") {
			while (true) {
				tm = ReadStrLine(lvldat, DataStructure, tm);
				if (DataStructure == "") continue;
				temp.clear();
				numLoop = "";
				//if said Enemies Data break it
				if (DataStructure == "--Bonus Data--" || DataStructure == "--Tile Data--" || DataStructure == "--Slope Data--") break;
				for (const auto& i : DataStructure) {
					if (i != ' ') numLoop += i;
					else {
						if (numLoop != "") temp.push_back(std::stof(numLoop));
						numLoop = "";
					}
				}
				if (numLoop != "") temp.push_back(std::stof(numLoop));
				EnemyData.push_back({ temp[0], temp[1], temp[2], temp[3] });
				if (tm == -1) break;
			}
		}
		if (DataStructure == "--Slope Data--") {
			while (true) {
				tm = ReadStrLine(lvldat, DataStructure, tm);
				if (DataStructure == "") continue;
				temp.clear();
				numLoop = "";
				//if said Bonus Data break it
				if (DataStructure == "--Bonus Data--" || DataStructure == "--Enemy Data--" || DataStructure == "--Tile Data--") break;
				for (const auto& i : DataStructure) {
					if (i != ' ') numLoop += i;
					else {
						if (numLoop != "") temp.push_back(std::stof(numLoop));
						numLoop = "";
					}
				}
				if (numLoop != "") temp.push_back(std::stof(numLoop));
				SlopeData.push_back({ temp[0], temp[1], temp[2] });
				if (tm == -1) break;
			}
		}
		if (tm == -1) break;
	}
}
void Obstaclebuilding() {
	int posTextureIndex;
	for (const auto& i : LevelData) {
		// Find the tile id
		posTextureIndex = find_if(ID_list.begin(), ID_list.end(), [&i](const std::array<int, 3>& compare) {return compare[0] == int(i[0]); }) - (ID_list.begin());
		// Then use the index of tile id property to add it to the list
		ObstaclesList.push_back(Obstacles{ int(i[0]), sf::Sprite(*ObstaclesTextureList[i[0]]) });
		ObstaclesList[int(ObstaclesList.size()) - 1].property.setPosition({ i[1], i[2] });
		setHitbox(ObstaclesList[int(ObstaclesList.size()) - 1].hitbox, { 0.f, 0.f, 32.f, 32.f });
	}
}
void Slopebuilding() {
	int posTextureIndex;
	for (const auto& i : SlopeData) {
		// Find the tile id
		posTextureIndex = find_if(IDSlope_list.begin(), IDSlope_list.end(), [&i](const std::array<int, 3>& compare) {return compare[0] == int(i[0]); }) - (IDSlope_list.begin());
		// Then use the index of tile id property to add it to the list
		SlopesList.push_back(Obstacles{ int(i[0]), sf::Sprite(SlopeTexture, sf::IntRect(IDSlope_list[posTextureIndex][1], IDSlope_list[posTextureIndex][2], 32, 32)) });
		SlopesList[int(SlopesList.size()) - 1].property.setPosition({ i[1], i[2] });
		SlopesIDList.push_back(IDSlope_list[posTextureIndex][0]);
		setHitbox(SlopesList[int(SlopesList.size()) - 1].hitbox, { 0.f, 0.f, 32.f, 32.f });
	}
}
void Objectbuilding() {
	//Delete Effects
	DeleteAllBrickParticle();
	DeleteAllCoinEffect();
	DeleteAllGoombaAIEffect();
	DeleteAllScoreEffect();
	//Delete Objects
	DeleteAllBrick();
	DeleteAllCoin();
	DeleteAllLuckyBlock();
	DeleteAllGoombaAI();
	//(Re)build Objects
	player.property.setPosition(PlayerData[0], PlayerData[1]);
	for (const auto& i : BonusData) {
		if (i[0] == 1) AddCoin(static_cast<CoinID>(static_cast<int>(i[1])), static_cast<CoinAtt>(static_cast<int>(i[2])), i[3], i[4]);
		else if (i[0] == 2) AddBrick(static_cast<BrickID>(static_cast<int>(i[1])), static_cast<BrickAtt>(static_cast<int>(i[2])), i[3], i[4]);
		else if (i[0] == 3) AddLuckyBlock(static_cast<LuckyBlockID>(static_cast<int>(i[1])), static_cast<LuckyBlockAtt>(static_cast<int>(i[2])), i[3], i[4]);
	}
	for (const auto& i : EnemyData) {
		AddGoombaAI(static_cast<GoombaAIType>(i[0]), static_cast<int>(i[1]), i[2], i[3]);
	}
}