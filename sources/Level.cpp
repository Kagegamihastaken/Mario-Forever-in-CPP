#include "../headers/Block/Obstacles.hpp"
#include "../headers/Core/Level.hpp"
#include "../headers/Object/Coin.hpp"
#include "../headers/Block/Brick.hpp"
#include "../headers/Block/LuckyBlock.hpp"
#include "../headers/Core/Loading/enum.hpp"
#include "../headers/Core/Loading/Loading.hpp"
#include "../headers/Core/Collision/Collide.hpp"
#include "../headers/Object/GoombaAI.hpp"
#include "../headers/Effect/BrickParticle.hpp"
#include "../headers/Effect/CoinEffect.hpp"
#include "../headers/Effect/GoombaAIEffect.hpp"
#include "../headers/Effect/ScoreEffect.hpp"
#include "../headers/Block/Slopes.hpp"
#include "../headers/Object/PiranhaAI.hpp"
#include "../headers/Object/Spike.hpp"
#include "../headers/Core/Scroll.hpp"
#include "../headers/Core/MusicManager.hpp"
#include "../headers/Core/Music.hpp"
#include "../headers/Core/Background/BgGradient.hpp"
#include "../headers/Core/Background/Bg.hpp"
#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Object/ExitGate.hpp"

#include "../resource.h"

