#include "Block/Obstacles.hpp"
#include "Core/Level.hpp"
#include "Object/Coin.hpp"
#include "Block/Brick.hpp"
#include "Block/LuckyBlock.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/Loading/Loading.hpp"
#include "Core/Collision/Collide.hpp"
#include "Object/GoombaAI.hpp"
#include "Effect/BrickParticle.hpp"
#include "Effect/CoinEffect.hpp"
#include "Effect/GoombaAIEffect.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Block/Slopes.hpp"
#include "Object/PiranhaAI.hpp"
#include "Object/Spike.hpp"
#include "Core/Scroll.hpp"
#include "Core/MusicManager.hpp"
#include "Core/Music.hpp"
#include "Core/Background/Bg.hpp"
#include "Object/ExitGate.hpp"

#include <utility>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <array>
#include <ctre.hpp>

#include "Core/WindowFrame.hpp"
#include "Core/Background/BgGradient.hpp"
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
void NPCDATAREAD(std::string line) {
	std::array<float, 5> temp{};
	bool flag = false;
	for (auto match : ctre::search_all<"(NPC_TYPE|NPC_ID|NPC_ATT|NPC_X|NPC_Y)=(\\S*)">(line)) {
		if (match.get<1>() == "NPC_TYPE") {
			temp[0] = match.get<2>().to_number<float>();
			if (!flag) flag = true;
		}
		else if (match.get<1>() == "NPC_ID") {
			temp[1] = match.get<2>().to_number<float>();
			if (!flag) flag = true;
		}
		else if (match.get<1>() == "NPC_ATT") {
			temp[2] = match.get<2>().to_number<float>();
			if (!flag) flag = true;
		}
		else if (match.get<1>() == "NPC_X") {
			temp[3] = match.get<2>().to_number<float>();
			if (!flag) flag = true;
		}
		else if (match.get<1>() == "NPC_Y") {
			temp[4] = match.get<2>().to_number<float>();
			if (!flag) flag = true;
		}
	}
	if (flag) EnemyData.push_back(temp);
}
void BONUSDATAREAD(std::string line) {
	std::array<float, 5> temp{};
	bool flag = false;
	for (auto match : ctre::search_all<"(BONUS_TYPE|BONUS_ID|BONUS_ATT|BONUS_X|BONUS_Y)=(\\S*)">(line)) {
		if (match.get<1>() == "BONUS_TYPE") {
			temp[0] = match.get<2>().to_number<float>();
			if (!flag) flag = true;
		}
		else if (match.get<1>() == "BONUS_ID") {
			temp[1] = match.get<2>().to_number<float>();
			if (!flag) flag = true;
		}
		else if (match.get<1>() == "BONUS_ATT") {
			temp[2] = match.get<2>().to_number<float>();
			if (!flag) flag = true;
		}
		else if (match.get<1>() == "BONUS_X") {
			temp[3] = match.get<2>().to_number<float>();
			if (!flag) flag = true;
		}
		else if (match.get<1>() == "BONUS_Y") {
			temp[4] = match.get<2>().to_number<float>();
			if (!flag) flag = true;
		}
	}
	if (flag) BonusData.push_back(temp);
}
void TILEDATAREAD(std::string line) {
	//TILE
	std::array<float, 3> temp{};
	bool flag = false;
	for (auto match : ctre::search_all<"(TILE_ID|TILE_X|TILE_Y)=(\\S*)">(line)) {
		if (match.get<1>() == "TILE_ID") {
			temp[0] = match.get<2>().to_number<float>();
			if (!flag) flag = true;
		}
		else if (match.get<1>() == "TILE_X") {
			temp[1] = match.get<2>().to_number<float>();
			if (!flag) flag = true;
		}
		else if (match.get<1>() == "TILE_Y") {
			temp[2] = match.get<2>().to_number<float>();
			if (!flag) flag = true;
		}
	}
	if (flag) LevelData.push_back(temp);
}
void LEVELDATAREAD(std::string line) {
	//BG
	for (auto match : ctre::search_all<"(BG_FIRST_COLOR|BG_SECOND_COLOR)=(\\S*)">(line)) {
		if (match.get<1>() == "BG_FIRST_COLOR") {
			bgGradient[0].color = sf::Color(hex_to_int(match.get<2>().to_string().substr(0, 2)), hex_to_int(match.get<2>().to_string().substr(2, 2)), hex_to_int(match.get<2>().to_string().substr(4, 2)), 255);
			bgGradient[1].color = sf::Color(hex_to_int(match.get<2>().to_string().substr(0, 2)), hex_to_int(match.get<2>().to_string().substr(2, 2)), hex_to_int(match.get<2>().to_string().substr(4, 2)), 255);
		}
		else if (match.get<1>() == "BG_SECOND_COLOR") {
			bgGradient[2].color = sf::Color(hex_to_int(match.get<2>().to_string().substr(0, 2)), hex_to_int(match.get<2>().to_string().substr(2, 2)), hex_to_int(match.get<2>().to_string().substr(4, 2)), 255);
			bgGradient[3].color = sf::Color(hex_to_int(match.get<2>().to_string().substr(0, 2)), hex_to_int(match.get<2>().to_string().substr(2, 2)), hex_to_int(match.get<2>().to_string().substr(4, 2)), 255);
		}
	}
	//EXIT
	for (auto match : ctre::search_all<"(EXIT_INDICATOR_X|EXIT_INDICATOR_Y|EXIT_GATE_X|EXIT_GATE_Y)=(\\S*)">(line)) {
		if (match.get<1>() == "EXIT_INDICATOR_X") ExitGateData[0] = match.get<2>().to_number<float>();
		else if (match.get<1>() == "EXIT_INDICATOR_Y") ExitGateData[1] = match.get<2>().to_number<float>();
		else if (match.get<1>() == "EXIT_GATE_X") ExitGateData[2] = match.get<2>().to_number<float>();
		else if (match.get<1>() == "EXIT_GATE_Y") ExitGateData[3] = match.get<2>().to_number<float>();
	}
	//LEVELDATA
	for (auto match : ctre::search_all<"(LEVEL_WIDTH|LEVEL_HEIGHT)=(\\S*)">(line)) {
		if (match.get<1>() == "LEVEL_WIDTH") LevelWidth = match.get<2>().to_number<float>();
		else if (match.get<1>() == "LEVEL_HEIGHT") LevelHeight = match.get<2>().to_number<float>();
	}
	//MARIOPOS
	for (auto match : ctre::search_all<"(MARIO_X|MARIO_Y)=(\\S*)">(line)) {
		if (match.get<1>() == "MARIO_X") PlayerData[0] = match.get<2>().to_number<float>();
		else if (match.get<1>() == "MARIO_Y") PlayerData[1] = match.get<2>().to_number<float>();
	}
	//MUSIC
	for (auto match : ctre::search_all<"(MUSIC_TYPE|MUSIC_NAME)=(\\S*)">(line)) {
		if (match.get<1>() == "MUSIC_TYPE") MusicData.first = match.get<2>().to_number<int>();
		else if (match.get<1>() == "MUSIC_NAME") MusicData.second = match.get<2>().to_string();
	}
}
void ReadData(std::string path) {
	std::string lvldat;
	LoadLvl(lvldat, std::move(path));
	std::stringstream input_view(lvldat);
	std::string line;
	int ReadMode = 0;
	// Read the file
	bool flag = false;
	while (std::getline(input_view, line)) {
		flag = false;
		for (auto match : ctre::search_all<"(\\[LEVEL_DATA\\]|\\[TILE_DATA\\]|\\[BONUS_DATA\\]|\\[NPC_DATA\\])">(line)) {
			if (match.get<0>() == "[LEVEL_DATA]") {
				ReadMode = 0;
				flag = true;
			}
			else if (match.get<0>() == "[TILE_DATA]") {
				ReadMode = 1;
				flag = true;
			}
			else if (match.get<0>() == "[BONUS_DATA]") {
				ReadMode = 2;
				flag = true;
			}
			else if (match.get<0>() == "[NPC_DATA]") {
				ReadMode = 3;
				flag = true;
			}
		}
		if (flag) continue;

		switch (ReadMode) {
			case 0: LEVELDATAREAD(line); break;
			case 1: TILEDATAREAD(line); break;
			case 2: BONUSDATAREAD(line); break;
			case 3: NPCDATAREAD(line); break;
			default: ;
		}
	}
}
/*
void ReadData(std::string path) {
	std::string lvldat;
	LoadLvl(lvldat, path);
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
*/
void Obstaclebuilding() {
	ObstaclesVA.setPrimitiveType(sf::PrimitiveType::Triangles);
	ObstaclesVA.resize(LevelData.size() * 6);
	for (int i = 0; i < LevelData.size(); ++i) {
		//for (const auto& i : LevelData) {
			// Find the tile id
		const int posTextureIndex = std::ranges::find_if(ID_list, [&i](const std::array<int, 3> &compare) {
			return compare[0] == static_cast<int>(LevelData[i][0]);
		}) - (ID_list.begin());
		// Then use the index of tile id property to add it to the list
		//ObstaclesList.push_back(Obstacles{ int(LevelData[i][0]), sf::Sprite(*ObstaclesTextureManager.GetTexture("Obstacles_" + std::to_string(posTextureIndex))) });
		//ObstaclesList.emplace_back(Obstacles{ static_cast<int>(LevelData[i][0]), sf::Sprite(*ObstaclesTextureManager.GetTexture("Tileset"), sf::IntRect({ID_list[posTextureIndex][1], ID_list[posTextureIndex][2] }, {32, 32})) });
		//ObstaclesList.push_back(Obstacles{ int(i[0]), sf::Sprite(*ObstaclesTextureList[i[0]]) });
		//ObstaclesList[static_cast<int>(ObstaclesList.size()) - 1].property.setPosition({ LevelData[i][1], LevelData[i][2] });
		ObstaclesVertPosList.emplace_back(sf::FloatRect({ 0.f, 0.f }, { 32.f, 32.f }) ,sf::Vector2f({LevelData[i][1], LevelData[i][2] }));
		ObstaclesHorzPosList.emplace_back(sf::FloatRect({ 0.f, 0.f }, { 32.f, 32.f }) ,sf::Vector2f({LevelData[i][1], LevelData[i][2] }));
		//setHitbox(ObstaclesList[static_cast<int>(ObstaclesList.size()) - 1].hitbox, sf::FloatRect({ 0.f, 0.f }, { 32.f, 32.f }));

		sf::Vertex* vertex = &ObstaclesVA[i * 6];
		vertex[0].position = sf::Vector2f(LevelData[i][1], LevelData[i][2]);
		vertex[1].position = sf::Vector2f(LevelData[i][1] + 32.0f, LevelData[i][2]);
		vertex[2].position = sf::Vector2f(LevelData[i][1], LevelData[i][2] + 32.0f);
		vertex[3].position = sf::Vector2f(LevelData[i][1], LevelData[i][2] + 32.0f);
		vertex[4].position = sf::Vector2f(LevelData[i][1] + 32.0f, LevelData[i][2]);
		vertex[5].position = sf::Vector2f(LevelData[i][1] + 32.0f, LevelData[i][2] + 32.0f);

		vertex[0].texCoords = sf::Vector2f(ID_list[LevelData[i][0]][1], ID_list[LevelData[i][0]][2]);
		vertex[1].texCoords = sf::Vector2f(ID_list[LevelData[i][0]][1] + 32.0f, ID_list[LevelData[i][0]][2]);
		vertex[2].texCoords = sf::Vector2f(ID_list[LevelData[i][0]][1], ID_list[LevelData[i][0]][2] + 32.0f);
		vertex[3].texCoords = sf::Vector2f(ID_list[LevelData[i][0]][1], ID_list[LevelData[i][0]][2] + 32.0f);
		vertex[4].texCoords = sf::Vector2f(ID_list[LevelData[i][0]][1] + 32.0f, ID_list[LevelData[i][0]][2]);
		vertex[5].texCoords = sf::Vector2f(ID_list[LevelData[i][0]][1] + 32.0f, ID_list[LevelData[i][0]][2] + 32.0f);
	}
	//std::ranges::sort(ObstaclesList, [](const Obstacles& A, const Obstacles& B) {
		//return A.property.getPosition().x < B.property.getPosition().x;
		//});
	std::ranges::sort(ObstaclesHorzPosList, [](const std::pair<sf::FloatRect, sf::Vector2f>& A, const std::pair<sf::FloatRect, sf::Vector2f>& B) {
		if (A.second.x < B.second.x) return true;
		else if (A.second.x == B.second.x) return A.second.y < B.second.y;
		else return false;
		});
	std::ranges::sort(ObstaclesVertPosList, [](const std::pair<sf::FloatRect, sf::Vector2f>& A, const std::pair<sf::FloatRect, sf::Vector2f>& B) {
		if (A.second.y < B.second.y) return true;
		else if (A.second.y == B.second.y) return A.second.x < B.second.x;
		else return false;
		});
}
void Slopebuilding() {
	std::ranges::sort(SlopeData, [](const std::array<float, 3>& a, const std::array<float, 3>& b) {return a[1] < b[1]; });
	for (const auto& i : SlopeData) {
		// Find the tile id
		const int posTextureIndex = std::ranges::find_if(IDSlope_list, [&i](const std::array<int, 3> &compare) {
			return compare[0] == static_cast<int>(i[0]);
		}) - (IDSlope_list.begin());
		// Then use the index of tile id property to add it to the list
		SlopesList.push_back(Obstacles{ static_cast<int>(i[0]), sf::Sprite(SlopeTexture, sf::IntRect({IDSlope_list[posTextureIndex][1], IDSlope_list[posTextureIndex][2] }, {32, 32}))
			});
		SlopesList[static_cast<int>(SlopesList.size()) - 1].property.setPosition({ i[1], i[2] });
		SlopesIDList.push_back(IDSlope_list[posTextureIndex][0]);
		setHitbox(SlopesList[static_cast<int>(SlopesList.size()) - 1].hitbox, sf::FloatRect({ 0.f, 0.f }, { 32.f, 32.f }));
	}
}
void Bgbuilding() {
	for (const auto& i : BgData) {
		AddBg(static_cast<int>(i[0]), static_cast<int>(i[1]));
	}
}
void ExitGateBuilding() {
	ExitGateBack.setPosition({ ExitGateData[2], ExitGateData[3] });
	ExitGateIndicator.setPosition({ ExitGateData[0], ExitGateData[1] });
	ExitGateFore.setPosition({ ExitGateBack.getPosition().x + 43.0f, ExitGateBack.getPosition().y - 250.0f });
	ExitGateForeCurr = ExitGateForePrev = ExitGateFore.getPosition();
}
void Objectbuilding() {
	std::ranges::sort(BonusData, [](const std::array<float, 5>& a, const std::array<float, 5>& b) {return a[3] < b[3]; });
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
	if (!BonusData.empty()) {
		for (const auto& i : BonusData) {
			if (i[0] == 1) AddCoin(static_cast<CoinID>(static_cast<int>(i[1])), static_cast<CoinAtt>(static_cast<int>(i[2])), i[3], i[4]);
			else if (i[0] == 2) AddBrick(static_cast<BrickID>(static_cast<int>(i[1])), static_cast<BrickAtt>(static_cast<int>(i[2])), i[3], i[4]);
			else if (i[0] == 3) AddLuckyBlock(static_cast<LuckyBlockID>(static_cast<int>(i[1])), static_cast<LuckyBlockAtt>(static_cast<int>(i[2])), i[3], i[4]);
		}
	}
	if (!EnemyData.empty()) {
		for (const auto& i : EnemyData) {
			if (i[0] == 0) AddGoombaAI(static_cast<GoombaAIType>(i[1]), static_cast<int>(i[2]), i[3], i[4], LEFT);
			else if (i[0] == 1) AddPiranha(static_cast<PiranhaID>(i[1]), i[3], i[4]);
			else if (i[0] == 2) AddSpike(static_cast<SpikeID>(i[1]), i[3], i[4]);
		}
	}
	BricksSort();
	LuckyBlockSort();
}