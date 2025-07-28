#include "Block/Brick.hpp"
#include "Block/LuckyBlock.hpp"
#include "Core/ImageManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Sound.hpp"
#include "Block/Obstacles.hpp"
#include "Core/Level.hpp"
#include "Core/Music.hpp"
#include "Core/Scroll.hpp"
#include "Core/Background/Bg.hpp"
#include "Effect/BrickParticle.hpp"
#include "Effect/CoinEffect.hpp"
#include "Effect/GoombaAIEffect.hpp"
#include "Effect/MarioEffect.hpp"
#include "Object/Coin.hpp"
#include "Object/ExitGate.hpp"
#include "Object/GoombaAI.hpp"
#include "Object/Mario.hpp"
#include "Object/PiranhaAI.hpp"
#include "Object/Spike.hpp"
#include "Text/Text.hpp"
#include "Core/Scene.hpp"
#include "Editor/Editor.hpp"
#include "Editor/SelectTile.hpp"
#include "Object/BroAI.hpp"
#include "Effect/FireballExplosion.hpp"
#include "Core/Game.hpp"

#include "Block/BulletLauncher.hpp"
#include "Core/SceneManager.hpp"
#include "Core/Scene/EditorScene.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Effect/BroAIEffect.hpp"
#include "Object/BulletBill.hpp"
#include "Object/Platform.hpp"
#include "Projectiles/ProjectileHelper.hpp"

SceneManager sceneManager;

void GameObjectInit() {
    InitTempTex();
    windowInit();
    SoundInit();
    loadFontRes();
    MusicInit();
    loadObstacleRes();
    loadMarioRes();
    BrickParticleInit();
    LoadBricks();
    LoadLuckyBlock();
    GoombaAILoadRes();
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
    ViewInit();
    FireballExplosionInit();
    BulletLauncherInit();
    BulletBillInit();
    PlatformInit();

    EditorInit();
    SelectTileInit();

    sceneManager.addScene<GameScene>("Game");
    sceneManager.addScene<EditorScene>("Editor");
    sceneManager.changeScene("Game");
}
void GameObjectEditText() {
    sceneManager.textUpdate();
}
void GameObjectSetPrev() {
    sceneManager.setPreviousPosition();
}
void GameObjectDeltaMovement(const float dt) {
    sceneManager.update(dt);
}
void GameObjectRetrieveEvent(const std::optional<sf::Event>& event) {
    Window::WindowEventUpdate(event);
    sceneManager.handleInput(event);
}
void GameObjectInterpolateMovement(const float alpha) {
    sceneManager.interpolatePosition(alpha);
}
void GameObjectCollision() {
    sceneManager.postUpdate();
}
void GameObjectMiscUpdate() {
    WindowSetView();
    sceneManager.setView();
    updateFrame();
    updateView();
    UpdatePositionCharacter();
    sceneManager.HUDPositionUpdate();
}
void GameObjectDraw() {
    sceneManager.draw(window);
}
void GameCleanUp() {
    sceneManager.objectCleanup();
    // if (CurrentScene == SceneID::SCENE_GAMEPLAY) {
    //     //TODO: Change to plf colony
    // }
}