#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <array>
// Level data
float LevelWidth, LevelHeight;
std::vector<std::array<float, 2>> BgData;
std::vector<std::array<float, 3>> LevelData;
std::vector<std::array<float, 3>> SlopeData;
std::vector<std::array<float, 5>> BonusData;
std::vector<std::array<float, 5>> EnemyData;
std::array<float, 4> ExitGateData;
std::array<float, 2> PlayerData;
std::pair<int, std::string> MusicData;
void ReadData(int IDLevel) {
	std::string lvldat;
	LoadLvl(lvldat, IDLevel);
	float value = 0.0f;
	std::string DataStructure;
	std::vector<float> temp;
	int tm = 0;
	std::string numLoop;
	// Read the file
	while (true) {
		if (DataStructure != "--Level Data--" && DataStructure != "--Tile Data--" && DataStructure != "--Bonus Data--" && DataStructure != "--Enemy Data--" && DataStructure != "--Slope Data--" && DataStructure != "--Background Data--") {
			tm = ReadStrLine(lvldat, DataStructure, tm);
		}
		if (tm == -1) break;
		if (DataStructure == "--Level Data--") {
			int C;
			for (int i = 0; i < 5; ++i) {
				while (true) {
					tm = ReadStrLine(lvldat, DataStructure, tm);
					if (DataStructure == "") continue;
					C = 0;
					numLoop = "";
					for (const auto& j : DataStructure) {
						if (j != ' ') numLoop += j;
						else {
							switch (i) {
							case 0:
								if (C == 0) {
									bgGradient[0].color = sf::Color(hex_to_int(numLoop.substr(0, 2)), hex_to_int(numLoop.substr(2, 2)), hex_to_int(numLoop.substr(4, 2)), 255);
									bgGradient[1].color = sf::Color(hex_to_int(numLoop.substr(0, 2)), hex_to_int(numLoop.substr(2, 2)), hex_to_int(numLoop.substr(4, 2)), 255);
								}
								else {
									bgGradient[2].color = sf::Color(hex_to_int(numLoop.substr(0, 2)), hex_to_int(numLoop.substr(2, 2)), hex_to_int(numLoop.substr(4, 2)), 255);
									bgGradient[3].color = sf::Color(hex_to_int(numLoop.substr(0, 2)), hex_to_int(numLoop.substr(2, 2)), hex_to_int(numLoop.substr(4, 2)), 255);
								}
								break;
							case 1:
								ExitGateData[C] = std::stof(numLoop);
								break;
							case 2:
								if (C == 0) LevelWidth = std::stof(numLoop);
								else if (C == 1) LevelHeight = std::stof(numLoop);
								break;
							case 3:
								PlayerData[C] = std::stof(numLoop);
								break;
							case 4:
								if (C == 0) MusicData.first = std::stoi(numLoop);
								else if (C == 1) MusicData.second = numLoop;
								break;
							}
							++C;
							numLoop = "";
						}
					}
					if (numLoop != "") {
						switch (i) {
						case 0:
							if (C == 0) {
								bgGradient[0].color = sf::Color(hex_to_int(numLoop.substr(0, 2)), hex_to_int(numLoop.substr(2, 2)), hex_to_int(numLoop.substr(4, 2)), 255);
								bgGradient[1].color = sf::Color(hex_to_int(numLoop.substr(0, 2)), hex_to_int(numLoop.substr(2, 2)), hex_to_int(numLoop.substr(4, 2)), 255);
							}
							else {
								bgGradient[2].color = sf::Color(hex_to_int(numLoop.substr(0, 2)), hex_to_int(numLoop.substr(2, 2)), hex_to_int(numLoop.substr(4, 2)), 255);
								bgGradient[3].color = sf::Color(hex_to_int(numLoop.substr(0, 2)), hex_to_int(numLoop.substr(2, 2)), hex_to_int(numLoop.substr(4, 2)), 255);
							}
							break;
						case 1:
							ExitGateData[C] = std::stof(numLoop);
							break;
						case 2:
							if (C == 0) LevelWidth = std::stof(numLoop);
							else if (C == 1) LevelHeight = std::stof(numLoop);
							break;
						case 3:
							PlayerData[C] = std::stof(numLoop);
							break;
						case 4:
							if (C == 0) MusicData.first = std::stoi(numLoop);
							else if (C == 1) MusicData.second = numLoop;
							break;
						}
					}
					break;
				}
			}
		}
		if (DataStructure == "--Background Data--") {
			while (true) {
				tm = ReadStrLine(lvldat, DataStructure, tm);
				if (DataStructure == "") continue;
				temp.clear();
				numLoop = "";
				//if said Bonus Data break it
				if (DataStructure == "--Bonus Data--" || DataStructure == "--Enemy Data--" || DataStructure == "--Slope Data--" || DataStructure == "--Tile Data--") break;
				for (const auto& i : DataStructure) {
					if (i != ' ') numLoop += i;
					else {
						if (numLoop != "") temp.push_back(std::stof(numLoop));
						numLoop = "";
					}
				}
				if (numLoop != "") temp.push_back(std::stof(numLoop));
				BgData.push_back({ temp[0], temp[1] });
				if (tm == -1) break;
			}
		}
		if (DataStructure == "--Tile Data--") {
			while (true) {
				tm = ReadStrLine(lvldat, DataStructure, tm);
				if (DataStructure == "") continue;
				temp.clear();
				numLoop = "";
				//if said Bonus Data break it
				if (DataStructure == "--Bonus Data--" || DataStructure == "--Enemy Data--" || DataStructure == "--Slope Data--" || DataStructure == "--Background Data--") break;
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
				if (DataStructure == "--Tile Data--" || DataStructure == "--Enemy Data--" || DataStructure == "--Slope Data--" || DataStructure == "--Background Data--") break;
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
				if (DataStructure == "--Bonus Data--" || DataStructure == "--Tile Data--" || DataStructure == "--Slope Data--" || DataStructure == "--Background Data--") break;
				for (const auto& i : DataStructure) {
					if (i != ' ') numLoop += i;
					else {
						if (numLoop != "") temp.push_back(std::stof(numLoop));
						numLoop = "";
					}
				}
				if (numLoop != "") temp.push_back(std::stof(numLoop));
				EnemyData.push_back({ temp[0], temp[1], temp[2], temp[3], temp[4] });
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
				if (DataStructure == "--Bonus Data--" || DataStructure == "--Enemy Data--" || DataStructure == "--Tile Data--" || DataStructure == "--Background Data--") break;
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
	//ObstaclesVA.setPrimitiveType(sf::PrimitiveType::Triangles);
	//ObstaclesVA.resize(LevelData.size() * 6);
	for (int i = 0; i < LevelData.size(); ++i) {
		//for (const auto& i : LevelData) {
			// Find the tile id
		posTextureIndex = find_if(ID_list.begin(), ID_list.end(), [&i](const std::array<int, 3>& compare) {return compare[0] == int(LevelData[i][0]); }) - (ID_list.begin());
		// Then use the index of tile id property to add it to the list
		//ObstaclesList.push_back(Obstacles{ int(LevelData[i][0]), sf::Sprite(*ObstaclesTextureManager.GetTexture("Obstacles_" + std::to_string(posTextureIndex))) });
		ObstaclesList.push_back(Obstacles{ int(LevelData[i][0]), sf::Sprite(*ObstaclesTextureManager.GetTexture("Tileset"), sf::IntRect({ID_list[posTextureIndex][1], ID_list[posTextureIndex][2] }, {32, 32})) });
		//ObstaclesList.push_back(Obstacles{ int(i[0]), sf::Sprite(*ObstaclesTextureList[i[0]]) });
		ObstaclesList[int(ObstaclesList.size()) - 1].property.setPosition({ LevelData[i][1], LevelData[i][2] });
		ObstaclesVertPosList.push_back({ sf::FloatRect({ 0.f, 0.f }, { 32.f, 32.f }) ,{LevelData[i][1], LevelData[i][2] } });
		setHitbox(ObstaclesList[int(ObstaclesList.size()) - 1].hitbox, sf::FloatRect({ 0.f, 0.f }, { 32.f, 32.f }));

		//sf::Vertex* vertex = &ObstaclesVA[i * 6];
		//vertex[0].position = sf::Vector2f(LevelData[i][1], LevelData[i][2]);
		//vertex[1].position = sf::Vector2f(LevelData[i][1] + 32.0f, LevelData[i][2]);
		//vertex[2].position = sf::Vector2f(LevelData[i][1], LevelData[i][2] + 32.0f);
		//vertex[3].position = sf::Vector2f(LevelData[i][1], LevelData[i][2] + 32.0f);
		//vertex[4].position = sf::Vector2f(LevelData[i][1] + 32.0f, LevelData[i][2]);
		//vertex[5].position = sf::Vector2f(LevelData[i][1] + 32.0f, LevelData[i][2] + 32.0f);

		//vertex[0].texCoords = sf::Vector2f(ID_list[LevelData[i][0]][1], ID_list[LevelData[i][0]][2]);
		//vertex[1].texCoords = sf::Vector2f(ID_list[LevelData[i][0]][1] + 32.0f, ID_list[LevelData[i][0]][2]);
		//vertex[2].texCoords = sf::Vector2f(ID_list[LevelData[i][0]][1], ID_list[LevelData[i][0]][2] + 32.0f);
		//vertex[3].texCoords = sf::Vector2f(ID_list[LevelData[i][0]][1], ID_list[LevelData[i][0]][2] + 32.0f);
		//vertex[4].texCoords = sf::Vector2f(ID_list[LevelData[i][0]][1] + 32.0f, ID_list[LevelData[i][0]][2]);
		//vertex[5].texCoords = sf::Vector2f(ID_list[LevelData[i][0]][1] + 32.0f, ID_list[LevelData[i][0]][2] + 32.0f);
	}
	sort(ObstaclesList.begin(), ObstaclesList.end(), [](Obstacles& A, Obstacles& B) {
		return A.property.getPosition().x < B.property.getPosition().x;
		});
	sort(ObstaclesVertPosList.begin(), ObstaclesVertPosList.end(), [](std::pair<sf::FloatRect, sf::Vector2f>& A, std::pair<sf::FloatRect, sf::Vector2f>& B) {
		if (A.second.y < B.second.y) return true;
		else if (A.second.y == B.second.y) return A.second.x < B.second.x;
		else return false;
		});
}
void Slopebuilding() {
	std::sort(SlopeData.begin(), SlopeData.end(), [](const std::array<float, 3>& a, const std::array<float, 3>& b) {return a[1] < b[1]; });
	int posTextureIndex;
	for (const auto& i : SlopeData) {
		// Find the tile id
		posTextureIndex = find_if(IDSlope_list.begin(), IDSlope_list.end(), [&i](const std::array<int, 3>& compare) {return compare[0] == int(i[0]); }) - (IDSlope_list.begin());
		// Then use the index of tile id property to add it to the list
		SlopesList.push_back(Obstacles{ int(i[0]), sf::Sprite(SlopeTexture, sf::IntRect({IDSlope_list[posTextureIndex][1], IDSlope_list[posTextureIndex][2] }, {32, 32}))
			});
		SlopesList[int(SlopesList.size()) - 1].property.setPosition({ i[1], i[2] });
		SlopesIDList.push_back(IDSlope_list[posTextureIndex][0]);
		setHitbox(SlopesList[int(SlopesList.size()) - 1].hitbox, sf::FloatRect({ 0.f, 0.f }, { 32.f, 32.f }));
	}
}
void Bgbuilding() {
	for (const auto& i : BgData) {
		AddBg(int(i[0]), int(i[1]));
	}
}
void ExitGateBuilding() {
	ExitGateBack.setPosition({ ExitGateData[2], ExitGateData[3] });
	ExitGateIndicator.setPosition({ ExitGateData[0], ExitGateData[1] });
	ExitGateFore.setPosition({ ExitGateBack.getPosition().x + 43.0f, ExitGateBack.getPosition().y - 250.0f });
	ExitGateForeCurr = ExitGateForePrev = ExitGateFore.getPosition();
}
void Objectbuilding() {
	sort(BonusData.begin(), BonusData.end(), [](const std::array<float, 5>& a, const std::array<float, 5>& b) {return a[3] < b[3]; });
	//Musicial
	if (MusicData.first == 0) {
		if (Music.IsMODMusicPlaying(MusicData.second)) Music.StopMODMusic(MusicData.second);
		Music.SetMODLoop(MusicData.second, true);
		Music.PlayMODMusic(MusicData.second);
	}
	else if (MusicData.first == 1) {
		if (Music.IsOGGMusicPlaying(MusicData.second)) Music.StopOGGMusic(MusicData.second);
		Music.SetOGGLoop(MusicData.second, true);
		Music.PlayOGGMusic(MusicData.second);
	}
	player.property.setPosition({ PlayerData[0], PlayerData[1] });
	player.curr = player.prev = player.property.getPosition();
	MarioDirection = FirstMarioDirection;
	//rTexture.resize(sf::Vector2u({ static_cast<unsigned int>(LevelWidth) * 2, static_cast<unsigned int>(LevelHeight) * 2 }));
	setView();
	updateView();
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
	ClearPiranhaAI();
	DeleteAllSpike();
	//(Re)build Objects
	if (BonusData.size() > 0) {
		for (const auto& i : BonusData) {
			if (i[0] == 1) AddCoin(static_cast<CoinID>(static_cast<int>(i[1])), static_cast<CoinAtt>(static_cast<int>(i[2])), i[3], i[4]);
			else if (i[0] == 2) AddBrick(static_cast<BrickID>(static_cast<int>(i[1])), static_cast<BrickAtt>(static_cast<int>(i[2])), i[3], i[4]);
			else if (i[0] == 3) AddLuckyBlock(static_cast<LuckyBlockID>(static_cast<int>(i[1])), static_cast<LuckyBlockAtt>(static_cast<int>(i[2])), i[3], i[4]);
		}
	}
	if (EnemyData.size() > 0) {
		for (const auto& i : EnemyData) {
			if (i[0] == 0) AddGoombaAI(static_cast<GoombaAIType>(i[1]), static_cast<int>(i[2]), i[3], i[4], LEFT);
			else if (i[0] == 1) AddPiranha(static_cast<PiranhaID>(i[1]), i[3], i[4]);
			else if (i[0] == 2) AddSpike(static_cast<SpikeID>(i[1]), i[3], i[4]);
		}
	}
	BricksSort();
	LuckyBlockSort();
}