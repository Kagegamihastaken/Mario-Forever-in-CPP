#include "../headers/Obstacles.hpp"
#include "../headers/Level.hpp"
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
// Level data
float LevelWidth, LevelHeight;
std::vector<std::vector<float>> LevelData;
void ReadData(std::ifstream& foi) {
	float value;
	std::string DataStructure;
	std::vector<float> temp;

	std::string numLoop;
	// Read the file
	while (true) {
		getline(foi, DataStructure);
		if (DataStructure == "--Level Data--") {
			while (true) {
				getline(foi, DataStructure);
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
				getline(foi, DataStructure);
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
				if (numLoop != "") temp.push_back(std::stof(numLoop));
				LevelData.push_back(temp);
				if (foi.eof()) break;
			}
		}
		if (foi.eof()) break;
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