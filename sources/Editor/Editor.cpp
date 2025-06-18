#include <SFML/System/Vector2.hpp>
#include <unordered_set>
#include "Editor/Editor.hpp"
#include "Editor/SelectTile.hpp"
#include "Editor/TabButton.hpp"

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

//Tile
std::unordered_set<RenderTile, RenderTileHash, RenderTileEqual> Tile;
static float TileX;
static float TileY;
// NOTE: Object below only have 1 at a time
// Mario

static sf::Sprite EDITOR_Mario(tempTex); // 128, 320
static sf::Sprite EDITOR_ExitGateIndicator(tempTex); // 256, 320
static sf::Sprite EDITOR_ExitGate(tempTex); // 384, 320

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
                    RenderTile tile(*ImageManager::GetReturnTexture(TilePage[CurrPage][CurrSelectTile].name), sf::Vector2f(TileX, TileY), CurrPage);
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
