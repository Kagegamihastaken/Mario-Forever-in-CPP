#include <SFML/Graphics.hpp>
#include <unordered_set>
#include "Editor/Editor.hpp"
#include "Editor/SelectTile.hpp"

#include <iostream>

#include "Core/WindowFrame.hpp"
#include "Core/Hash.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Interpolation.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Scroll.hpp"

static float lastPlaceX = -1.0f;
static float lastPlaceY = -1.0f;
static float lastDeleteX = -1.0f;
static float lastDeleteY = -1.0f;

sf::Vector2f EditorPos;
sf::Vector2f EditorPrevPos;
sf::Vector2f EditorInterpolatedPos;

static sf::Sprite SelectBox(tempTex);
static float SelectBoxAlpha = 100.0f;
static bool SelectBoxAlphaState = true;
constexpr float SELECTBOXALPHA_MAX = 160.0f;
constexpr float SELECTBOXALPHA_MIN = 100.0f;
constexpr float SELECTBOXALPHA_CHANGE = 1.0f;

static sf::VertexArray Grid(sf::PrimitiveType::TriangleStrip, 4);
static float GridAlpha = 0.0f;
static bool GridAlphaState = true;
constexpr float GRIDALPHA_MAX = 255.0f;
constexpr float GRIDALPHA_MIN = 0.0f;
constexpr float GRIDALPHA_CHANGE = 1.0f;

static float TEST_LevelWidth = 10016.0f;
static float TEST_LevelHeight = 480.0f;

static sf::Sprite SelectedBlock(tempTex);
static int CurrTileName = 0;
static int PrevTileName = -1;

std::unordered_set<RenderTile, RenderTileHash, RenderTileEqual> Tile;
static constexpr std::array<std::string, 11> TileName{
    "Tile_0",
    "Tile_1",
    "Tile_2",
    "Tile_3",
    "Tile_4",
    "Tile_5",
    "Tile_6",
    "Tile_7",
    "Tile_8",
    "Tile_9",
    "Tile_10"
};
static float TileX;
static float TileY;

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

    SelectBox.setTexture(ImageManager::GetTexture("EDITOR_SelectBox"), true);
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

    if (PrevTileName != CurrTileName) {
        SelectedBlock.setTexture(ImageManager::GetTexture(TileName[CurrTileName]), true);
        PrevTileName = CurrTileName;
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
                    CurrTileName = (CurrTileName + 1) % static_cast<int>(TileName.size());
                    SoundManager::PlaySound("EDITOR_SWITCH");
                }
                break;
            case sf::Keyboard::Key::Num1:
                if (!EDITOR_SELECTTILE) {
                    CurrTileName = CurrTileName - 1 < 0 ? static_cast<int>(TileName.size()) - 1 : CurrTileName - 1;
                    SoundManager::PlaySound("EDITOR_SWITCH");
                }
                break;
            case sf::Keyboard::Key::Space:
                SoundManager::PlaySound("EDITOR_MENU");
                EDITOR_SELECTTILE = !EDITOR_SELECTTILE;
                break;
            default: ;
        }
    }
    else if (const auto* mouse = event->getIf<sf::Event::MouseWheelScrolled>()) {
        if (mouse->delta < 0) {
            CurrTileName = (CurrTileName + 1) % static_cast<int>(TileName.size());
            SoundManager::PlaySound("EDITOR_SWITCH");
        }
        else if (mouse->delta > 0) {
            CurrTileName = CurrTileName - 1 < 0 ? static_cast<int>(TileName.size()) - 1 : CurrTileName - 1;
            SoundManager::PlaySound("EDITOR_SWITCH");
        }
    }
}
void PlaceTile() {
    if (EDITOR_SELECTTILE) return;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && window.hasFocus()) {
        if ((lastDeleteX != TileX || lastDeleteY != TileY) || (lastPlaceX == TileX && lastPlaceY == TileY)) {
            if (Tile.contains(sf::Vector2f(TileX, TileY))) {
                //std::cout << "Found!\n";
                SoundManager::PlaySound("EDITOR_DELETE");
                Tile.erase(sf::Vector2f(TileX, TileY));
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
            if (!Tile.contains(sf::Vector2f(TileX, TileY))) {
                //std::cout << "Placed\n";
                SoundManager::PlaySound("EDITOR_PLACE");
                Tile.insert(RenderTile(ImageManager::GetTexture(TileName[CurrTileName]), sf::Vector2f(TileX, TileY)));
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
    for (const auto &i : Tile) {
        if (!isOutScreen(i.getPosition().x, i.getPosition().y, 32, 32)) {
            window.draw(i);
        }
    }
    window.draw(SelectBox);
    window.draw(Grid, ImageManager::GetReturnTexture("EDITOR_Grid"));
    window.draw(SelectedBlock);
}
