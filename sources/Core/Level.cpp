#include <nlohmann/json.hpp>
#include "Core/JsonUtils.hpp"
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
#include "Object/PiranhaAI.hpp"
#include "Object/Spike.hpp"
#include "Core/Scroll.hpp"
#include "Core/MusicManager.hpp"
#include "Core/Background/Bg.hpp"
#include "Object/ExitGate.hpp"
#include "Editor/SelectTile.hpp"
#include "Core/Background/BgGradient.hpp"
#include "Effect/BroAIEffect.hpp"
#include "Effect/FireballExplosion.hpp"
#include "Object/BroAI.hpp"
#include "Projectiles/BroAIProjectile.hpp"
#include "Projectiles/MarioProjectile.hpp"
#include "Core/Tilemap.hpp"
#include "Object/BulletBill.hpp"
#include "Object/Mario.hpp"
#include "Object/Platform.hpp"
// Level data
float LevelWidth, LevelHeight;
static std::vector<std::array<float, 2>> BgData;
static std::vector<std::array<float, 3>> LevelData;
static std::vector<std::array<float, 3>> SlopeData;
static std::vector<std::array<float, 5>> BonusData;
static std::vector<std::array<float, 5>> EnemyData;
static std::vector<PlatformData> PlatformDataList;
static sf::Vector2f ExitIndicator;
static sf::Vector2f ExitGate;
static sf::Vector2f PlayerData;
static std::string MusicData;
static MFCPP::Color BGFirstColor;
static MFCPP::Color BGSecondColor;
void PlatformDataProcess(const nlohmann::json& tileObj, const sf::Vector2f& pos, const int page, const int id) {
	sf::Vector2f endPos = tileObj.value("end_position", pos);
	if (endPos == sf::Vector2f(-1.f, -1.f)) endPos = pos;
	if (tileObj.contains("properties") && TilePage[page][id].prop.getPropertyCount() > 0) {
		CustomTileProperty custom_props = TilePage[page][id].prop;
		const nlohmann::json& propsJson = tileObj.at("properties");
		for (int i = 0; i < TilePage[page][id].prop.getPropertyCount() > 0; ++i) {
			TileProperty* prop = custom_props.at(i);
			from_json(propsJson, *prop);
		}
		PlatformDataList.push_back({pos, endPos, custom_props});
	}
}
void ReadData(const std::filesystem::path& path) {
	std::string LevelDataText;
	LoadLvl(LevelDataText, path.string());

	nlohmann::json levelJson;
	try {
		levelJson = nlohmann::json::parse(LevelDataText);
	}
	catch (nlohmann::json::parse_error& e) {
		throw std::runtime_error(fmt::format("Level: Failed to parse level file: {}", e.what()));
	}
	LevelWidth = levelJson["level_properties"].value("width", 10016.f);
	LevelHeight = levelJson["level_properties"].value("height", 480.f);
	PlayerData = levelJson.value("player_start", sf::Vector2f(128.f, 320.f));
	ExitGate = levelJson["exit_gate"].value("gate_pos", sf::Vector2f(384.f, 320.f));
	ExitIndicator = levelJson["exit_gate"].value("indicator_pos", sf::Vector2f(256, 320));

	BGFirstColor = levelJson["level_properties"].value("background_first_color", MFCPP::Color::LevelDefaultFirst);
	BGSecondColor = levelJson["level_properties"].value("background_second_color", MFCPP::Color::LevelDefaultSecond);
	MusicData = levelJson["level_properties"].value("music", "DansLaRue");

	const nlohmann::json& tilesJson = levelJson["tiles"];
	for (const auto& tileObj : tilesJson) {
		const int page = tileObj.at("page").get<int>();
		const int id = tileObj.at("id").get<int>();
		const sf::Vector2f pos = tileObj.at("position").get<sf::Vector2f>();
		switch (const SelectTileData* ReadTile = &TilePage[page][id]; ReadTile->categoryID) {
			case 0:
				LevelData.push_back({static_cast<float>(ReadTile->objectID), pos.x, pos.y});
				break;
			case 1:
				BonusData.push_back({static_cast<float>(ReadTile->objectID), static_cast<float>(ReadTile->customID1), static_cast<float>(ReadTile->customID2), pos.x, pos.y});
				break;
			case 2:
				EnemyData.push_back({static_cast<float>(ReadTile->objectID), static_cast<float>(ReadTile->customID1), static_cast<float>(ReadTile->customID2), pos.x, pos.y});
				break;
			case 3:
				PlatformDataProcess(tileObj, pos, page, id);
				break;
			default: ;
		}
	}
	MFCPP::Log::SuccessPrint(fmt::format("Successfully Loaded {}", path.string()));
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
		MFCPP::setIndexTilemapFloorY(LevelData[i][1], LevelData[i][2], {static_cast<float>(ID_list[posTextureIndex][4]), static_cast<float>(ID_list[posTextureIndex][5])});
		ObstacleRTexture.draw(obstaclesRender);
	}
	ObstacleRTexture.display();
}
void Bgbuilding() {
	BgGradientSetColor(BGFirstColor.ColorNormalize(), BGSecondColor.ColorNormalize());
	BgGradientInitPos(LevelWidth, LevelHeight);
	for (const auto& i : BgData) {
		AddBg(static_cast<int>(i[0]), static_cast<int>(i[1]));
	}
}
void ExitGateBuilding() {
	ExitGateBack.setPosition(ExitGate);
	ExitGateIndicator.setPosition(ExitIndicator);
	ExitGateFore.setPosition({ ExitGateBack.getPosition().x + 43.0f, ExitGateBack.getPosition().y - 250.0f });
	ExitGateForeCurr = ExitGateForePrev = ExitGateFore.getPosition();
}
void Objectbuilding() {
	std::ranges::sort(BonusData, [](const std::array<float, 5>& a, const std::array<float, 5>& b) {return a[3] < b[3]; });
	//Music
	MusicManager::StopAllMusic();
	MusicManager::SetLoop(MusicData, true);
	MusicManager::PlayMusic(MusicData);

	player.property.setPosition(PlayerData + sf::Vector2f( 0.f, 7.f ));
	player.curr = player.prev = player.property.getPosition();
	MarioDirection = FirstMarioDirection;
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
	//Delete Platform
	DeleteAllPlatform();
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
	if (!PlatformDataList.empty()) {
		for (auto &i : PlatformDataList) {
			AddPlatform(i.start, i.end, i.props.getProperty<IntProps>("Speed")->val, i.props.getProperty<BoolProps>("isSmooth")->val, i.props.getProperty<BoolProps>("isFall")->val, i.props.getProperty<BoolProps>("isWait")->val);
		}
	}
}
std::string getMusicLevelName() {
	return MusicData;
}