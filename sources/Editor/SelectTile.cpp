#include <SFML/Graphics.hpp>
#include "Core/ImageManager.hpp"
#include "Editor/Editor.hpp"
#include "Core/WindowFrame.hpp"
#include "Editor/SelectTile.hpp"

#include <iostream>

#include "Editor/TabButton.hpp"

static sf::VertexArray SelectTileBackground(sf::PrimitiveType::TriangleStrip, 4);
bool EDITOR_SELECTTILE = false;
sf::Clock EDITOR_SELECTILE_CLOCK;
static sf::VertexArray SelectTileGrid(sf::PrimitiveType::TriangleStrip, 4);
static float SelectTileGridAlpha = 0.0f;
static bool SelectTileGridAlphaState = true;
static float SELECT_TILE_GRID_ALPHA_MIN = 0.0f;
static float SELECT_TILE_GRID_ALPHA_MAX = 255.0f;
static float SELECT_TILE_GRID_ALPHA_CHANGE = 1.0f;

static sf::Sprite SelectTileBox(tempTex);
static int SelectTileWidth, SelectTileHeight;
static float SelectTileX, SelectTileY;
static float SelectTileAlpha = 0.0f;
static bool SelectTileAlphaState = true;
static float SELECT_TILE_ALPHA_MIN = 0.0f;
static float SELECT_TILE_ALPHA_MAX = 255.0f;
static float SELECT_TILE_ALPHA_CHANGE = 7.0f;

static float TilePosX, TilePosY;

static sf::RenderTexture SelectTileRender(sf::Vector2u(480, 352));
static sf::VertexArray SelectTileRenderVA(sf::PrimitiveType::TriangleStrip, 4);
std::array<std::vector<SelectTileData>, 4> TilePage = {{
    { // PAGE 1
        {"Tile_0", sf::Vector2f(0, 0)},
        {"Tile_1", sf::Vector2f(32, 0)},
        {"Tile_2", sf::Vector2f(64, 0)},
        {"Tile_3", sf::Vector2f(0, 32)},
        {"Tile_4", sf::Vector2f(32, 32)},
        {"Tile_5", sf::Vector2f(64, 32)},
        {"Tile_6", sf::Vector2f(96, 0)},
        {"Tile_7", sf::Vector2f(128, 0)},
        {"Tile_8", sf::Vector2f(160, 0)},
        {"Tile_9", sf::Vector2f(128, 32)},
        {"Tile_10", sf::Vector2f(160, 32)}
    },
    { // PAGE 2
            {"Coin_0", sf::Vector2f(0, 0)},
            {"EDITOR_CoinLuckyblock", sf::Vector2f(32, 0)},
            {"EDITOR_MushroomLuckyblock", sf::Vector2f(64, 0)},
            {"NormalBrick", sf::Vector2f(96, 0)},
            {"EDITOR_CoinBrick", sf::Vector2f(128, 0)},
        },
    { // PAGE 3
        {"Goomba_0", sf::Vector2f(0, 0), sf::Vector2i(0, 0), sf::Vector2f(15.0f, 31.0f)},
        {"GreenKoopaLeft_0", sf::Vector2f(32, 0), sf::Vector2i(0, 0), sf::Vector2f(32.0f, 47.0f)},
        {"RedSpinyLeft_0", sf::Vector2f(64, 0), sf::Vector2i(0, 0), sf::Vector2f(16.0f, 29.0f)},
        {"GreenKoopaShell_3", sf::Vector2f(96, 0), sf::Vector2i(0, 0), sf::Vector2f(16.0f, 27.0f)},
        {"PiranhaGreen_0", sf::Vector2f(128, 0), sf::Vector2i(16, 22), sf::Vector2f(16.0f, 27.0f)},
        {"PiranhaGreenGround_0", sf::Vector2f(0, 32)},
        {"NormalSpike", sf::Vector2f(32, 32)}
    },
    { // PAGE 4
        {"SmallMarioRight_2", sf::Vector2f(0, 0), sf::Vector2i(0, 28), sf::Vector2f(11.0f, 51.0f)},
        {"ExitGateIndicator_0", sf::Vector2f(32, 0), sf::Vector2i(0, 0), sf::Vector2f(0.0f, 31.0f)},
        {"ExitGateBack", sf::Vector2f(64, 0), sf::Vector2i(64, 0), sf::Vector2f(0.0f, 287.0f)}
    }
}};
std::array<MFCPP::TabButton, 4> TabList{};

