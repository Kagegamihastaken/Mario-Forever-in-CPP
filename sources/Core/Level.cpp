#include "Block/Obstacles.hpp"
#include "Core/Level.hpp"
#include "Object/Coin.hpp"
#include "Block/Brick.hpp"
#include "Block/BulletLauncher.hpp"
#include "Block/LuckyBlock.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/Loading/Loading.hpp"
#include "Object/GoombaAI.hpp"
#include "Effect/BrickParticle.hpp"
#include "Effect/CoinEffect.hpp"
#include "Effect/GoombaAIEffect.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Core/Exception.hpp"
#include "Object/PiranhaAI.hpp"
#include "Object/Spike.hpp"
#include "Core/Scroll.hpp"
#include "Core/MusicManager.hpp"
#include "Core/Background/Bg.hpp"
#include "Object/ExitGate.hpp"
#include "Editor/SelectTile.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Background/BgGradient.hpp"
#include "Effect/BroAIEffect.hpp"
#include "Effect/FireballExplosion.hpp"
#include "Object/BroAI.hpp"
#include "Projectiles/BroAIProjectile.hpp"
#include "Projectiles/MarioProjectile.hpp"
#include "Core/Tilemap.hpp"
#include "Object/BulletBill.hpp"
#include "Object/Mario.hpp"
// Level data
float LevelWidth, LevelHeight;
std::vector<std::array<float, 2>> BgData;
std::vector<std::array<float, 3>> LevelData;
std::vector<std::array<float, 3>> SlopeData;
std::vector<std::array<float, 5>> BonusData;
std::vector<std::array<float, 5>> EnemyData;
std::array<float, 4> ExitGateData;
std::array<float, 2> PlayerData;
std::string MusicData;
/*
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
*/
void TILEDATAREAD(const std::string_view line) {
	//TILE
	std::array<int, 4> temp{};
	bool flag = false;
	for (auto match : ctre::search_all<"(EI|EP|EX|EY)=(\\S*)">(line)) {
		if (match.get<1>() == "EI") {
			temp[0] = match.get<2>().to_number<int>();
			if (!flag) flag = true;
		}
		else if (match.get<1>() == "EP") {
			temp[1] = match.get<2>().to_number<int>();
			if (!flag) flag = true;
		}
		else if (match.get<1>() == "EX") {
			temp[2] = match.get<2>().to_number<int>();
			if (!flag) flag = true;
		}
		else if (match.get<1>() == "EY") {
			temp[3] = match.get<2>().to_number<int>();
			if (!flag) flag = true;
		}
	}
	if (flag) {
		switch (const SelectTileData* ReadTile = &TilePage[temp[1]][temp[0]]; ReadTile->categoryID) {
			case 0:
				LevelData.push_back({static_cast<float>(ReadTile->objectID), static_cast<float>(temp[2]), static_cast<float>(temp[3])});
				break;
			case 1:
				BonusData.push_back({static_cast<float>(ReadTile->objectID), static_cast<float>(ReadTile->customID1), static_cast<float>(ReadTile->customID2), static_cast<float>(temp[2]), static_cast<float>(temp[3])});
				break;
			case 2:
				EnemyData.push_back({static_cast<float>(ReadTile->objectID), static_cast<float>(ReadTile->customID1), static_cast<float>(ReadTile->customID2), static_cast<float>(temp[2]), static_cast<float>(temp[3])});
				break;
			default: ;
		}
	}
}
void LEVELDATAREAD(const std::string& line) {
	//BG
	for (auto match : ctre::search_all<"(BGFC|BGSC)=(\\S*)">(line)) {
		if (match.get<1>() == "BGFC") {
			bgGradient[0].color = sf::Color(hex_to_int(match.get<2>().to_string().substr(0, 2)), hex_to_int(match.get<2>().to_string().substr(2, 2)), hex_to_int(match.get<2>().to_string().substr(4, 2)), 255);
			bgGradient[1].color = sf::Color(hex_to_int(match.get<2>().to_string().substr(0, 2)), hex_to_int(match.get<2>().to_string().substr(2, 2)), hex_to_int(match.get<2>().to_string().substr(4, 2)), 255);
		}
		else if (match.get<1>() == "BGSC") {
			bgGradient[2].color = sf::Color(hex_to_int(match.get<2>().to_string().substr(0, 2)), hex_to_int(match.get<2>().to_string().substr(2, 2)), hex_to_int(match.get<2>().to_string().substr(4, 2)), 255);
			bgGradient[3].color = sf::Color(hex_to_int(match.get<2>().to_string().substr(0, 2)), hex_to_int(match.get<2>().to_string().substr(2, 2)), hex_to_int(match.get<2>().to_string().substr(4, 2)), 255);
		}
	}
	//EXIT
	for (auto match : ctre::search_all<"(EGIX|EGIY|EGX|EGY)=(\\S*)">(line)) {
		if (match.get<1>() == "EGIX") ExitGateData[0] = match.get<2>().to_number<float>();
		else if (match.get<1>() == "EGIY") ExitGateData[1] = match.get<2>().to_number<float>();
		else if (match.get<1>() == "EGX") ExitGateData[2] = match.get<2>().to_number<float>();
		else if (match.get<1>() == "EGY") ExitGateData[3] = match.get<2>().to_number<float>();
	}
	//LEVELDATA
	for (auto match : ctre::search_all<"(LVLW|LVLH)=(\\S*)">(line)) {
		if (match.get<1>() == "LVLW") LevelWidth = match.get<2>().to_number<int>();
		else if (match.get<1>() == "LVLH") LevelHeight = match.get<2>().to_number<int>();
	}
	//MARIOPOS
	for (auto match : ctre::search_all<"(MX|MY)=(\\S*)">(line)) {
		if (match.get<1>() == "MX") PlayerData[0] = match.get<2>().to_number<int>();
		else if (match.get<1>() == "MY") PlayerData[1] = match.get<2>().to_number<int>();
	}
	//MUSIC
	for (auto match : ctre::search_all<"(MUSIC)=(\\S*)">(line)) {
		if (match.get<1>() == "MUSIC") MusicData = match.get<2>().to_string();
	}
}
void ReadData(const std::string_view path) {
	std::string lvldat;
	LoadLvl(lvldat, path.data());
	std::stringstream input_view(lvldat);
	std::string line;
	int ReadMode = 0;
	// Read the file
	bool flag = false;
	while (std::getline(input_view, line)) {
		flag = false;
		for (auto match : ctre::search_all<"(\\[LVL\\]|\\[TILE\\])">(line)) {
			if (match.get<0>() == "[LVL]") {
				ReadMode = 0;
				flag = true;
			}
			else if (match.get<0>() == "[TILE]") {
				ReadMode = 1;
				flag = true;
			}
			/*
			else if (match.get<0>() == "[BONUS_DATA]") {
				ReadMode = 2;
				flag = true;
			}
			else if (match.get<0>() == "[NPC_DATA]") {
				ReadMode = 3;
				flag = true;
			}
			*/
		}
		if (flag) continue;

		switch (ReadMode) {
			case 0: LEVELDATAREAD(line); break;
			case 1: TILEDATAREAD(line); break;
			/*
			case 2: BONUSDATAREAD(line); break;
			case 3: NPCDATAREAD(line); break;
			*/
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
	if (!ObstacleRTexture.resize({static_cast<unsigned>(LevelWidth), static_cast<unsigned>(LevelHeight)}))
		throw std::runtime_error("Cannot resize Obstacles Texture");

	MFCPP::setTileMapSize(LevelWidth, LevelHeight);

	ObstacleRTexture.clear(sf::Color::Transparent);
	ObstaclesVA.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
	ObstaclesVA.resize(4);

	ObstaclesVA[0].position = sf::Vector2f(0.f, 0.f);
	ObstaclesVA[1].position = sf::Vector2f(LevelWidth, 0.f);
	ObstaclesVA[2].position = sf::Vector2f(0.f, LevelHeight);
	ObstaclesVA[3].position = sf::Vector2f(LevelWidth, LevelHeight);

	ObstaclesVA[0].texCoords = sf::Vector2f(0.f, 0.f);
	ObstaclesVA[1].texCoords = sf::Vector2f(LevelWidth, 0.f);
	ObstaclesVA[2].texCoords = sf::Vector2f(0.f, LevelHeight);
	ObstaclesVA[3].texCoords = sf::Vector2f(LevelWidth, LevelHeight);

	//ObstaclesVA.setPrimitiveType(sf::PrimitiveType::Triangles);
	//ObstaclesVA.resize(LevelData.size() * 6);
	for (int i = 0; i < LevelData.size(); ++i) {
		//for (const auto& i : LevelData) {
			// Find the tile id
		const int posTextureIndex = std::ranges::find_if(ID_list, [&i](const std::array<int, 6> &compare) {
			return compare[0] == static_cast<int>(LevelData[i][0]);
		}) - (ID_list.begin());
		sf::Sprite obstaclesRender(ImageManager::GetTexture(fmt::format("Tile_{}", posTextureIndex)));
		obstaclesRender.setPosition({ LevelData[i][1], LevelData[i][2] });
		MFCPP::setIndexTilemapCollision(LevelData[i][1], LevelData[i][2], true);
		MFCPP::setIndexTilemapID(LevelData[i][1], LevelData[i][2], ID_list[posTextureIndex][3]);
		MFCPP::setIndexTilemapFloorY(LevelData[i][1], LevelData[i][2], {ID_list[posTextureIndex][4], ID_list[posTextureIndex][5]});
		ObstacleRTexture.draw(obstaclesRender);
		// Then use the index of tile id property to add it to the list
		//ObstaclesList.emplace_back(Obstacles{ static_cast<int>(LevelData[i][0]), sf::Sprite(ImageManager::GetTexture("Tileset"), sf::IntRect({ID_list[posTextureIndex][1], ID_list[posTextureIndex][2] }, {32, 32})) });
		//ObstaclesList.push_back(Obstacles{ static_cast<int>(LevelData[i][0]), sf::Sprite(tempTex) });
		//ObstaclesList.back().property.setTexture(ImageManager::GetTexture("Tile_"+std::to_string(posTextureIndex)), true);
		//ObstaclesList.back().property.setPosition({ LevelData[i][1], LevelData[i][2] });
		//setHitbox(ObstaclesList[static_cast<int>(ObstaclesList.size()) - 1].hitbox, sf::FloatRect({ 0.f, 0.f }, { 32.f, 32.f }));

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
	ObstacleRTexture.display();
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
	MusicManager::StopAllMusic();
	MusicManager::SetLoop(MusicData, true);
	MusicManager::PlayMusic(MusicData);

	player.property.setPosition({ PlayerData[0], PlayerData[1] + 7.f });
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
	DeleteAllBroAIProjectile();
	DeleteAllMarioProjectile();
	DeleteAllBroAIEffect();
	DeleteAllFireballExplosion();
	//Delete Objects
	DeleteAllBrick();
	DeleteAllCoin();
	DeleteAllLuckyBlock();
	DeleteAllGoombaAI();
	DeleteAllBroAI();
	ClearPiranhaAI();
	DeleteAllSpike();
	DeleteAllBulletBill();
	BulletLauncherClear();
	//(Re)build Objects
	if (!BonusData.empty()) {
		for (const auto& i : BonusData) {
			if (i[0] == 1) {
				AddCoin(static_cast<CoinID>(static_cast<int>(i[1])), static_cast<CoinAtt>(static_cast<int>(i[2])), i[3], i[4]);
				MFCPP::setIndexCollectableMapID(i[3], i[4], 0);
				MFCPP::setIndexCollectableMapCollision(i[3], i[4], true);
			}
			else if (i[0] == 2) {
				MFCPP::setIndexTilemapCollision(i[3], i[4], true);
				MFCPP::setIndexTilemapID(i[3], i[4], 1);
				AddBrick(static_cast<BrickID>(static_cast<int>(i[1])), static_cast<BrickAtt>(static_cast<int>(i[2])), i[3], i[4]);
			}
			else if (i[0] == 3) {
				MFCPP::setIndexTilemapCollision(i[3], i[4], true);
				MFCPP::setIndexTilemapID(i[3], i[4], 2);
				AddLuckyBlock(static_cast<LuckyBlockID>(static_cast<int>(i[1])), static_cast<LuckyBlockAtt>(static_cast<int>(i[2])), i[3], i[4]);
			}
		}
	}
	if (!EnemyData.empty()) {
		for (const auto& i : EnemyData) {
			switch (static_cast<int>(i[0])) {
				case 0:
					AddGoombaAI(static_cast<GoombaAIType>(i[1]), static_cast<int>(i[2]), i[3], i[4], LEFT);
					break;
				case 1:
					AddPiranha(static_cast<PiranhaID>(i[1]), i[3], i[4]);
					break;
				case 2:
					AddSpike(static_cast<SpikeID>(i[1]), i[3], i[4]);
					break;
				case 3:
					AddBroAI(static_cast<BroAIType>(i[1]), static_cast<BroAIMovementType>(i[2]), i[3], i[4]);
					break;
				case 4:
					AddBulletLauncher(static_cast<BulletType>(i[1]), i[3], i[4]);
					break;
				default: ;
			}
		}
	}
}
std::string getMusicLevelName() {
	return MusicData;
}