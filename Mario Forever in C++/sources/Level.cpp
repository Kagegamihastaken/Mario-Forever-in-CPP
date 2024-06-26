#include "../headers/Obstacles.hpp"
#include "../headers/Level.hpp"
#include "../headers/Coin.hpp"
#include "../headers/Brick.hpp"
#include "../headers/LuckyBlock.hpp"
#include "../headers/enum.hpp"
#include "../headers/Loading.hpp"

#include "../resource.h"

#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
// Level data
float LevelWidth, LevelHeight;
std::vector<std::vector<float>> LevelData;
void ReadData(int IDLevel) {
	std::string lvldat;
	LoadLvl(lvldat, IDLevel, LVLFILE);
	//std::cout << lvldat;
	float value;
	std::string DataStructure;
	std::vector<float> temp;
	int tm = 0;
	std::string numLoop;
	// Read the file
	while (true) {
		tm = ReadStrLine(lvldat, DataStructure, tm);
		if (tm == -1) break;
		if (DataStructure == "--Level Data--") {
			while (true) {
				tm = ReadStrLine(lvldat, DataStructure, tm);
				if (DataStructure == "") continue;
				int C = 0;
				numLoop = "";
				for (const auto& i : DataStructure) {
					if (i != ' ') numLoop += i;
					else {
						if (numLoop != "") {
							if (C == 0) LevelWidth = std::stof(numLoop);
							else if (C == 1) LevelHeight = std::stof(numLoop);
						}
						++C;
						numLoop = "";
					}
				}
				if (numLoop != "") {
					if (C == 0) LevelWidth = std::stof(numLoop);
					else if (C == 1) LevelHeight = std::stof(numLoop);
				}
				break;
			}
		}
		if (DataStructure == "--Tile Data--") {
			while (true) {
				tm = ReadStrLine(lvldat, DataStructure, tm);
				if (DataStructure == "") continue;
				temp.clear();
				numLoop = "";
				//if said Bonus Data break it
				if (DataStructure == "--Bonus Data--") break;
				for (const auto& i : DataStructure) {
					if (i != ' ') numLoop += i;
					else {
						if (numLoop != "") temp.push_back(std::stof(numLoop));
						numLoop = "";
					}
				}
				if (tm == -1) break;
				if (numLoop != "") temp.push_back(std::stof(numLoop));
				LevelData.push_back(temp);
			}
		}
		if (DataStructure == "--Bonus Data--") {
			while (true) {
				tm = ReadStrLine(lvldat, DataStructure, tm);
				if (DataStructure == "") continue;
				temp.clear();
				numLoop = "";
				//if said Enemies Data break it
				if (DataStructure == "--Enemies Data--") break;
				for (const auto& i : DataStructure) {
					if (i != ' ') numLoop += i;
					else {
						if (numLoop != "") temp.push_back(std::stof(numLoop));
						numLoop = "";
					}
				}
				if (tm == -1) break;
				if (numLoop != "") temp.push_back(std::stof(numLoop));
				if (temp[0] == 1) AddCoin(static_cast<CoinID>(static_cast<int>(temp[1])), static_cast<CoinAtt>(static_cast<int>(temp[2])), temp[3], temp[4]);
				else if (temp[0] == 2) AddBrick(static_cast<BrickID>(static_cast<int>(temp[1])), static_cast<BrickAtt>(static_cast<int>(temp[2])), temp[3], temp[4]);
				else if (temp[0] == 3) AddLuckyBlock(static_cast<LuckyBlockID>(static_cast<int>(temp[1])), static_cast<LuckyBlockAtt>(static_cast<int>(temp[2])), temp[3], temp[4]);
			}
		}
	}
}
void building() {
	int posTextureIndex;
	for (const auto& i : LevelData) {
		// Find the tile id
		posTextureIndex = find_if(ID_list.begin(), ID_list.end(), [&i](const std::vector<int>& compare) {return compare[0] == int(i[0]); }) - (ID_list.begin());
		// Then use the index of tile id property to add it to the list
		ObstaclesList.push_back(Obstacles{ int(i[0]), sf::Sprite(ObstaclesTexture, sf::IntRect(ID_list[posTextureIndex][1], ID_list[posTextureIndex][2], 32, 32)) });
		ObstaclesList[int(ObstaclesList.size()) - 1].property.setPosition({ i[1], i[2] });
		ObstaclesList[int(ObstaclesList.size()) - 1].setHitbox({ 0.f, 0.f, 32.f, 32.f });
	}
}