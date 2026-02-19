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
#include "Core/Time.hpp"
#include "Object/BulletBillAI.hpp"
#include "Object/Mario.hpp"
#include "Object/Platform.hpp"
#include "Projectiles/PiranhaProjectile.hpp"
#include "Core/Checkpoint.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Enemy/RedRotodiscFlower.hpp"
#include "Object/Enemy/RedRotodiscRound.hpp"
// Level data
float LevelWidth, LevelHeight;
static std::vector<std::pair<std::string, sf::Vector2f>> BgData;
static std::vector<std::array<float, 3>> LevelData;
static std::vector<std::array<float, 5>> BonusData;
static std::vector<std::array<float, 5>> EnemyData;
static std::vector<sf::Vector2f> CheckpointData;
static std::vector<PlatformData> PlatformDataList;
static std::vector<RotodiscData> RotodiscDataList;
static sf::Vector2f ExitIndicator;
static sf::Vector2f ExitGate;
static sf::Vector2f PlayerData;
static std::string MusicData;
static MFCPP::Color BGFirstColor;
static MFCPP::Color BGSecondColor;
//Texture Gate
static std::set<std::string> ObstacleTexture;
static std::set<BrickID> BricksTexture;
static std::set<LuckyBlockID> LuckyBlockTexture;
static bool CoinTexture = false;
//Texture Loading
void ObstaclesTextureBuild() {
	MFCPP::Log::InfoPrint(fmt::format("Level: Load {} Obstacles Texture", ObstacleTexture.size()));
	for (const auto &i : ObstacleTexture) {
		ImageManager::LoadTexture(i);
	}
}
void BricksTextureBuild() {
	MFCPP::Log::InfoPrint(fmt::format("Level: Load {} Bricks Texture", BricksTexture.size()));
	for (const auto &i : BricksTexture) {
		ForceLoadBricksTexture(i);
	}
}
void LuckyBlockTextureBuild() {
	MFCPP::Log::InfoPrint(fmt::format("Level: Load {} LuckyBlock Texture", LuckyBlockTexture.size()));
	for (const auto &i : LuckyBlockTexture) {
		ForceLoadLuckyBlockTexture(i);
	}
}
void BackgroundTextureBuild() {
	MFCPP::Log::InfoPrint(fmt::format("Level: Load {} Background Texture", BgData.size()));
	for (const auto &key: BgData | std::views::keys) {
		ImageManager::LoadTexture(key);
	}
}
void CoinTextureBuild() {
	MFCPP::Log::InfoPrint("Build Coin Texture");
	if (CoinTexture) ForceLoadCoinTexture();
}
void PlatformDataProcess(const nlohmann::json& tileObj, const sf::Vector2f& pos, const int page, const int id) {
	sf::Vector2f endPos = tileObj.value("end_position", pos);
	if (endPos == sf::Vector2f(-1.f, -1.f)) endPos = pos;
	if (tileObj.contains("properties") && TilePage[page][id].prop.getPropertyCount() > 0) {
		CustomTileProperty custom_props = TilePage[page][id].prop;
		const nlohmann::json& propsJson = tileObj.at("properties");
		for (int i = 0; i < TilePage[page][id].prop.getPropertyCount(); ++i) {
			TileProperty* prop = custom_props.at(i);
			from_json(propsJson, *prop);
		}
		PlatformDataList.push_back({pos, endPos, custom_props});
	}
}
void RotodiscDataProcess(const nlohmann::json& tileObj, const sf::Vector2f& pos, const int page, const int id, const int objID) {
	if (tileObj.contains("properties") && TilePage[page][id].prop.getPropertyCount() > 0) {
		CustomTileProperty custom_props = TilePage[page][id].prop;
		const nlohmann::json& propsJson = tileObj.at("properties");
		for (int i = 0; i < TilePage[page][id].prop.getPropertyCount(); ++i) {
			TileProperty* prop = custom_props.at(i);
			from_json(propsJson, *prop);
		}
		RotodiscDataList.emplace_back(objID, pos, custom_props);
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
	setDefaultTime(levelJson["level_properties"].value("time", 360));
	PlayerData = levelJson.value("player_start", sf::Vector2f(128.f, 320.f));
	ExitGate = levelJson["exit_gate"].value("gate_pos", sf::Vector2f(384.f, 320.f));
	ExitIndicator = levelJson["exit_gate"].value("indicator_pos", sf::Vector2f(256, 320));

	BGFirstColor = levelJson["level_properties"].value("background_first_color", MFCPP::Color::LevelDefaultFirst);
	BGSecondColor = levelJson["level_properties"].value("background_second_color", MFCPP::Color::LevelDefaultSecond);
	MusicData = levelJson["level_properties"].value("music", "DansLaRue");
	const nlohmann::json& bgJson = levelJson["backgrounds"];
	for (const auto& bgObj : bgJson)
		BgData.emplace_back(bgObj.at("name").get<std::string>(), bgObj.value("parallax", sf::Vector2f(1.f, 1.f)));
	const nlohmann::json& tilesJson = levelJson["tiles"];
	for (const auto& tileObj : tilesJson) {
		const int page = tileObj.at("page").get<int>();
		const int id = tileObj.at("id").get<int>();
		const sf::Vector2f pos = tileObj.at("position").get<sf::Vector2f>();
		switch (const SelectTileData* ReadTile = &TilePage[page][id]; ReadTile->categoryID) {
			case 0:
				ObstacleTexture.insert(fmt::format("Tile_{}", ReadTile->objectID));
				LevelData.push_back({static_cast<float>(ReadTile->objectID), pos.x, pos.y});
				break;
			case 1:
				BonusData.push_back({static_cast<float>(ReadTile->objectID), static_cast<float>(ReadTile->customID1), static_cast<float>(ReadTile->customID2), pos.x, pos.y});
				switch (ReadTile->objectID) {
					case 1:
						CoinTexture = true;
						break;
					case 2:
						BricksTexture.insert(static_cast<BrickID>(ReadTile->customID1));
						break;
					case 3:
						LuckyBlockTexture.insert(static_cast<LuckyBlockID>(ReadTile->customID1));
						break;
					default: ;
				}
				break;
			case 2:
				if (ReadTile->objectID < 5)
					EnemyData.push_back({static_cast<float>(ReadTile->objectID), static_cast<float>(ReadTile->customID1), static_cast<float>(ReadTile->customID2), pos.x, pos.y});
				else
					RotodiscDataProcess(tileObj, pos, page, id, ReadTile->objectID);
				break;
			case 3:
				PlatformDataProcess(tileObj, pos, page, id);
				break;
			case 4:
				switch (ReadTile->objectID) {
				case 0:
					CheckpointData.emplace_back(sf::Vector2f(pos.x, pos.y));
					break;
				default: ;
				}
			default: ;
		}
	}
	setStartPosition(PlayerData + sf::Vector2f(0.f, 7.f));

	ObstaclesTextureBuild();
	BricksTextureBuild();
	LuckyBlockTextureBuild();
	BackgroundTextureBuild();
	CoinTextureBuild();
	MFCPP::Log::SuccessPrint(fmt::format("Successfully Loaded {}", path.string()));
}
void CheckpointBuilding() {
	for (const auto &i : CheckpointData) {
		AddCheckpoint(i);
	}
}
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

	for (auto & i : LevelData) {
		const int posTextureIndex = std::ranges::find_if(ID_list, [&i](const std::array<int, 6> &compare) {
			return compare[0] == static_cast<int>(i[0]);
		}) - (ID_list.begin());
		sf::Sprite obstaclesRender(ImageManager::GetTexture(fmt::format("Tile_{}", posTextureIndex)));
		obstaclesRender.setPosition({ i[1], i[2] });
		MFCPP::setIndexTilemapCollision(i[1], i[2], true);
		MFCPP::setIndexTilemapID(i[1], i[2], ID_list[posTextureIndex][3]);
		MFCPP::setIndexTilemapFloorY(i[1], i[2], {static_cast<float>(ID_list[posTextureIndex][4]), static_cast<float>(ID_list[posTextureIndex][5])});
		ObstacleRTexture.draw(obstaclesRender);
	}
	ObstacleRTexture.display();
}
void Bgbuilding() {
	BgGradientSetColor(BGFirstColor.ColorNormalize(), BGSecondColor.ColorNormalize());
	BgGradientInitPos(LevelWidth, LevelHeight);
	for (const auto& i : BgData) {
		AddBg(i.first, i.second);
	}
}
void ExitGateBuilding() {
	ExitGateBack.setCurrentPosition(ExitGate);
	ExitGateIndicator.setCurrentPosition(ExitIndicator);
	ExitGateFore.setCurrentPosition(sf::Vector2f(ExitGateBack.getCurrentPosition().x + 43.0f, ExitGateBack.getCurrentPosition().y - 250.0f));
	ExitGateFore.setPreviousPosition(ExitGateFore.getCurrentPosition());
	ExitGateFore.setInterpolatedPosition(ExitGateFore.getCurrentPosition());
}
void Objectbuilding() {
	std::ranges::sort(BonusData, [](const std::array<float, 5>& a, const std::array<float, 5>& b) {return a[3] < b[3]; });
	//Music

	/*
	MusicManager::StopAllMusic();
	MusicManager::SetLoop(MusicData, true);
	MusicManager::PlayMusic(MusicData);
	*/

	player.property.setPosition(getStartPosition());

	player.curr = player.prev = player.property.getPosition();
	MarioDirection = FirstMarioDirection;
	WindowSetView();
	updateView();
	//Delete Effects
	DeleteAllBrickParticle();
	DeleteAllCoinEffect();
	DeleteAllScoreEffect();
	DeleteAllBroAIProjectile();
	DeleteAllMarioProjectile();
	DeleteAllFireballExplosion();
	DeleteAllPiranhaAIProjectile();
	//Delete Objects
	DeleteAllBrick();
	DeleteAllCoin();
	DeleteAllLuckyBlock();
	DeleteAllSpike();
	BulletLauncherClear();
	//Delete Platform
	DeleteAllPlatform();

	GameScene::enemyManager.DeleteAll();
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
					AddPiranha(static_cast<PiranhaID>(i[1]), static_cast<PiranhaDirection>(i[2]), i[3], i[4]);
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
	if (!RotodiscDataList.empty()) {
		for (const auto& i : RotodiscDataList) {
			switch (i.data) {
				case 5:
					GameScene::enemyManager.addEnemy<RedRotodiscRound>(i.pos, i.props.getProperty<IntProps>("Position State")->val, i.props.getProperty<FloatProps>("Radius")->val, i.props.getProperty<IntProps>("Speed")->val, i.props.getProperty<FloatProps>("Angle")->val);
					break;
				case 6:
					GameScene::enemyManager.addEnemy<RedRotodiscFlower>(i.pos, i.props.getProperty<IntProps>("Position State")->val, i.props.getProperty<FloatProps>("Max Radius")->val, i.props.getProperty<IntProps>("Speed")->val, i.props.getProperty<FloatProps>("Radius Change Rate")->val, i.props.getProperty<FloatProps>("Angle")->val);
					break;
				default: ;
			}
		}
	}
	if (!PlatformDataList.empty()) {
		for (auto &i : PlatformDataList) {
			AddPlatform(i.start, i.end, i.props.getProperty<IntProps>("Speed")->val, i.props.getProperty<BoolProps>("is Smooth")->val, i.props.getProperty<BoolProps>("is Fall")->val, i.props.getProperty<BoolProps>("is Wait")->val);
		}
	}
}
std::string getMusicLevelName() {
	return MusicData;
}