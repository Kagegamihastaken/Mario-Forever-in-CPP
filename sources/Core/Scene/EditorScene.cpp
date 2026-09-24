#include "Core/Scene/EditorScene.hpp"

#include "Block/Obstacles.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Background/Bg.hpp"
#include "Core/Background/BgGradient.hpp"
#include "Editor/Editor.hpp"
#include "Effect/MarioEffect.hpp"
#include "Object/Coin.hpp"
#include "Object/ExitGate.hpp"
#include "Object/Mario.hpp"
#include "Text/Text.hpp"
#include "Core/Scroll.hpp"
#include "Core/TextureConfig.hpp"

EditorScene::EditorScene(SceneManager &manager) : Scene(manager) {}
void EditorScene::loadResources() {
    loadObstacleTemp();
    MFCPP::TextureConfig::loadFile("data/properties/Textures.toml");
    MFCPP::TextureConfig::loadFile("data/editor/EditorTextures.toml");
    CoinInit();
    ForceLoadCoinTexture();
    MarioEffectInit();
    BgInit();
    ExitGateInit();
    //
    SelectTileInit();
    EditorInit();
    AddText("_FPS", "", TextMarginID::LEFT_MARGIN, 0.0f, 464.0f);
    AddText("_MOUSEXY", "", TextMarginID::RIGHT_MARGIN, 624.0f, 464.0f);
}
void EditorScene::unloadResources() {}
void EditorScene::handleInput(const std::optional<sf::Event> &event) {
    EditorEvent(event);
}
void EditorScene::update(const float deltaTime) {
    EditorScreenMove(deltaTime);
    TilePosUpdate(deltaTime);
    SelectTileAlphaUpdate(deltaTime);
}
void EditorScene::interpolatePosition(const float alpha) {
    InterpolateEditorPos(alpha);
}
void EditorScene::setPreviousPosition() {
    SetPrevEditor();
}
void EditorScene::textUpdate() {
    EditText(fmt::format("FPS: {}", static_cast<int>(WindowFrame::getFpsLite().getFps())), "_FPS");
    EditText(std::to_string(static_cast<int>(WindowFrame::getMousePosition().x)) + "/" + std::to_string(static_cast<int>(WindowFrame::getMousePosition().y)) + "  R", "_MOUSEXY");
}
void EditorScene::HUDPositionUpdate() {
    SelectedTilePosUpdate();
    SelectTilePosUpdate();
}
void EditorScene::draw(sf::RenderWindow &window, float alpha) {
    BgGradientDraw();
    DrawTile();
    SelectTileDraw();
    TextDraw();
    //ImageManager::DrawAllVertex();
}
void EditorScene::postUpdate() {
    PlaceTile();
}
void EditorScene::objectCleanup() {}
void EditorScene::setView() {
    Scroll::getView().setCenter({320.0f + EditorInterpolatedPos.x, 240.0f+ EditorInterpolatedPos.y});
}











