#include "Core/Scene/EditorScene.hpp"

#include "Core/WindowFrame.hpp"
#include "Core/Background/BgGradient.hpp"
#include "Editor/Editor.hpp"
#include "Text/Text.hpp"

EditorScene::EditorScene(SceneManager &manager) : Scene(manager) {}
void EditorScene::loadResources() {
    AddText("_FPS", "", LEFT_MARGIN, 0.0f, 464.0f);
    AddText("_MOUSEXY", "", RIGHT_MARGIN, 624.0f, 464.0f);
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
    EditText(fmt::format("FPS: {}", static_cast<int>(fpsLite.getFps())), "_FPS");
    EditText(std::to_string(static_cast<int>(MouseX)) + "/" + std::to_string(static_cast<int>(MouseY)) + "  R", "_MOUSEXY");
}
void EditorScene::HUDPositionUpdate() {
    SelectedTilePosUpdate();
    SelectTilePosUpdate();
}
void EditorScene::draw(sf::RenderWindow &window) {
    BgGradientDraw();
    DrawTile();
    SelectTileDraw();
    TextDraw();
}
void EditorScene::postUpdate() {
    PlaceTile();
}
void EditorScene::objectCleanup() {}
void EditorScene::setView() {
    view.setCenter({320.0f + EditorInterpolatedPos.x, 240.0f+ EditorInterpolatedPos.y});
}











