#include <nlohmann/json.hpp>
#include "Editor/Editor.hpp"

#include "config.h"
#include <imgui.h>
#include "Editor/SelectTile.hpp"
#include "Editor/TabButton.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Hash.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Logging.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Scroll.hpp"
#include "Core/SimpleSprite.hpp"
#include "Core/JsonUtils.hpp"

std::string fileOutPut = "lvl-test2.json";
std::string fileInput = "lvl-test2.json";

static float lastPlaceX = -1.0f;
static float lastPlaceY = -1.0f;
static float lastDeleteX = -1.0f;
static float lastDeleteY = -1.0f;

// Editor Position
sf::Vector2f EditorPos;
sf::Vector2f EditorPrevPos;
sf::Vector2f EditorInterpolatedPos;

//Select Box
static sf::Sprite SelectBox(tempTex);
static float SelectBoxAlpha = 100.0f;
static bool SelectBoxAlphaState = true;
constexpr float SELECTBOXALPHA_MAX = 160.0f;
constexpr float SELECTBOXALPHA_MIN = 100.0f;
constexpr float SELECTBOXALPHA_CHANGE = 1.0f;

// Grid
static sf::VertexArray Grid(sf::PrimitiveType::TriangleStrip, 4);
static float GridAlpha = 0.0f;
static bool GridAlphaState = true;
constexpr float GRIDALPHA_MAX = 255.0f;
constexpr float GRIDALPHA_MIN = 0.0f;
constexpr float GRIDALPHA_CHANGE = 1.0f;
//Level Data
static float TEST_LevelWidth = 10016.0f;
static float TEST_LevelHeight = 480.0f;
//Selected Block HUD
static sf::Sprite SelectedBlock(tempTex);

//Tile
std::unordered_set<RenderTile, RenderTileHash, RenderTileEqual> Tile;
static float TileX;
static float TileY;

// NOTE: Object below only have 1 at a time
// Mario
static sf::Sprite EDITOR_Mario(tempTex); // 128, 320
static sf::Sprite EDITOR_ExitGateIndicator(tempTex); // 256, 320
static sf::Sprite EDITOR_ExitGate(tempTex); // 384, 320

//Read file purpose only
static sf::Vector2f ExitGateData{};
static sf::Vector2f ExitGateIndicatorData{};
static sf::Vector2f PlayerData{};

//Editor Can Place
static bool EDITOR_CanPlace = true;
static sf::Vector2f EDITOR_SavePos = sf::Vector2f(0.f, 0.f);
static bool EDITOR_isLeftHolding = false;
static bool EDITOR_isRightHolding = false;

//Edit Property
static bool showEditProperty = false;

//Build Mode
static bool EDITOR_BuildMode = ON;

//
static RenderTile SaveTile;

void SetPrevEditor() {
    EditorPrevPos = EditorPos;
}
void InterpolateEditorPos(const float alpha) {
    EditorInterpolatedPos = linearInterpolation(EditorPrevPos, EditorPos, alpha);
}

void AlphaUpdate(float& alpha, bool& state, const float min, const float max, const float change, const float dt) {
    if (state) {
        if (alpha < max) alpha += change * dt;
        if (alpha >= max) {
            alpha = max;
            state = false;
        }
    }
    else {
        if (alpha > min) alpha -= change * dt;
        if (alpha <= min) {
            alpha = min;
            state = true;
        }
    }
}