int CurrSelectTile = 0;
int PrevSelectTile = -1;
int CurrPage = 0;
int PrevPage = -1;
int PreviewPage = CurrPage;

const int LevelTab = 3;

void SelectTileInit() {
    ImageManager::AddImage("SelectTileBackgroundImage", "data/resources/Editor/EDITOR_TileSelectBackGround.png");
    ImageManager::AddTexture("SelectTileBackgroundImage", "EDITOR_SelectTileBackground");
    ImageManager::AddImage("SelectTileGridImage", "data/resources/Editor/EDITOR_SelectTileGrid.png");
    ImageManager::AddTexture("SelectTileGridImage", "EDITOR_SelectTileGrid");
    ImageManager::AddImage("SelectTileBoxImage", "data/resources/Editor/EDITOR_SelectTileBox.png");
    ImageManager::AddTexture("SelectTileBoxImage", "EDITOR_SelectTileBox");
    //TAB
    ImageManager::AddImage("TileTabImage", "data/resources/Editor/EDITOR_TAB/EDITOR_TileTab.png");
    ImageManager::AddTexture("TileTabImage", "EDITOR_TileTab");
    ImageManager::AddImage("BonusTabImage", "data/resources/Editor/EDITOR_TAB/EDITOR_BonusTab.png");
    ImageManager::AddTexture("BonusTabImage", "EDITOR_BonusTab");
    ImageManager::AddImage("EnemyTabImage", "data/resources/Editor/EDITOR_TAB/EDITOR_EnemyTab.png");
    ImageManager::AddTexture("EnemyTabImage", "EDITOR_EnemyTab");
    ImageManager::AddImage("LevelTabImage", "data/resources/Editor/EDITOR_TAB/EDITOR_LevelTab.png");
    ImageManager::AddTexture("LevelTabImage", "EDITOR_LevelTab");

    TabList[0].setTexture(*ImageManager::GetReturnTexture("EDITOR_TileTab"));
    TabList[1].setTexture(*ImageManager::GetReturnTexture("EDITOR_BonusTab"));
    TabList[2].setTexture(*ImageManager::GetReturnTexture("EDITOR_EnemyTab"));
    TabList[3].setTexture(*ImageManager::GetReturnTexture("EDITOR_LevelTab"));

    SelectTileWidth = ImageManager::GetReturnTexture("EDITOR_SelectTileGrid")->getSize().x;
    SelectTileHeight = ImageManager::GetReturnTexture("EDITOR_SelectTileGrid")->getSize().y;

    SelectTileBackground[0].texCoords = sf::Vector2f(0.0f, 0.0f);
    SelectTileBackground[1].texCoords = sf::Vector2f(640.0f, 0.0f);
    SelectTileBackground[2].texCoords = sf::Vector2f(0.0f, 640.0f);
    SelectTileBackground[3].texCoords = sf::Vector2f(640.0f, 640.0f);

    SelectTileGrid[0].texCoords = SelectTileRenderVA[0].texCoords = sf::Vector2f(0.0f, 0.0f);
    SelectTileGrid[1].texCoords = SelectTileRenderVA[1].texCoords = sf::Vector2f(480.0f, 0.0f);
    SelectTileGrid[2].texCoords = SelectTileRenderVA[2].texCoords = sf::Vector2f(0.0f, 352.0f);
    SelectTileGrid[3].texCoords = SelectTileRenderVA[3].texCoords = sf::Vector2f(480.0f, 352.0f);

    SelectTileBox.setTexture(ImageManager::GetTexture("EDITOR_SelectTileBox"), true);
}

int CheckExistPos() {
    for (int i = 0; i < TilePage[PreviewPage].size(); ++i) {
        if (TilePosX == TilePage[PreviewPage][i].position.x && TilePosY == TilePage[PreviewPage][i].position.y) return i;
    }
    return -1;
}

void SelectTileDisplayUpdate() {
    SelectTileRender.clear(sf::Color::Transparent);
    for (const auto & [name, position, texPos, origin] : TilePage[PreviewPage]) {
        sf::Sprite loop(ImageManager::GetTexture(name));
        loop.setTextureRect(sf::IntRect(texPos, {std::min(static_cast<int>(loop.getGlobalBounds().size.x - texPos.x), 32), std::min(static_cast<int>(loop.getGlobalBounds().size.y - texPos.y), 32)}));
        loop.setPosition(position);
        SelectTileRender.draw(loop);
    }
    SelectTileRender.display();
}

