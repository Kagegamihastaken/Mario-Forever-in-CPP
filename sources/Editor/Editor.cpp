#include <SFML/System/Vector2.hpp>
#include <unordered_set>
#include "Editor/Editor.hpp"

#include <fstream>
#include <ctre.hpp>

#include "Editor/SelectTile.hpp"
#include "Editor/TabButton.hpp"

#include <iostream>

#include "Core/WindowFrame.hpp"
#include "Core/Hash.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Interpolation.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Scroll.hpp"

std::string fileOutPut = "lvl-test2.txt";
std::string fileInput = "lvl-test.txt";

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

static std::vector<std::array<float, 5>> LevelData;
//static std::vector<std::array<float, 3>> SlopeData;
static std::vector<std::array<float, 7>> BonusData;
static std::vector<std::array<float, 7>> EnemyData;

//Read file purpose onlu
static std::array<float, 4> ExitGateData{};
static std::array<float, 2> PlayerData{};

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
    ImageManager::AddImage("SelectBoxImage", "data/resources/Editor/EDITOR_SelectBox.png");
    ImageManager::AddTexture("SelectBoxImage", "EDITOR_SelectBox");
    ImageManager::AddImage("GridImage", "data/resources/Editor/EDITOR_Grid.png");
    ImageManager::AddTexture("GridImage", "EDITOR_Grid", true);

    ImageManager::AddImage("MushroomLuckyblockImage", "data/resources/Editor/EDITOR_SELECT_TILE_EXCLUSIVE/EDITOR_Mushroom_Luckyblock.png");
    ImageManager::AddTexture("MushroomLuckyblockImage", "EDITOR_MushroomLuckyblock");
    ImageManager::AddImage("CoinLuckyblockImage", "data/resources/Editor/EDITOR_SELECT_TILE_EXCLUSIVE/EDITOR_Coin_Luckyblock.png");
    ImageManager::AddTexture("CoinLuckyblockImage", "EDITOR_CoinLuckyblock");
    ImageManager::AddImage("CoinBrickImage", "data/resources/Editor/EDITOR_SELECT_TILE_EXCLUSIVE/EDITOR_Coin_Brick.png");
    ImageManager::AddTexture("CoinBrickImage", "EDITOR_CoinBrick");
    ImageManager::AddImage("HammerBroCanMoveImage", "data/resources/Editor/EDITOR_SELECT_TILE_EXCLUSIVE/EDITOR_Hammer_CanMove.png");
    ImageManager::AddTexture("HammerBroCanMoveImage", "EDITOR_HammerBroCanMove");
    ImageManager::AddImage("HammerBroCannotMoveImage", "data/resources/Editor/EDITOR_SELECT_TILE_EXCLUSIVE/EDITOR_Hammer_CannotMove.png");
    ImageManager::AddTexture("HammerBroCannotMoveImage", "EDITOR_HammerBroCannotMove");
    ImageManager::AddImage("FireFlowerLuckyblockImage", "data/resources/Editor/EDITOR_SELECT_TILE_EXCLUSIVE/EDITOR_FireFlower_Luckyblock.png");
    ImageManager::AddTexture("FireFlowerLuckyblockImage", "EDITOR_FireFlowerLuckyblock");

    SelectBox.setTexture(ImageManager::GetTexture("EDITOR_SelectBox"), true);

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
void FileSave() {
    std::ofstream foo(fileOutPut);
    if (!foo.is_open()) {
        std::cout << "Cannot create file: " << fileOutPut;
        return;
    }
    std::cout << "Saving...\n";
    foo << "[LVL]\n";
    foo << "BGFC=7495f5 BGSC=f5fefd\n";
    foo << "EGIX=" << EDITOR_ExitGateIndicator.getPosition().x - EDITOR_ExitGateIndicator.getOrigin().x + TilePage[LevelTab][1].origin.x << " " << "EGIY=" << EDITOR_ExitGateIndicator.getPosition().y - EDITOR_ExitGateIndicator.getOrigin().y + TilePage[LevelTab][1].origin.y << " " << "EGX=" << EDITOR_ExitGate.getPosition().x - EDITOR_ExitGate.getOrigin().x + TilePage[LevelTab][2].origin.x << " " << "EGY=" << EDITOR_ExitGate.getPosition().y - EDITOR_ExitGate.getOrigin().y + TilePage[LevelTab][2].origin.y << "\n";
    foo << "LVLW=" << TEST_LevelWidth << " " << "LVLH=" << TEST_LevelHeight << "\n";
    foo << "MX=" << EDITOR_Mario.getPosition().x - EDITOR_Mario.getOrigin().x + TilePage[LevelTab][0].origin.x << " " << "MY=" << EDITOR_Mario.getPosition().y - EDITOR_Mario.getOrigin().y + TilePage[LevelTab][0].origin.y << "\n";
    foo << "MUSICT=0 MUSICN=MainMusicMOD\n";
    foo << "[TILE]\n";
    for (const auto &i : Tile) {
        foo << "EI=" << i.getID() << " " << "EP=" << i.getPage() << " " << "EX=" << i.getPosition().x - i.getOrigin().x + TilePage[i.getPage()][i.getID()].origin.x << " " << "EY=" << i.getPosition().y - i.getOrigin().y + TilePage[i.getPage()][i.getID()].origin.y << "\n";
        //switch (TileLoop->categoryID) {
        //    case 0:
                // EI EP TILE_ID TILE_X TILE_Y
        //        LevelData.push_back({static_cast<float>(idLoop), static_cast<float>(pgLoop), static_cast<float>(TileLoop->objectID), i.getPosition().x-i.getOrigin().x+TileLoop->origin.x, i.getPosition().y-i.getOrigin().y+TileLoop->origin.y});
        //        break;
        //    case 1:
                // EI EP BONUS_TYPE BONUS_ID BONUS_ATT BONUS_X BONUS_Y
        //        BonusData.push_back({static_cast<float>(idLoop), static_cast<float>(pgLoop), static_cast<float>(TileLoop->objectID), static_cast<float>(TileLoop->customID1), static_cast<float>(TileLoop->customID2), i.getPosition().x-i.getOrigin().x+TileLoop->origin.x, i.getPosition().y-i.getOrigin().y+TileLoop->origin.y});
        //        break;
        //    case 2:
        //        EnemyData.push_back({static_cast<float>(idLoop), static_cast<float>(pgLoop), static_cast<float>(TileLoop->objectID), static_cast<float>(TileLoop->customID1), static_cast<float>(TileLoop->customID2), i.getPosition().x-i.getOrigin().x+TileLoop->origin.x, i.getPosition().y-i.getOrigin().y+TileLoop->origin.y});
        //        break;
        //    default: ;
        //}
    }
    foo.close();
}
void TILEREAD(const std::string& line) {
    //TILE
    std::array<float, 4> temp{};
    bool flag = false;
    for (auto match : ctre::search_all<"(EI|EP|EX|EY)=(\\S*)">(line)) {
        if (match.get<1>() == "EI") {
            temp[0] = match.get<2>().to_number<float>();
            if (!flag) flag = true;
        }
        else if (match.get<1>() == "EP") {
            temp[1] = match.get<2>().to_number<float>();
            if (!flag) flag = true;
        }
        else if (match.get<1>() == "EX") {
            temp[2] = match.get<2>().to_number<float>();
            if (!flag) flag = true;
        }
        else if (match.get<1>() == "EY") {
            temp[3] = match.get<2>().to_number<float>();
            if (!flag) flag = true;
        }
    }
    if (flag) {
        const sf::Vector2f origin_tile(0.0f, static_cast<float>(ImageManager::GetReturnTexture(TilePage[static_cast<int>(temp[1])][static_cast<int>(temp[0])].name)->getSize().y) - 32.0f);
        const sf::Vector2f pos(temp[2] - TilePage[static_cast<int>(temp[1])][static_cast<int>(temp[0])].origin.x + origin_tile.x, temp[3] - TilePage[static_cast<int>(temp[1])][static_cast<int>(temp[0])].origin.y + origin_tile.y);
        RenderTile tile(*ImageManager::GetReturnTexture(TilePage[static_cast<int>(temp[1])][static_cast<int>(temp[0])].name), pos, static_cast<int>(temp[1]), static_cast<int>(temp[0]));
        tile.setOrigin(origin_tile);
        Tile.insert(tile);
    }
}
void LVLREAD(const std::string& line) {
    //BG
    /*
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
    */
    //EXIT
    for (auto match : ctre::search_all<"(EGIX|EGIY|EGX|EGY)=(\\S*)">(line)) {
        if (match.get<1>() == "EGIX") ExitGateData[0] = match.get<2>().to_number<float>();
        else if (match.get<1>() == "EGIY") ExitGateData[1] = match.get<2>().to_number<float>();
        else if (match.get<1>() == "EGX") ExitGateData[2] = match.get<2>().to_number<float>();
        else if (match.get<1>() == "EGY") ExitGateData[3] = match.get<2>().to_number<float>();
    }
    //LEVELDATA
    /*
    for (auto match : ctre::search_all<"(LEVEL_WIDTH|LEVEL_HEIGHT)=(\\S*)">(line)) {
        if (match.get<1>() == "LEVEL_WIDTH") LevelWidth = match.get<2>().to_number<float>();
        else if (match.get<1>() == "LEVEL_HEIGHT") LevelHeight = match.get<2>().to_number<float>();
    }
    */
    //MARIOPOS
    for (auto match : ctre::search_all<"(MX|MY)=(\\S*)">(line)) {
        if (match.get<1>() == "MX") PlayerData[0] = match.get<2>().to_number<float>();
        else if (match.get<1>() == "MY") PlayerData[1] = match.get<2>().to_number<float>();
    }
    //MUSIC
    /*
    for (auto match : ctre::search_all<"(MUSIC_TYPE|MUSIC_NAME)=(\\S*)">(line)) {
        if (match.get<1>() == "MUSIC_TYPE") MusicData.first = match.get<2>().to_number<int>();
        else if (match.get<1>() == "MUSIC_NAME") MusicData.second = match.get<2>().to_string();
    }
    */
    //process
}
void FileLoad() {
    std::ifstream foi(fileInput);
    if (!foi.is_open()) {
        std::cout << "Cannot open file: " << fileInput << "\n";
        return;
    }
    //std::string lvldat;
    //std::stringstream input_view(lvldat);
    std::string line;
    Tile.clear();
    int ReadMode = 0;
    // Read the file
    bool flag = false;
    while (std::getline(foi, line)) {
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
        }
        if (flag) continue;

        switch (ReadMode) {
            case 0: LVLREAD(line); break;
            case 1: TILEREAD(line); break;
            default: ;
        }
    }
    EDITOR_Mario.setPosition(sf::Vector2f(PlayerData[0] - TilePage[LevelTab][0].origin.x + EDITOR_Mario.getOrigin().x, PlayerData[1] - TilePage[LevelTab][0].origin.y + EDITOR_Mario.getOrigin().y));
    EDITOR_ExitGateIndicator.setPosition(sf::Vector2f(ExitGateData[0] - TilePage[LevelTab][1].origin.x + EDITOR_ExitGateIndicator.getOrigin().x, ExitGateData[1] - TilePage[LevelTab][1].origin.y + EDITOR_ExitGateIndicator.getOrigin().y));
    EDITOR_ExitGate.setPosition(sf::Vector2f(ExitGateData[2] - TilePage[LevelTab][2].origin.x + EDITOR_ExitGate.getOrigin().x, ExitGateData[3] - TilePage[LevelTab][2].origin.y + EDITOR_ExitGate.getOrigin().y));
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
                SoundManager::PlaySound("EDITOR_MENU");
                EDITOR_SELECTTILE = !EDITOR_SELECTTILE;
                if (EDITOR_SELECTTILE) PreviewPage = CurrPage;
                SelectTileDisplayUpdate();
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
    else if (EDITOR_SELECTILE_CLOCK.getElapsedTime().asSeconds() < 0.15f) return;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && window.hasFocus()) {
        if ((lastDeleteX != TileX || lastDeleteY != TileY) || (lastPlaceX == TileX && lastPlaceY == TileY)) {
            if (CurrPage != LevelTab) {
                if (Tile.contains(sf::Vector2f(TileX, TileY))) {
                    //std::cout << "Found!\n";
                    SoundManager::PlaySound("EDITOR_DELETE");
                    Tile.erase(sf::Vector2f(TileX, TileY));
                }
            }
            //else std::cout << "Not Found!\n";
            lastDeleteX = TileX;
            lastDeleteY = TileY;
            lastPlaceX = -1.0f;
            lastPlaceY = -1.0f;
        }
    }
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)&& window.hasFocus()) {
        if ((lastPlaceX != TileX || lastPlaceY != TileY) || (lastDeleteX == TileX && lastDeleteY == TileY)) {
            if (CurrPage != LevelTab) {
                if (TileX == EDITOR_Mario.getPosition().x && TileY == EDITOR_Mario.getPosition().y) return;
                else if (TileX == EDITOR_ExitGateIndicator.getPosition().x && TileY == EDITOR_ExitGateIndicator.getPosition().y) return;
                if (!Tile.contains(sf::Vector2f(TileX, TileY))) {
                    //std::cout << "Placed\n";
                    SoundManager::PlaySound("EDITOR_PLACE");
                    RenderTile tile(*ImageManager::GetReturnTexture(TilePage[CurrPage][CurrSelectTile].name), sf::Vector2f(TileX, TileY), CurrPage, CurrSelectTile);
                    tile.setOrigin(sf::Vector2f(0.0f, ImageManager::GetReturnTexture(TilePage[CurrPage][CurrSelectTile].name)->getSize().y - 32.0f));
                    Tile.insert(tile);
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
    window.draw(EDITOR_ExitGate);
    for (const auto &i : Tile) {
        if (!isOutScreen(i.getPosition().x, i.getPosition().y, 32, 32)) {
            window.draw(i);
        }
    }
    window.draw(EDITOR_Mario);
    window.draw(EDITOR_ExitGateIndicator);
    window.draw(SelectBox);
    window.draw(Grid, ImageManager::GetReturnTexture("EDITOR_Grid"));
    window.draw(SelectedBlock);
}
