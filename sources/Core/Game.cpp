#include "Block/Brick.hpp"
#include "Block/LuckyBlock.hpp"
#include "Core/ImageManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Sound.hpp"
#include "Core/Music.hpp"
#include "Core/Scroll.hpp"
#include "Text/Text.hpp"
#include "Editor/Editor.hpp"
#include "Core/Game.hpp"
#include "Core/SceneManager.hpp"
#include "Core/Scene/EditorScene.hpp"
#include "Core/Scene/GameScene.hpp"

SceneManager sceneManager;

void GameObjectInit() {
    //Essential
    InitTempTex();
    windowInit();
    SoundInit();
    loadFontRes();
    MusicInit();
    ViewInit();
    //Scene
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