void SelectTileAlphaUpdate(const float dt) {
    AlphaUpdate(SelectTileAlpha, SelectTileAlphaState, SELECT_TILE_ALPHA_MIN, SELECT_TILE_ALPHA_MAX, SELECT_TILE_ALPHA_CHANGE, dt);
    SelectTileBox.setColor(sf::Color(255, 255, 255, static_cast<int>(SelectTileAlpha)));

    AlphaUpdate(SelectTileGridAlpha, SelectTileGridAlphaState, SELECT_TILE_GRID_ALPHA_MIN, SELECT_TILE_GRID_ALPHA_MAX, SELECT_TILE_GRID_ALPHA_CHANGE, dt);
    SelectTileGrid[0].color = SelectTileGrid[1].color = SelectTileGrid[2].color = SelectTileGrid[3].color = sf::Color(255, 255, 255, SelectTileGridAlpha);
}

void SelectTilePosUpdate() {
    SelectTileBackground[0].position = sf::Vector2f(EditorInterpolatedPos.x, EditorInterpolatedPos.y);
    SelectTileBackground[1].position = sf::Vector2f(EditorInterpolatedPos.x + 640.0f, EditorInterpolatedPos.y);
    SelectTileBackground[2].position = sf::Vector2f(EditorInterpolatedPos.x, EditorInterpolatedPos.y + 640.0f);
    SelectTileBackground[3].position = sf::Vector2f(EditorInterpolatedPos.x + 640.0f, EditorInterpolatedPos.y + 640.0f);

    SelectTileGrid[0].position = SelectTileRenderVA[0].position = sf::Vector2f(128.0f + EditorInterpolatedPos.x, 96.0f + EditorInterpolatedPos.y);
    SelectTileGrid[1].position = SelectTileRenderVA[1].position = sf::Vector2f(128.0f + EditorInterpolatedPos.x + SelectTileWidth, 96.0f + EditorInterpolatedPos.y);
    SelectTileGrid[2].position = SelectTileRenderVA[2].position = sf::Vector2f(128.0f + EditorInterpolatedPos.x, 96.0f + EditorInterpolatedPos.y + SelectTileHeight);
    SelectTileGrid[3].position = SelectTileRenderVA[3].position = sf::Vector2f(128.0f + EditorInterpolatedPos.x + SelectTileWidth, 96.0f + EditorInterpolatedPos.y + SelectTileHeight);

    TabList[0].setPosition(sf::Vector2f(128.0f + EditorInterpolatedPos.x, 29.0f + EditorInterpolatedPos.y));
    TabList[1].setPosition(sf::Vector2f(170.0f + EditorInterpolatedPos.x, 29.0f + EditorInterpolatedPos.y));
    TabList[2].setPosition(sf::Vector2f(212.0f + EditorInterpolatedPos.x, 29.0f + EditorInterpolatedPos.y));
    TabList[3].setPosition(sf::Vector2f(254.0f + EditorInterpolatedPos.x, 29.0f + EditorInterpolatedPos.y));

    if (!EDITOR_SELECTTILE) return;

    const float ModX = f_mod(SelectTileBackground[0].position.x, 32.0f);
    const float ModY = f_mod(SelectTileBackground[0].position.y, 32.0f);
    const float BoxTileX = SelectTileBackground[0].position.x - ModX;
    const float BoxTileY = SelectTileBackground[0].position.y - ModY;
    SelectTileX = std::floor((MouseX + BoxTileX < 0 ? 0 : MouseX + BoxTileX) / 32.0f) * 32.0f + ModX;
    SelectTileY = std::floor((MouseY + BoxTileY < 0 ? 0 : MouseY + BoxTileY) / 32.0f) * 32.0f + ModY;
    //Update Tile Select Position
    TilePosX = SelectTileX - BoxTileX - ModX - 128.0f;
    TilePosY = SelectTileY - BoxTileY - ModY - 96.0f;

    SelectTileBox.setPosition(sf::Vector2f(SelectTileX, SelectTileY));
}

void SelectTileDraw() {
    if (!EDITOR_SELECTTILE) return;

    window.draw(SelectTileBackground, ImageManager::GetReturnTexture("EDITOR_SelectTileBackground"));
    window.draw(SelectTileRenderVA, &SelectTileRender.getTexture());
    window.draw(SelectTileGrid, ImageManager::GetReturnTexture("EDITOR_SelectTileGrid"));

    for (const auto &i : TabList)
        window.draw(i);
    if (TilePosX >= 0 && TilePosY >= 0) window.draw(SelectTileBox);
}