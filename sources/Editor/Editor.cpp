#include <SFML/Graphics.hpp>
#include <unordered_set>
#include "Editor/Editor.hpp"
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

static sf::VertexArray Grid;
static sf::Texture Gridtex;
static float GridAlpha = 0.0f;
static bool GridAlphaState = true;
constexpr float GRIDALPHA_MAX = 255.0f;
constexpr float GRIDALPHA_MIN = 0.0f;
constexpr float GRIDALPHA_CHANGE = 1.0f;

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

static void AlphaUpdate(float& alpha, bool& state, const float min, const float max, const float change, const float dt) {
    if (state) {
        if (alpha < max) alpha += change * dt;
        else {
            alpha = max;
            state = false;
        }
    }
    else {
        if (alpha > min) alpha -= change * dt;
        else {
            alpha = min;
            state = true;
        }
    }
}

void EditorInit() {

    Grid.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
    Grid.resize(4);

    ImageManager::AddImage("SelectBoxImage", "data/resources/Editor/EDITOR_SelectBox.png");
    ImageManager::AddTexture("SelectBoxImage", "EDITOR_SelectBox");
    ImageManager::AddImage("GridImage", "data/resources/Editor/EDITOR_Grid.png");
    ImageManager::AddTexture("GridImage", "EDITOR_Grid", true);

    Gridtex = ImageManager::GetTexture("EDITOR_Grid");
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
    TileX = std::floor((MouseX + EditorPos.x < 0 ? 0 : MouseX + EditorPos.x) / 32.0f) * 32.0f;
    TileY = std::floor((MouseY + EditorPos.y < 0 ? 0 : MouseY + EditorPos.y) / 32.0f) * 32.0f;

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
void SwitchTile(const std::optional<sf::Event>& event) {
    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Num2) {
            CurrTileName = (CurrTileName + 1) % static_cast<int>(TileName.size());
            SoundManager::PlaySound("EDITOR_SWITCH");
        }
        else if (keyPressed->code == sf::Keyboard::Key::Num1) {
            CurrTileName = CurrTileName - 1 < 0 ? static_cast<int>(TileName.size()) - 1 : CurrTileName - 1;
            SoundManager::PlaySound("EDITOR_SWITCH");
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) EditorPos.x += 8.0f * dt;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) EditorPos.x -= 8.0f * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) EditorPos.y -= 8.0f * dt;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) EditorPos.y += 8.0f * dt;
    if (EditorPos.x <= 0.0f) EditorPos.x = 0.0f;
    if (EditorPos.y <= 0.0f) EditorPos.y = 0.0f;
}

void DrawTile() {
    for (const auto &i : Tile) {
        if (!isOutScreen(i.getPosition().x, i.getPosition().y, 32, 32)) {
            window.draw(i);
        }
    }
    window.draw(SelectBox);
    window.draw(Grid, &Gridtex);
    window.draw(SelectedBlock);
}
