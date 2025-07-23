#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <cmake-build-relwithdebinfo-mingw/config/config.h>

#include "RenderTile.hpp"
#include "Core/Hash.hpp"
#include "Core/ImageManager.hpp"
#include "Core/SimpleSprite.hpp"
#include "SFML/Window/Event.hpp"

extern sf::Vector2f EditorPos;
extern sf::Vector2f EditorInterpolatedPos;

extern void AlphaUpdate(float& alpha, bool& state, float min, float max, float change, float dt);
extern void PlaceTile();
extern void DrawTile();
extern void EditorScreenMove(float dt);
extern void SetPrevEditor();
extern void InterpolateEditorPos(float alpha);
extern void EditorEvent(const std::optional<sf::Event>& event);
extern void TilePosUpdate(float dt);
extern void SelectedTilePosUpdate();
extern void EditorInit();
extern void SettingDialog();

static float lastPlaceX = -1.0f;
static float lastPlaceY = -1.0f;
static float lastDeleteX = -1.0f;
static float lastDeleteY = -1.0f;

// Editor Position
inline sf::Vector2f EditorPos;
inline sf::Vector2f EditorPrevPos;
inline sf::Vector2f EditorInterpolatedPos;

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
//Selected Block HUD
static sf::Sprite SelectedBlock(tempTex);

//Tile
inline std::unordered_set<RenderTile, RenderTileHash, RenderTileEqual> Tile;
static float TileX;
static float TileY;

// NOTE: Object below only have 1 at a time
// Mario
static MFCPP::SimpleSprite EDITOR_Mario; // 128, 320
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
static bool EDITOR_ShowProperty = false;

//Build Mode
static bool EDITOR_BuildMode = ON;

//Save & Load
static bool EDITOR_OpenDialog = false;
static bool EDITOR_SaveDialog = false;

//Setting
static bool EDITOR_Setting = false;
static CustomTileProperty BgColor(ColorProps("First Background Color", MFCPP::Color::LevelDefaultFirst), ColorProps("Second Background Color", MFCPP::Color::LevelDefaultSecond));
static CustomTileProperty LevelSize(FloatProps("Level Width", 640.f, 640.f, 32000.f), FloatProps("Level Height", 480.f, 480.f, 32000.f));
//SaveTile For Edit Property
static RenderTile SaveTile;

#endif //EDITOR_HPP
