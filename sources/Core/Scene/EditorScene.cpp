#include "Core/Scene/EditorScene.hpp"

#include "Block/Brick.hpp"
#include "Block/BulletLauncher.hpp"
#include "Block/LuckyBlock.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Background/Bg.hpp"
#include "Core/Background/BgGradient.hpp"
#include "Editor/Editor.hpp"
#include "Effect/BrickParticle.hpp"
#include "Effect/BroAIEffect.hpp"
#include "Effect/CoinEffect.hpp"
#include "Effect/FireballExplosion.hpp"
#include "Effect/GoombaAIEffect.hpp"
#include "Effect/MarioEffect.hpp"
#include "Object/BroAI.hpp"
#include "Object/BulletBill.hpp"
#include "Object/Coin.hpp"
#include "Object/ExitGate.hpp"
#include "Object/GoombaAI.hpp"
#include "Object/Mario.hpp"
#include "Object/PiranhaAI.hpp"
#include "Object/Platform.hpp"
#include "Object/Spike.hpp"
#include "Projectiles/ProjectileHelper.hpp"
#include "Text/Text.hpp"

EditorScene::EditorScene(SceneManager &manager) : Scene(manager) {}
void EditorScene::loadResources() {
    loadObstacleTemp();
    loadMarioRes();

    BrickParticleInit();
    BricksInitTemp();
    LuckyBLockTemp();
    GoombaAIInit();
    ProjectileInit();
    BroAILoadRes();
    BroAIEffectInit();
    CoinEffectInit();
    CoinInit();
    ScoreEffectInit();
    GoombaAIEffectInit();
    PiranhaAIInit();
    SpikeInit();
    MarioEffectInit();
    BgInit();
    ExitGateInit();
    FireballExplosionInit();
    BulletLauncherInit();
    BulletBillInit();
    PlatformInit();
    //
    SelectTileInit();
    EditorInit();
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
    ImageManager::DrawAllVertex();
}
void EditorScene::postUpdate() {
    PlaceTile();
}
void EditorScene::objectCleanup() {}
void EditorScene::setView() {
    view.setCenter({320.0f + EditorInterpolatedPos.x, 240.0f+ EditorInterpolatedPos.y});
}











