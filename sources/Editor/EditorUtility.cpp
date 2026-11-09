#include "Editor/EditorUtility.hpp"
#include <nlohmann/json.hpp>

#include "Core/Background/BgGradient.hpp"
#include "Editor/Editor.hpp"
#include "Core/JsonUtils.hpp"

void MFCPP::EditorUtility::FileSave(const std::filesystem::path& path) {
    nlohmann::json levelJson;

    levelJson["level_properties"]["width"] = LevelSize.getProperty<FloatProps>("Level Width")->val;
    levelJson["level_properties"]["height"] = LevelSize.getProperty<FloatProps>("Level Height")->val;
    levelJson["level_properties"]["time"] = TimeSetting.getProperty<IntProps>("Time")->val;
    levelJson["level_properties"]["autoscroll"] = AutoScrollSetting.getProperty<BoolProps>("AutoScroll Mode")->val;
    levelJson["level_properties"]["tank_mode"] = AutoScrollSetting.getProperty<BoolProps>("Tank Mode")->val;
    levelJson["level_properties"]["autoscroll_speed"] = AutoScrollSetting.getProperty<FloatProps>("AutoScroll Speed")->val;
    levelJson["level_properties"]["enable_weather"] = WeatherSetting.getProperty<BoolProps>("Enable Weather")->val;
    levelJson["level_properties"]["weather_id"] = WeatherSetting.getProperty<IntProps>("Weather ID")->val;
    levelJson["level_properties"]["env_id"] = LevelEnvironmentEditor.getProperty<IntProps>("Environment ID")->val;
    levelJson["level_properties"]["music"] = LevelMusic.getProperty<StringProps>("Music Name")->val;
    levelJson["level_properties"]["background_first_color"] = BgColor.getProperty<ColorProps>("First Background Color")->val;
    levelJson["level_properties"]["background_second_color"] = BgColor.getProperty<ColorProps>("Second Background Color")->val;

    levelJson["player_start"] = sf::Vector2f(EDITOR_Mario.getPosition().x - EDITOR_Mario.getOrigin().x + TilePage[LevelTab][0].origin.x, EDITOR_Mario.getPosition().y - EDITOR_Mario.getOrigin().y + TilePage[LevelTab][0].origin.y);

    levelJson["exit_gate"]["indicator_pos"] = sf::Vector2f(EDITOR_ExitGateIndicator.getPosition().x - EDITOR_ExitGateIndicator.getOrigin().x + TilePage[LevelTab][1].origin.x, EDITOR_ExitGateIndicator.getPosition().y - EDITOR_ExitGateIndicator.getOrigin().y + TilePage[LevelTab][1].origin.y);
    levelJson["exit_gate"]["gate_pos"] = sf::Vector2f(EDITOR_ExitGate.getPosition().x - EDITOR_ExitGate.getOrigin().x + TilePage[LevelTab][2].origin.x, EDITOR_ExitGate.getPosition().y - EDITOR_ExitGate.getOrigin().y + TilePage[LevelTab][2].origin.y);
    nlohmann::json& bgJson = levelJson["backgrounds"];
    bgJson = nlohmann::json::array();
    for (const auto &bg : BackgroundLayers) {
        nlohmann::json bgObj;
        bgObj["name"] = bg.name;
        bgObj["parallax"] = bg.parallaxFactor;
        bgJson.push_back(bgObj);
    }
    nlohmann::json& tilesJson = levelJson["tiles"];
    tilesJson = nlohmann::json::array();
    for (const auto & tile: Tile) {
        nlohmann::json tileObj;
        tileObj["id"] = tile.getID();
        tileObj["page"] = tile.getPage();
        tileObj["position"] = tile.getPosition() - tile.getOrigin() + TilePage[tile.getPage()][tile.getID()].origin;
        if (tile.getEndPos() != sf::Vector2f(-1.f, -1.f))
            tileObj["end_position"] = tile.getEndPos();
        for (int i = 0; i < tile.getProperty().getPropertyCount(); ++i)
            to_json(tileObj["properties"], *tile.getProperty().at(i));
        tilesJson.push_back(tileObj);
    }
    std::ofstream fooJson(path.string());
    if (!fooJson.is_open()) {
        MFCPP::Log::ErrorPrint(fmt::format("Editor: Cannot Open {}", path.string()));
        return;
    }
    fooJson << levelJson.dump(4);
    fooJson.close();
    Log::SuccessPrint(fmt::format("Success Saved File {}", path.string()));
}

