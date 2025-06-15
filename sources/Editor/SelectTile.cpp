#include <SFML/Graphics.hpp>
#include "Core/ImageManager.hpp"
#include "Editor/Editor.hpp"
#include "Core/WindowFrame.hpp"

#include <iostream>

static sf::VertexArray SelectTileBackground(sf::PrimitiveType::TriangleStrip, 4);
bool EDITOR_SELECTTILE = false;
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

void SelectTileInit() {
    ImageManager::AddImage("SelectTileBackgroundImage", "data/resources/Editor/EDITOR_TileSelectBackGround.png");
    ImageManager::AddTexture("SelectTileBackgroundImage", "EDITOR_SelectTileBackground");
    ImageManager::AddImage("SelectTileGridImage", "data/resources/Editor/EDITOR_SelectTileGrid.png");
    ImageManager::AddTexture("SelectTileGridImage", "EDITOR_SelectTileGrid");
    ImageManager::AddImage("SelectTileBoxImage", "data/resources/Editor/EDITOR_SelectTileBox.png");
    ImageManager::AddTexture("SelectTileBoxImage", "EDITOR_SelectTileBox");

    SelectTileWidth = ImageManager::GetReturnTexture("EDITOR_SelectTileGrid")->getSize().x;
    SelectTileHeight = ImageManager::GetReturnTexture("EDITOR_SelectTileGrid")->getSize().y;

    SelectTileBackground[0].texCoords = sf::Vector2f(0.0f, 0.0f);
    SelectTileBackground[1].texCoords = sf::Vector2f(640.0f, 0.0f);
    SelectTileBackground[2].texCoords = sf::Vector2f(0.0f, 640.0f);
    SelectTileBackground[3].texCoords = sf::Vector2f(640.0f, 640.0f);

    SelectTileGrid[0].texCoords = sf::Vector2f(0.0f, 0.0f);
    SelectTileGrid[1].texCoords = sf::Vector2f(480.0f, 0.0f);
    SelectTileGrid[2].texCoords = sf::Vector2f(0.0f, 352.0f);
    SelectTileGrid[3].texCoords = sf::Vector2f(480.0f, 352.0f);

    SelectTileBox.setTexture(ImageManager::GetTexture("EDITOR_SelectTileBox"), true);
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

    SelectTileGrid[0].position = sf::Vector2f(128.0f + EditorInterpolatedPos.x, 96.0f + EditorInterpolatedPos.y);
    SelectTileGrid[1].position = sf::Vector2f(128.0f + EditorInterpolatedPos.x + SelectTileWidth, 96.0f + EditorInterpolatedPos.y);
    SelectTileGrid[2].position = sf::Vector2f(128.0f + EditorInterpolatedPos.x, 96.0f + EditorInterpolatedPos.y + SelectTileHeight);
    SelectTileGrid[3].position = sf::Vector2f(128.0f + EditorInterpolatedPos.x + SelectTileWidth, 96.0f + EditorInterpolatedPos.y + SelectTileHeight);

    if (!EDITOR_SELECTTILE) return;

    const float ModX = f_mod(SelectTileBackground[0].position.x, 32.0f);
    const float ModY = f_mod(SelectTileBackground[0].position.y, 32.0f);
    const float BoxTileX = SelectTileBackground[0].position.x - ModX;
    const float BoxTileY = SelectTileBackground[0].position.y - ModY;
    SelectTileX = std::floor((std::max(std::min(MouseX, 120.0f + SelectTileWidth), 128.0f) + BoxTileX < 0 ? 0 : std::max(std::min(MouseX, 120.0f + SelectTileWidth), 128.0f) + BoxTileX) / 32.0f) * 32.0f + ModX;
    SelectTileY = std::floor((std::max(std::min(MouseY, 96.0f + SelectTileHeight - 32.0f), 96.0f) + BoxTileY < 0 ? 0 : std::max(std::min(MouseY, 96.0f + SelectTileHeight - 32.0f), 96.0f) + BoxTileY) / 32.0f) * 32.0f + ModY;
    //Update Tile Select Position
    TilePosX = SelectTileX - BoxTileX - ModX - 128.0f;
    TilePosY = SelectTileY - BoxTileY - ModY - 96.0f;

    SelectTileBox.setPosition(sf::Vector2f(SelectTileX, SelectTileY));
}

void SelectTileDraw() {
    if (EDITOR_SELECTTILE) {
        window.draw(SelectTileBackground, ImageManager::GetReturnTexture("EDITOR_SelectTileBackground"));
        window.draw(SelectTileGrid, ImageManager::GetReturnTexture("EDITOR_SelectTileGrid"));
        window.draw(SelectTileBox);
    }
}