void EditorInit() {
    ImageManager::AddTexture("EDITOR_SelectBox", "data/resources/Editor/EDITOR_SelectBox.png");
    ImageManager::AddTexture("EDITOR_Grid", "data/resources/Editor/EDITOR_Grid.png", {}, true);

    ImageManager::AddTexture("EDITOR_MushroomLuckyblock", "data/resources/Editor/EDITOR_SELECT_TILE_EXCLUSIVE/EDITOR_Mushroom_Luckyblock.png");
    ImageManager::AddTexture( "EDITOR_CoinLuckyblock", "data/resources/Editor/EDITOR_SELECT_TILE_EXCLUSIVE/EDITOR_Coin_Luckyblock.png");
    ImageManager::AddTexture( "EDITOR_CoinBrick", "data/resources/Editor/EDITOR_SELECT_TILE_EXCLUSIVE/EDITOR_Coin_Brick.png");
    ImageManager::AddTexture("EDITOR_HammerBroCanMove", "data/resources/Editor/EDITOR_SELECT_TILE_EXCLUSIVE/EDITOR_Hammer_CanMove.png");
    ImageManager::AddTexture("EDITOR_HammerBroCannotMove", "data/resources/Editor/EDITOR_SELECT_TILE_EXCLUSIVE/EDITOR_Hammer_CannotMove.png");
    ImageManager::AddTexture( "EDITOR_FireFlowerLuckyblock", "data/resources/Editor/EDITOR_SELECT_TILE_EXCLUSIVE/EDITOR_FireFlower_Luckyblock.png");

    SelectBox.setTexture(ImageManager::GetTexture("EDITOR_SelectBox"), true);
    if (ImageManager::GetReturnTexture(TilePage[LevelTab][0].name) == nullptr) throw std::runtime_error("NULLPTR");
    EDITOR_Mario.setPosition(sf::Vector2f(128, 320));
    EDITOR_Mario.setTexture(*ImageManager::GetReturnTexture(TilePage[LevelTab][0].name), true);
    EDITOR_Mario.setOrigin(sf::Vector2f(0.0f, ImageManager::GetReturnTexture(TilePage[LevelTab][0].name)->getSize().y - 32.0f));

    EDITOR_ExitGateIndicator.setPosition(sf::Vector2f(256, 320));
    EDITOR_ExitGateIndicator.setTexture(*ImageManager::GetReturnTexture(TilePage[LevelTab][1].name), true);
    EDITOR_ExitGateIndicator.setOrigin(sf::Vector2f(0.0f, ImageManager::GetReturnTexture(TilePage[LevelTab][1].name)->getSize().y - 32.0f));

    EDITOR_ExitGate.setPosition(sf::Vector2f(384, 320));
    EDITOR_ExitGate.setTexture(*ImageManager::GetReturnTexture(TilePage[LevelTab][2].name), true);
    EDITOR_ExitGate.setOrigin(sf::Vector2f(0.0f, ImageManager::GetReturnTexture(TilePage[LevelTab][2].name)->getSize().y - 32.0f));
}
void EditPropertyDialog() {
    if (showEditProperty) {
        ImGui::Begin("Edit Property", &showEditProperty, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
        //ImGui::SetWindowSize("Edit Property", ImVec2(256.f, 256.f));
        ImGui::SetWindowPos(ImVec2(window.getSize().x / 2.f - ImGui::GetWindowSize().x / 2.f, window.getSize().y / 2.f - ImGui::GetWindowSize().y / 2.f));
        if (ImGui::BeginTabBar("EditPropertyTab")) {
            if (ImGui::BeginTabItem("Property")) {
                for (int i = 0; i < SaveTile.getProperty().getPropertyCount(); ++i) {
                    TileProperty* property = SaveTile.getProperty().at(i);
                    if (std::holds_alternative<BoolProps>(*property))
                        ImGui::Checkbox(std::get<BoolProps>(*property).name.c_str(), &std::get<BoolProps>(*property).val);
                    else if (std::holds_alternative<IntProps>(*property)) {
                        ImGui::PushItemWidth(125);
                        ImGui::InputInt(std::get<IntProps>(*property).name.c_str(), &std::get<IntProps>(*property).val, 1, 100, ImGuiInputTextFlags_EscapeClearsAll);
                        ImGui::PopItemWidth();
                        if (std::get<IntProps>(*property).val > std::get<IntProps>(*property).max) std::get<IntProps>(*property).val = std::get<IntProps>(*property).max;
                        else if (std::get<IntProps>(*property).val < std::get<IntProps>(*property).min) std::get<IntProps>(*property).val = std::get<IntProps>(*property).min;
                    }
                    else if (std::holds_alternative<StringProps>(*property)) {
                        ImGui::InputText(std::get<StringProps>(*property).name.c_str(), std::get<StringProps>(*property).val, IM_ARRAYSIZE(std::get<StringProps>(*property).val), ImGuiInputTextFlags_EscapeClearsAll);
                    }
                    else if (std::holds_alternative<FloatProps>(*property)) {
                        ImGui::PushItemWidth(125);
                        ImGui::InputFloat(std::get<FloatProps>(*property).name.c_str(), &std::get<FloatProps>(*property).val, 1, 100, "%.2f", ImGuiInputTextFlags_EscapeClearsAll);
                        ImGui::PopItemWidth();
                        if (std::get<FloatProps>(*property).val > std::get<FloatProps>(*property).max) std::get<FloatProps>(*property).val = std::get<FloatProps>(*property).max;
                        else if (std::get<FloatProps>(*property).val < std::get<FloatProps>(*property).min) std::get<FloatProps>(*property).val = std::get<FloatProps>(*property).min;
                    }
                    else if (std::holds_alternative<Vector2fProps>(*property)) {
                        ImGui::PushItemWidth(125);
                        ImGui::InputFloat((std::get<Vector2fProps>(*property).name+".x").c_str(), &std::get<Vector2fProps>(*property).val.x, 0, 0, "%.2f", ImGuiInputTextFlags_EscapeClearsAll);
                        ImGui::InputFloat((std::get<Vector2fProps>(*property).name+".y").c_str(), &std::get<Vector2fProps>(*property).val.y, 0, 0, "%.2f", ImGuiInputTextFlags_EscapeClearsAll);
                        ImGui::PopItemWidth();
                    }
                }
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        //ImGui::Checkbox("isSmooth", &smooth);
        //ImGui::Checkbox("isFall", &fall);
        //ImGui::Checkbox("isWait", &wait);
        if (ImGui::Button("Confirm")) {
            Tile.erase(SaveTile);
            Tile.insert(SaveTile);
            showEditProperty = false;
        }
        ImGui::End();
    }
}

void FileSave() {
    nlohmann::json levelJson;

    levelJson["level_properties"]["width"] = TEST_LevelWidth;
    levelJson["level_properties"]["height"] = TEST_LevelHeight;
    //TODO: Add Custom Music
    levelJson["level_properties"]["music"] = "DansLaRue";
    //TODO: Add Custom Color
    levelJson["level_properties"]["background_first_color"] = "7495f5";
    levelJson["level_properties"]["background_second_color"] = "f5fefd";

    levelJson["player_start"] = sf::Vector2f(EDITOR_Mario.getPosition().x - EDITOR_Mario.getOrigin().x + TilePage[LevelTab][0].origin.x, EDITOR_Mario.getPosition().y - EDITOR_Mario.getOrigin().y + TilePage[LevelTab][0].origin.y);

    levelJson["exit_gate"]["indicator_pos"] = sf::Vector2f(EDITOR_ExitGateIndicator.getPosition().x - EDITOR_ExitGateIndicator.getOrigin().x + TilePage[LevelTab][1].origin.x, EDITOR_ExitGateIndicator.getPosition().y - EDITOR_ExitGateIndicator.getOrigin().y + TilePage[LevelTab][1].origin.y);
    levelJson["exit_gate"]["gate_pos"] = sf::Vector2f(EDITOR_ExitGate.getPosition().x - EDITOR_ExitGate.getOrigin().x + TilePage[LevelTab][2].origin.x, EDITOR_ExitGate.getPosition().y - EDITOR_ExitGate.getOrigin().y + TilePage[LevelTab][2].origin.y);

    nlohmann::json& tilesJson = levelJson["tiles"];
    tilesJson = nlohmann::json::array();
    for (const auto & tile: Tile) {
        nlohmann::json tileObj;
        tileObj["id"] = tile.getID();
        tileObj["page"] = tile.getPage();
        tileObj["position"] = tile.getPosition();
        if (tile.getEndPos() != sf::Vector2f(-1.f, -1.f))
            tileObj["end_position"] = tile.getEndPos();
        for (int i = 0; i < tile.getProperty().getPropertyCount(); ++i)
            to_json(tileObj["properties"], *tile.getProperty().at(i));
        tilesJson.push_back(tileObj);
    }
    std::ofstream fooJson(fileOutPut);
    if (!fooJson.is_open()) {
        MFCPP::Log::ErrorPrint(fmt::format("Editor: Cannot Open {}", fileOutPut));
        return;
    }
    fooJson << levelJson.dump(4);
    fooJson.close();
    MFCPP::Log::SuccessPrint(fmt::format("Success Saved File {}", fileOutPut));
}

void FileLoad() {
    std::ifstream foi(fileInput);
    if (!foi.is_open()) {
        MFCPP::Log::ErrorPrint(fmt::format("Cannot open file {}", fileInput));
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

    Tile.clear();

    TEST_LevelWidth = levelJson["level_properties"].value("width", 10016.f);
    TEST_LevelHeight = levelJson["level_properties"].value("height", 480.f);
    PlayerData = levelJson.value("player_start", sf::Vector2f(128.f, 320.f));
    ExitGateData = levelJson["exit_gate"].value("gate_pos", sf::Vector2f(384, 320));
    ExitGateIndicatorData = levelJson["exit_gate"].value("indicator_pos", sf::Vector2f(256, 320));
    //Color
    //Music

    const nlohmann::json& tilesJson = levelJson["tiles"];
    for (const auto& tileObj : tilesJson) {
        const int page = tileObj.at("page").get<int>();
        const int id = tileObj.at("id").get<int>();
        const sf::Vector2f pos = tileObj.at("position").get<sf::Vector2f>();
        const sf::Vector2f endPos = tileObj.value("end_position", sf::Vector2f(-1.f, -1.f));

        RenderTile tile(TilePage[page][id].prop, *ImageManager::GetReturnTexture(TilePage[page][id].name), pos, page, id, endPos);

        if (tileObj.contains("properties")) {
            const nlohmann::json& propsJson = tileObj.at("properties");
            for (int i = 0; i < tile.getProperty().getPropertyCount(); ++i) {
                TileProperty* prop = tile.getProperty().at(i);
                from_json(propsJson, *prop);
            }
        }
        const sf::Vector2f origin_tile(0.0f, static_cast<float>(tile.getTexture()->getSize().y) - 32.0f);
        tile.setOrigin(origin_tile);
        Tile.insert(tile);
    }
    MFCPP::Log::SuccessPrint(fmt::format("Success Loaded File {}", fileInput));
    lastPlaceX = -1.0f;
    lastPlaceY = -1.0f;
    lastDeleteX = -1.0f;
    lastDeleteY = -1.0f;
    EDITOR_Mario.setPosition(sf::Vector2f(PlayerData.x - TilePage[LevelTab][0].origin.x + EDITOR_Mario.getOrigin().x, PlayerData.y - TilePage[LevelTab][0].origin.y + EDITOR_Mario.getOrigin().y));
    EDITOR_ExitGateIndicator.setPosition(sf::Vector2f(ExitGateIndicatorData.x - TilePage[LevelTab][1].origin.x + EDITOR_ExitGateIndicator.getOrigin().x, ExitGateIndicatorData.y - TilePage[LevelTab][1].origin.y + EDITOR_ExitGateIndicator.getOrigin().y));
    EDITOR_ExitGate.setPosition(sf::Vector2f(ExitGateData.x - TilePage[LevelTab][2].origin.x + EDITOR_ExitGate.getOrigin().x, ExitGateData.y - TilePage[LevelTab][2].origin.y + EDITOR_ExitGate.getOrigin().y));
}
void IncreaseTile() {
    CurrSelectTile = (CurrSelectTile + 1) % static_cast<int>(TilePage[CurrPage].size());
}
void DecreaseTile() {
    CurrSelectTile = CurrSelectTile - 1 < 0 ? static_cast<int>(TilePage[CurrPage].size()) - 1 : CurrSelectTile - 1;
}
void SelectedTilePosUpdate() {
    SelectedBlock.setPosition(sf::Vector2f(EditorInterpolatedPos.x + 16, EditorInterpolatedPos.y + 16));

    Grid[0].texCoords = sf::Vector2f(EditorInterpolatedPos.x, EditorInterpolatedPos.y);
    Grid[1].texCoords = sf::Vector2f(EditorInterpolatedPos.x + 640.0f, EditorInterpolatedPos.y);
    Grid[2].texCoords = sf::Vector2f(EditorInterpolatedPos.x, EditorInterpolatedPos.y + 640.0f);
    Grid[3].texCoords = sf::Vector2f(EditorInterpolatedPos.x + 640.0f, EditorInterpolatedPos.y + 640.0f);

    Grid[0].position = sf::Vector2f(EditorInterpolatedPos.x, EditorInterpolatedPos.y);
    Grid[1].position = sf::Vector2f(EditorInterpolatedPos.x + 640.0f, EditorInterpolatedPos.y);
    Grid[2].position = sf::Vector2f(EditorInterpolatedPos.x, EditorInterpolatedPos.y + 640.0f);
    Grid[3].position = sf::Vector2f(EditorInterpolatedPos.x + 640.0f, EditorInterpolatedPos.y + 640.0f);
}
void TilePosUpdate(const float dt) {
    TileX = std::floor((std::max(std::min(MouseX, Width - 32.0f), 0.0f) + EditorPos.x < 0 ? 0 : std::max(std::min(MouseX, Width - 32.0f), 0.0f) + EditorPos.x) / 32.0f) * 32.0f;
    TileY = std::floor((std::max(std::min(MouseY, Height - 32.0f), 0.0f) + EditorPos.y < 0 ? 0 : std::max(std::min(MouseY, Height - 32.0f), 0.0f) + EditorPos.y) / 32.0f) * 32.0f;

    AlphaUpdate(SelectBoxAlpha, SelectBoxAlphaState, SELECTBOXALPHA_MIN, SELECTBOXALPHA_MAX, SELECTBOXALPHA_CHANGE, dt);
    AlphaUpdate(GridAlpha, GridAlphaState, GRIDALPHA_MIN, GRIDALPHA_MAX, GRIDALPHA_CHANGE, dt);

    if (PrevSelectTile != CurrSelectTile || PrevPage != CurrPage) {
        SelectedBlock.setTexture(ImageManager::GetTexture(TilePage[CurrPage][CurrSelectTile].name), true);
        SelectedBlock.setTextureRect(sf::IntRect(TilePage[CurrPage][CurrSelectTile].texPos, {std::min(static_cast<int>(SelectedBlock.getGlobalBounds().size.x - TilePage[CurrPage][CurrSelectTile].texPos.x), 32), std::min(static_cast<int>(SelectedBlock.getGlobalBounds().size.y - TilePage[CurrPage][CurrSelectTile].texPos.y), 32)}));
        PrevSelectTile = CurrSelectTile;
        PrevPage = CurrPage;
    }
    SelectBox.setColor(sf::Color(255, 255, 255, static_cast<int>(SelectBoxAlpha)));
    Grid[0].color = sf::Color(255, 255, 255, static_cast<int>(GridAlpha));
    Grid[1].color = sf::Color(255, 255, 255, static_cast<int>(GridAlpha));
    Grid[2].color = sf::Color(255, 255, 255, static_cast<int>(GridAlpha));
    Grid[3].color = sf::Color(255, 255, 255, static_cast<int>(GridAlpha));
    SelectBox.setPosition(sf::Vector2f(TileX, TileY));
}
void EditorEvent(const std::optional<sf::Event>& event) {
    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        switch (keyPressed->code) {
            case sf::Keyboard::Key::Num2:
                if (!EDITOR_SELECTTILE) {
                    IncreaseTile();
                    SoundManager::PlaySound("EDITOR_SWITCH");
                }
                break;
            case sf::Keyboard::Key::Num1:
                if (!EDITOR_SELECTTILE) {
                    DecreaseTile();
                    SoundManager::PlaySound("EDITOR_SWITCH");
                }
                break;
            case sf::Keyboard::Key::Space:
                if (!showEditProperty && EDITOR_BuildMode) {
                    SoundManager::PlaySound("EDITOR_MENU");
                    EDITOR_SELECTTILE = !EDITOR_SELECTTILE;
                    if (EDITOR_SELECTTILE) PreviewPage = CurrPage;
                    SelectTileDisplayUpdate();
                }
                break;
            case sf::Keyboard::Key::S:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
                    FileSave();
                }
                break;
            case sf::Keyboard::Key::D:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
                    FileLoad();
                }
                break;
            case sf::Keyboard::Key::B:
                if (!showEditProperty)
                    EDITOR_BuildMode = !EDITOR_BuildMode;
                break;
            default: ;
        }
    }
    else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
        switch (mousePressed->button) {
            case sf::Mouse::Button::Left:
                if (EDITOR_SELECTTILE) {
                    bool isClickedTab = false;
                    for (int i = 0; i < TabList.size(); ++i) {
                        if (TabList[i].isMouseHovered(EditorInterpolatedPos, sf::Vector2f(MouseX, MouseY))) {
                            PreviewPage = i;
                            isClickedTab = true;
                            break;
                        }
                    }
                    if (isClickedTab) {
                        SelectTileDisplayUpdate();
                        SoundManager::PlaySound("EDITOR_TAB_SELECT");
                    }
                }
                if (const int exist = CheckExistPos(); EDITOR_SELECTTILE && exist != -1) {
                    SoundManager::PlaySound("EDITOR_CLOSE");
                    CurrSelectTile = exist;

                    CurrPage = PreviewPage;
                    EDITOR_SELECTILE_CLOCK.restart();
                    EDITOR_SELECTTILE = false;
                }
                break;
            default: ;
        }
    }
    else if (const auto* mouse = event->getIf<sf::Event::MouseWheelScrolled>()) {
        if (mouse->delta < 0) {
            IncreaseTile();
            SoundManager::PlaySound("EDITOR_SWITCH");
        }
        else if (mouse->delta > 0) {
            DecreaseTile();
            SoundManager::PlaySound("EDITOR_SWITCH");
        }
    }
}
void PlaceTile() {
    if (EDITOR_SELECTTILE) return;
    if (EDITOR_SELECTILE_CLOCK.getElapsedTime().asSeconds() < 0.15f) return;

    if (showEditProperty) return;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && window.hasFocus()) {
        if ((lastDeleteX != TileX || lastDeleteY != TileY) || (lastPlaceX == TileX && lastPlaceY == TileY)) {
            if (EDITOR_BuildMode) {
                if (CurrPage != LevelTab) {
                    if (Tile.contains(sf::Vector2f(TileX, TileY))) {
                        //std::cout << "Found!\n";
                        SoundManager::PlaySound("EDITOR_DELETE");
                        Tile.erase(sf::Vector2f(TileX, TileY));
                        if (!EDITOR_CanPlace) EDITOR_CanPlace = true;
                        lastDeleteX = TileX;
                        lastDeleteY = TileY;
                        lastPlaceX = -1.0f;
                        lastPlaceY = -1.0f;
                    }
                }
            }
            else {
                if (!EDITOR_isRightHolding) {
                    //if (SelectTile)
                    if (const auto it = Tile.find(sf::Vector2f(TileX, TileY)); it != Tile.end()) {
                        if (it->getProperty().getPropertyCount() > 0) {
                            showEditProperty = true;
                            SaveTile = *it;
                        }
                        //it_save = it->getProperty();
                        //MFCPP::Log::WarningPrint(fmt::format("PageID: {} {}", it->getPage(), it->getID()));
                        //MFCPP::Log::WarningPrint(fmt::format("EndPos: {} {}", it->getEndPos().x, it->getEndPos().y));
                    }
                    EDITOR_isRightHolding = true;
                }
            }
            //else std::cout << "Not Found!\n";
        }
    }
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && window.hasFocus()) {
        if (!EDITOR_BuildMode) return;
        if ((lastPlaceX != TileX || lastPlaceY != TileY) || (lastDeleteX == TileX && lastDeleteY == TileY)) {
            if (CurrPage != LevelTab) {
                if (TileX == EDITOR_Mario.getPosition().x && TileY == EDITOR_Mario.getPosition().y) return;
                if (TileX == EDITOR_ExitGateIndicator.getPosition().x && TileY == EDITOR_ExitGateIndicator.getPosition().y) return;

                if (EDITOR_isLeftHolding) return;
                if (!EDITOR_CanPlace) {
                    if (CurrPage == PlatformTab) {
                        if (CurrSelectTile == 0) {
                            Tile.erase(EDITOR_SavePos);

                            RenderTile tile(TilePage[CurrPage][CurrSelectTile].prop, *ImageManager::GetReturnTexture(TilePage[CurrPage][CurrSelectTile].name), EDITOR_SavePos, CurrPage, CurrSelectTile, sf::Vector2f(TileX, TileY));
                            tile.setOrigin(sf::Vector2f(0.0f, ImageManager::GetReturnTexture(TilePage[CurrPage][CurrSelectTile].name)->getSize().y - 32.0f));
                            Tile.insert(tile);
                            EDITOR_SavePos = sf::Vector2f(0.f, 0.f);
                            MFCPP::Log::InfoPrint(fmt::format("Reformated..."));

                            EDITOR_CanPlace = true;
                        }
                    }
                    EDITOR_isLeftHolding = true;
                    return;
                }

                if (!Tile.contains(sf::Vector2f(TileX, TileY))) {
                    //std::cout << "Placed\n";
                    SoundManager::PlaySound("EDITOR_PLACE");
                    RenderTile tile(TilePage[CurrPage][CurrSelectTile].prop, *ImageManager::GetReturnTexture(TilePage[CurrPage][CurrSelectTile].name), sf::Vector2f(TileX, TileY), CurrPage, CurrSelectTile);
                    tile.setOrigin(sf::Vector2f(0.0f, ImageManager::GetReturnTexture(TilePage[CurrPage][CurrSelectTile].name)->getSize().y - 32.0f));
                    Tile.insert(tile);
                    if (CurrPage == PlatformTab) {
                        switch (CurrSelectTile) {
                            case 0:
                                EDITOR_SavePos = sf::Vector2f(TileX, TileY);
                                EDITOR_CanPlace = false;
                                break;
                            default: ;
                        }
                    }
                }
            }
            else {
                switch (CurrSelectTile) {
                    case 0:
                        EDITOR_Mario.setPosition(sf::Vector2f(TileX, TileY));
                        break;
                    case 1:
                        EDITOR_ExitGateIndicator.setPosition(sf::Vector2f(TileX, TileY));
                        break;
                    case 2:
                        EDITOR_ExitGate.setPosition(sf::Vector2f(TileX, TileY));
                    default: ;
                }
            }
            //else std::cout << "Already Placed\n";
            lastPlaceX = TileX;
            lastPlaceY = TileY;
            lastDeleteX = -1.0f;
            lastDeleteY = -1.0f;
        }
    }

    if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (EDITOR_isLeftHolding) EDITOR_isLeftHolding = false;
    }
    if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
        if (EDITOR_isRightHolding) EDITOR_isRightHolding = false;
    }
}
void EditorScreenMove(const float dt) {
    if (!EDITOR_SELECTTILE) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) EditorPos.x += 8.0f * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) EditorPos.x -= 8.0f * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) EditorPos.y -= 8.0f * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) EditorPos.y += 8.0f * dt;
        if (EditorPos.x <= 0.0f) EditorPos.x = 0.0f;
        else if (EditorPos.x >= TEST_LevelWidth - Width) EditorPos.x = TEST_LevelWidth - Width;
        if (EditorPos.y <= 0.0f) EditorPos.y = 0.0f;
        else if (EditorPos.y >= TEST_LevelHeight - Height) EditorPos.y = TEST_LevelHeight - Height;
    }
}