void MFCPP::EditorUtility::FileLoad(const std::filesystem::path& path) {
    std::ifstream foi(path.string());
    if (!foi.is_open()) {
        MFCPP::Log::ErrorPrint(fmt::format("Cannot open file {}", path.string()));
        return;
    }

    nlohmann::json levelJson;
    try {
        foi >> levelJson;
    }
    catch (nlohmann::json::parse_error& e) {
        MFCPP::Log::ErrorPrint(fmt::format("Failed to parse level file: {}", e.what()));
        return;
    }
    BackgroundLayers.clear();
    const nlohmann::json& bgJson = levelJson["backgrounds"];
    for (const auto& bgObj : bgJson) {
        const BackgroundLayer layer(bgObj.at("name").get<std::string>(), bgObj.value("parallax", sf::Vector2f(1.f, 1.f)));
        BackgroundLayers.push_back(layer);
    }
    Tile.clear();
    LevelSize.getProperty<FloatProps>("Level Width")->val = levelJson["level_properties"].value("width", 10016.f);
    LevelSize.getProperty<FloatProps>("Level Height")->val = levelJson["level_properties"].value("height", 480.f);
    TimeSetting.getProperty<IntProps>("Time")->val = levelJson["level_properties"].value("time", 360);
    AutoScrollSetting.getProperty<BoolProps>("AutoScroll Mode")->val = levelJson["level_properties"].value("autoscroll", false);
    AutoScrollSetting.getProperty<BoolProps>("Tank Mode")->val = levelJson["level_properties"].value("tank_mode", false);
    AutoScrollSetting.getProperty<FloatProps>("AutoScroll Speed")->val = levelJson["level_properties"].value("autoscroll_speed", 1.f);
    WeatherSetting.getProperty<BoolProps>("Enable Weather")->val = levelJson["level_properties"].value("enable_weather", false);
    WeatherSetting.getProperty<IntProps>("Weather ID")->val = levelJson["level_properties"].value("weather_id", 0);
    LevelEnvironmentEditor.getProperty<IntProps>("Environment ID")->val = levelJson["level_properties"].value("env_id", 0);
    //set value for music
    const std::string val = levelJson["level_properties"].value("music", "default");
    std::memset(LevelMusic.getProperty<StringProps>("Music Name")->val, 0, sizeof(LevelMusic.getProperty<StringProps>("Music Name")->val));
    val.copy(LevelMusic.getProperty<StringProps>("Music Name")->val, sizeof(LevelMusic.getProperty<StringProps>("Music Name")->val) - 1);
    LevelMusic.getProperty<StringProps>("Music Name")->val[val.length()] = '\0';
    PlayerData = levelJson.value("player_start", sf::Vector2f(128.f, 320.f));
    ExitGateData = levelJson["exit_gate"].value("gate_pos", sf::Vector2f(384, 320));
    ExitGateIndicatorData = levelJson["exit_gate"].value("indicator_pos", sf::Vector2f(256, 320));
    //Color
    BgColor.getProperty<ColorProps>("First Background Color")->val = levelJson["level_properties"].value("background_first_color",  MFCPP::Color::LevelDefaultFirst);
    BgColor.getProperty<ColorProps>("Second Background Color")->val = levelJson["level_properties"].value("background_second_color", MFCPP::Color::LevelDefaultSecond);
    BgGradientSetColor(BgColor.getProperty<ColorProps>("First Background Color")->val.ColorNormalize(), BgColor.getProperty<ColorProps>("Second Background Color")->val.ColorNormalize());
    BgGradientInitPos(LevelSize.getProperty<FloatProps>("Level Width")->val, LevelSize.getProperty<FloatProps>("Level Height")->val);
    //Music
    const nlohmann::json& tilesJson = levelJson["tiles"];
    for (const auto& tileObj : tilesJson) {
        const int page = tileObj.at("page").get<int>();
        const int id = tileObj.at("id").get<int>();
        const sf::Vector2f pos = tileObj.at("position").get<sf::Vector2f>();
        const sf::Vector2f endPos = tileObj.value("end_position", sf::Vector2f(-1.f, -1.f));

        const sf::Vector2f origin_tile(0.0f, static_cast<float>(ImageManager::getReturnTexture(TilePage[page][id].name)->getSize().y) - 32.f);
        RenderTile tile(TilePage[page][id].prop, *ImageManager::getReturnTexture(TilePage[page][id].name), pos - TilePage[page][id].origin + origin_tile, page, id, endPos);
        tile.setOrigin(origin_tile);
        if (tileObj.contains("properties")) {
            const nlohmann::json& propsJson = tileObj.at("properties");
            for (int i = 0; i < tile.getProperty().getPropertyCount(); ++i) {
                TileProperty* prop = tile.getProperty().at(i);
                from_json(propsJson, *prop);
            }
        }
        Tile.insert(tile);
    }
    MFCPP::Log::SuccessPrint(fmt::format("Success Loaded File {}", path.string()));
    lastPlaceX = -1.0f;
    lastPlaceY = -1.0f;
    lastDeleteX = -1.0f;
    lastDeleteY = -1.0f;
    EDITOR_Mario.setPosition(sf::Vector2f(PlayerData.x - TilePage[LevelTab][0].origin.x + EDITOR_Mario.getOrigin().x, PlayerData.y - TilePage[LevelTab][0].origin.y + EDITOR_Mario.getOrigin().y));
    EDITOR_ExitGateIndicator.setPosition(sf::Vector2f(ExitGateIndicatorData.x - TilePage[LevelTab][1].origin.x + EDITOR_ExitGateIndicator.getOrigin().x, ExitGateIndicatorData.y - TilePage[LevelTab][1].origin.y + EDITOR_ExitGateIndicator.getOrigin().y));
    EDITOR_ExitGate.setPosition(sf::Vector2f(ExitGateData.x - TilePage[LevelTab][2].origin.x + EDITOR_ExitGate.getOrigin().x, ExitGateData.y - TilePage[LevelTab][2].origin.y + EDITOR_ExitGate.getOrigin().y));
}