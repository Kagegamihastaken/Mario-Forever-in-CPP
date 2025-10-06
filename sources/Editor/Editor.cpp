#include <nlohmann/json.hpp>
#include "Editor/Editor.hpp"

#include <imgui.h>
#include <ImGuiFileDialog.h>
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
#include "Core/Background/BgGradient.hpp"

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
    ImageManager::AddTexture("EDITOR_GreenMushroomLuckyblock", "data/resources/Editor/EDITOR_SELECT_TILE_EXCLUSIVE/EDITOR_GreenMushroom_LuckyBlock.png");

    SelectBox.setTexture(ImageManager::GetTexture("EDITOR_SelectBox"), true);
    if (ImageManager::GetReturnTexture(TilePage[LevelTab][0].name) == nullptr) throw std::runtime_error("NULLPTR");
    EDITOR_Mario.setPosition(sf::Vector2f(128, 320));
    EDITOR_Mario.setTexture(ImageManager::GetReturnTexture(TilePage[LevelTab][0].name), true);
    EDITOR_Mario.setOrigin(sf::Vector2f(0.0f, static_cast<float>(ImageManager::GetReturnTexture(TilePage[LevelTab][0].name)->getSize().y) - 32.0f));

    EDITOR_ExitGateIndicator.setPosition(sf::Vector2f(256, 320));
    EDITOR_ExitGateIndicator.setTexture(*ImageManager::GetReturnTexture(TilePage[LevelTab][1].name), true);
    EDITOR_ExitGateIndicator.setOrigin(sf::Vector2f(0.0f, static_cast<float>(ImageManager::GetReturnTexture(TilePage[LevelTab][1].name)->getSize().y) - 32.0f));

    EDITOR_ExitGate.setPosition(sf::Vector2f(384, 320));
    EDITOR_ExitGate.setTexture(*ImageManager::GetReturnTexture(TilePage[LevelTab][2].name), true);
    EDITOR_ExitGate.setOrigin(sf::Vector2f(0.0f, static_cast<float>(ImageManager::GetReturnTexture(TilePage[LevelTab][2].name)->getSize().y) - 32.0f));

    BgGradientSetColor(BgColor.getProperty<ColorProps>("First Background Color")->val.ColorNormalize(), BgColor.getProperty<ColorProps>("Second Background Color")->val.ColorNormalize());
    BgGradientInitPos(LevelSize.getProperty<FloatProps>("Level Width")->val, LevelSize.getProperty<FloatProps>("Level Height")->val);
}
static void EditPropertyHelper(TileProperty& prop) {
    std::visit([&]<typename T0>(T0&& arg) {
        using T = std::decay_t<T0>;
        if constexpr (std::is_same_v<T, BoolProps>)
            ImGui::Checkbox(arg.name.c_str(), &arg.val);
        else if constexpr (std::is_same_v<T, IntProps>) {
            ImGui::PushItemWidth(125);
            ImGui::InputInt(arg.name.c_str(), &arg.val, 1, 100, ImGuiInputTextFlags_EscapeClearsAll);
            ImGui::PopItemWidth();
            if (arg.val > arg.max && arg.max > 0.f) arg.val = arg.max;
            else if (arg.val < arg.min) arg.val = arg.min;
        }
        else if constexpr (std::is_same_v<T, StringProps>)
            ImGui::InputText(arg.name.c_str(), arg.val, IM_ARRAYSIZE(arg.val), ImGuiInputTextFlags_EscapeClearsAll);
        else if constexpr (std::is_same_v<T, FloatProps>) {
            ImGui::PushItemWidth(125);
            ImGui::InputFloat(arg.name.c_str(), &arg.val, 1, 100, "%.2f", ImGuiInputTextFlags_EscapeClearsAll);
            ImGui::PopItemWidth();
            if (arg.val > arg.max && arg.max > 0.f) arg.val = arg.max;
            else if (arg.val < arg.min) arg.val = arg.min;
        }
        else if constexpr (std::is_same_v<T, Vector2fProps>) {
            ImGui::PushItemWidth(125);
            ImGui::InputFloat((arg.name+".x").c_str(), &arg.val.x, 0, 0, "%.2f", ImGuiInputTextFlags_EscapeClearsAll);
            ImGui::InputFloat((arg.name+".y").c_str(), &arg.val.y, 0, 0, "%.2f", ImGuiInputTextFlags_EscapeClearsAll);
            ImGui::PopItemWidth();
        }
        else if constexpr (std::is_same_v<T, ColorProps>)
            ImGui::ColorEdit4(arg.name.c_str(), reinterpret_cast<float*>(&arg.val), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview);
        }, prop);
}
static void DialogPopup(const std::string& name) {
    if (ImGui::BeginPopup("AddBGDoesntExist")) {
        ImGui::Text("Cannot Add Texture %s", name.c_str());
        ImGui::Separator();
        if (ImGui::Button("Close"))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}
static void AddBGPopup() {
    if (ImGui::BeginPopup("AddBackgroundPopup")) {
        ImGui::Text("Add New Background Layer");
        ImGui::Separator();

        static char BgName[256] = "";
        ImGui::InputText("BG Name", BgName, IM_ARRAYSIZE(BgName));

        if (ImGui::Button("Add")) {
            if (strlen(BgName) > 0) {
                if (ImageManager::isExist(BgName)) {
                    bool isExist = false;
                    for (const auto &i : BackgroundLayers) {
                        if (i.name == BgName) {
                            isExist = true;
                            break;
                        }
                    }
                    if (isExist)
                        ImGui::OpenPopup("AddBGDoesntExist");
                    else {
                        BackgroundLayers.emplace_back(BgName);
                        memset(BgName, 0, sizeof(BgName));
                        ImGui::CloseCurrentPopup();
                    }
                }
                else {
                    ImGui::OpenPopup("AddBGDoesntExist");
                }
            }
        }
        DialogPopup(BgName);
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}
void SettingDialog() {
    if (EDITOR_Setting) {
        ImGui::Begin("Setting", &EDITOR_Setting, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::SetWindowPos(ImVec2(static_cast<float>(window.getSize().x) / 2.f - ImGui::GetWindowSize().x / 2.f, static_cast<float>(window.getSize().y) / 2.f - ImGui::GetWindowSize().y / 2.f));
        if (ImGui::BeginTabBar("SettingTab")) {
            if (ImGui::BeginTabItem("BGGradient")) {
                for (int i = 0; i < BgColor.getPropertyCount(); ++i) {
                    EditPropertyHelper(*BgColor.at(i));
                }
                static int select = 0;

                if (ImGui::BeginListBox("Background")) {
                    for (int i = 0; i < BackgroundLayers.size(); ++i) {
                        bool is_selected = (select == i);
                        ImGuiSelectableFlags flags = (select == i) ? ImGuiSelectableFlags_Highlight : 0;
                        if (ImGui::Selectable(BackgroundLayers[i].name.c_str(), is_selected, flags))
                            select = i;
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndListBox();
                }
                const bool DisabledBackground = BackgroundLayers.empty();
                if (DisabledBackground)
                    ImGui::BeginDisabled();
                if (!DisabledBackground && select < BackgroundLayers.size()) {
                    ImGui::PushItemWidth(150);
                    // Use InputFloat2 for a compact X/Y editor
                    ImGui::InputFloat2("Parallax Factor", &BackgroundLayers[select].parallaxFactor.x, "%.2f");
                    ImGui::PopItemWidth();
                }
                if (ImGui::Button("Delete")) {
                    BackgroundLayers.erase(BackgroundLayers.begin() + select);
                    select = 0;
                }
                if (DisabledBackground)
                    ImGui::EndDisabled();
                ImGui::SameLine();
                if (ImGui::Button("Add"))
                    ImGui::OpenPopup("AddBackgroundPopup");
                AddBGPopup();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Level Size")) {
                for (int i = 0; i < LevelSize.getPropertyCount(); ++i) {
                    EditPropertyHelper(*LevelSize.at(i));
                }
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::Separator();
        if (ImGui::Button("Confirm")) {
            BgGradientSetColor(BgColor.getProperty<ColorProps>("First Background Color")->val.ColorNormalize(), BgColor.getProperty<ColorProps>("Second Background Color")->val.ColorNormalize());
            BgGradientInitPos(LevelSize.getProperty<FloatProps>("Level Width")->val, LevelSize.getProperty<FloatProps>("Level Height")->val);
            EDITOR_Setting = false;
        }
        ImGui::SetItemTooltip("Confirm!");
        ImGui::End();
    }
}
void EditPropertyDialog() {
    if (EDITOR_ShowProperty) {
        ImGui::Begin("Edit Property", &EDITOR_ShowProperty, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
        //ImGui::SetWindowSize("Edit Property", ImVec2(256.f, 256.f));
        ImGui::SetWindowPos(ImVec2(static_cast<float>(window.getSize().x) / 2.f - ImGui::GetWindowSize().x / 2.f, static_cast<float>(window.getSize().y) / 2.f - ImGui::GetWindowSize().y / 2.f));
        if (ImGui::BeginTabBar("EditPropertyTab")) {
            if (ImGui::BeginTabItem("Property")) {
                for (int i = 0; i < SaveTile.getProperty().getPropertyCount(); ++i) {
                    EditPropertyHelper(*SaveTile.getProperty().at(i));
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
            EDITOR_ShowProperty = false;
        }
        ImGui::End();
    }
}
void FileSave(const std::filesystem::path& path) {
    nlohmann::json levelJson;

    levelJson["level_properties"]["width"] = LevelSize.getProperty<FloatProps>("Level Width")->val;
    levelJson["level_properties"]["height"] = LevelSize.getProperty<FloatProps>("Level Height")->val;
    //TODO: Add Custom Music
    levelJson["level_properties"]["music"] = "DansLaRue";
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
    MFCPP::Log::SuccessPrint(fmt::format("Success Saved File {}", path.string()));
}

void FileLoad(const std::filesystem::path& path) {
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

        const sf::Vector2f origin_tile(0.0f, static_cast<float>(ImageManager::GetReturnTexture(TilePage[page][id].name)->getSize().y) - 32.f);
        RenderTile tile(TilePage[page][id].prop, *ImageManager::GetReturnTexture(TilePage[page][id].name), pos - TilePage[page][id].origin + origin_tile, page, id, endPos);
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
void OpenFileDialog() {
    if (!EDITOR_OpenDialog) return;
    if (!ImGuiFileDialog::Instance()->IsOpened("ChooseFileDlgKey")) {
        if (EDITOR_ShowProperty) EDITOR_ShowProperty = false;
        IGFD::FileDialogConfig config;
        config.path = ".";
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Open File", ".json", config);
    }
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse, ImVec2(Width / 2.f, Height / 2.f))) {
        if (ImGuiFileDialog::Instance()->IsOk())
            FileLoad(ImGuiFileDialog::Instance()->GetFilePathName());
        EDITOR_SELECTILE_CLOCK.restart();
        EDITOR_OpenDialog = false;
        ImGuiFileDialog::Instance()->Close();
    }
}
void SaveFileDialog() {
    if (!EDITOR_SaveDialog) return;
    if (!ImGuiFileDialog::Instance()->IsOpened("SaveFileDlgKey")) {
        if (EDITOR_ShowProperty) EDITOR_ShowProperty = false;
        IGFD::FileDialogConfig config;
        config.path = ".";
        config.fileName = "untitled.json";
        config.flags = ImGuiFileDialogFlags_ConfirmOverwrite;
        ImGuiFileDialog::Instance()->OpenDialog("SaveFileDlgKey", "Save File As...", ".json", config);
    }
    if (ImGuiFileDialog::Instance()->Display("SaveFileDlgKey", ImGuiWindowFlags_NoCollapse, ImVec2(Width / 2.f, Height / 2.f))) {
        if (ImGuiFileDialog::Instance()->IsOk())
            FileSave(ImGuiFileDialog::Instance()->GetFilePathName());
        EDITOR_SELECTILE_CLOCK.restart();
        EDITOR_SaveDialog = false;
        ImGuiFileDialog::Instance()->Close();
    }
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
    if (!EDITOR_ShowProperty && !EDITOR_OpenDialog && !EDITOR_SaveDialog) {
        TileX = std::floor((MouseX + EditorPos.x) / 32.0f) * 32.0f;
        TileY = std::floor((MouseY + EditorPos.y) / 32.0f) * 32.0f;
    }
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
    if (EDITOR_OpenDialog || EDITOR_SaveDialog) return;
    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        switch (keyPressed->code) {
            case sf::Keyboard::Key::Space:
                if (EDITOR_Setting) return;
                if (!EDITOR_ShowProperty && EDITOR_BuildMode) {
                    SoundManager::PlaySound("EDITOR_MENU");
                    EDITOR_SELECTTILE = !EDITOR_SELECTTILE;
                    if (EDITOR_SELECTTILE) PreviewPage = CurrPage;
                    SelectTileDisplayUpdate();
                }
                break;
            case sf::Keyboard::Key::S:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
                    if (!EDITOR_OpenDialog)
                        EDITOR_SaveDialog = true;
                }
                break;
            case sf::Keyboard::Key::D:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
                    if (!EDITOR_SaveDialog)
                        EDITOR_OpenDialog = true;
                }
                break;
            case sf::Keyboard::Key::B:
                if (!EDITOR_ShowProperty)
                    EDITOR_BuildMode = !EDITOR_BuildMode;
                break;
            default: ;
        }
    }
    else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
        switch (mousePressed->button) {
            case sf::Mouse::Button::Left:
                if (EDITOR_Setting) return;
                if (EDITOR_SELECTTILE) {
                    if (SettingButton.isMouseHovered(EditorInterpolatedPos, sf::Vector2f(MouseX, MouseY)))
                        EDITOR_Setting = true;
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
}
void PlaceTile() {
    if (EDITOR_SELECTTILE) return;
    if (EDITOR_SELECTILE_CLOCK.getElapsedTime().asSeconds() < 0.15f) return;

    if (EDITOR_ShowProperty || EDITOR_OpenDialog || EDITOR_SaveDialog) return;

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
                            EDITOR_ShowProperty = true;
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
    if (EDITOR_ShowProperty || EDITOR_OpenDialog || EDITOR_SaveDialog) return;

    if (!EDITOR_SELECTTILE) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) EditorPos.x += 8.0f * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) EditorPos.x -= 8.0f * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) EditorPos.y -= 8.0f * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) EditorPos.y += 8.0f * dt;
        if (EditorPos.x <= -5 * 32.f) EditorPos.x = -5 * 32.f;
        else if (EditorPos.x >= LevelSize.getProperty<FloatProps>("Level Width")->val - Width + 5 * 32.f) EditorPos.x = LevelSize.getProperty<FloatProps>("Level Width")->val - Width + 5 * 32.f;
        if (EditorPos.y <= -5 * 32.f) EditorPos.y = -5 * 32.f;
        else if (EditorPos.y >= LevelSize.getProperty<FloatProps>("Level Height")->val - Height + 5 * 32.f) EditorPos.y = LevelSize.getProperty<FloatProps>("Level Height")->val - Height + 5 * 32.f;
    }
}

void DrawTile() {
    if (EDITOR_SELECTTILE) return;
    EditPropertyDialog();
    OpenFileDialog();
    SaveFileDialog();
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
            if (!EDITOR_BuildMode && !EDITOR_ShowProperty && isTileTouch) {
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