void DrawTile() {
    if (EDITOR_SELECTTILE) return;
    EditPropertyDialog();
    window.draw(EDITOR_ExitGate);
    for (const auto &i : Tile) {
        const bool isTileTouch = TileX == i.getPosition().x && TileY == i.getPosition().y;
        if (!isOutScreen(i.getPosition().x, i.getPosition().y, 32, 32)) {
            if (i.getEndPos() != sf::Vector2f(-1.f, -1.f)) {
                sf::VertexArray line(sf::PrimitiveType::Lines, 2);
                line[0].position = (i.getPosition() + i.getPosition() + static_cast<sf::Vector2f>(i.getTexture()->getSize())) * 0.5f;
                line[1].position = (i.getEndPos() + i.getEndPos() + static_cast<sf::Vector2f>(i.getTexture()->getSize())) * 0.5f;

                line[0].color = sf::Color::Black;
                line[1].color = sf::Color::Black;
                window.draw(line);

                MFCPP::SimpleSprite R(i.getTexture());
                R.setPosition(i.getEndPos());
                R.setOrigin(i.getOrigin());
                R.setColor(sf::Color(255, 255, 255, 128));
                window.draw(R);
            }
            MFCPP::SimpleSprite T(i.getTexture());
            T.setPosition(i.getPosition());
            T.setOrigin(i.getOrigin());
            if (!EDITOR_BuildMode && !showEditProperty && isTileTouch) {
                T.setColor(sf::Color(0, 255, 0));
                T.setRenderTexture(false);
            }
            window.draw(T);
        }
    }
    window.draw(EDITOR_Mario);
    window.draw(EDITOR_ExitGateIndicator);
    if (EDITOR_BuildMode) window.draw(SelectBox);
    window.draw(Grid, ImageManager::GetReturnTexture("EDITOR_Grid"));
    window.draw(SelectedBlock);
}