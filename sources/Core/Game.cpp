#include "Core/WindowFrame.hpp"
#include "Core/Scroll.hpp"
#include "Text/Text.hpp"
#include "Editor/Editor.hpp"
#include "Core/Game.hpp"
#include "Core/Scene/EditorScene.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Core/Profiler.hpp"
#include "Core/Config/SoundsConfig.hpp"
#include "Core/Config/MusicsConfig.hpp"

SceneManager g_sceneManager;

void Game::Init() {
    //Essential
    WindowFrame::Init();
    MFCPP::SoundsConfig::loadFile("data/properties/Sounds.toml");
    MFCPP::MusicsConfig::loadFile("data/properties/Musics.toml");
    //SoundInit();
    //MusicInit();
    Scroll::Init();
    //Scene
    g_sceneManager.addScene<GameScene>(SceneManager::SceneState::GAMEPLAY);
    g_sceneManager.addScene<EditorScene>(SceneManager::SceneState::EDITOR);
    g_sceneManager.changeScene(SceneManager::SceneState::GAMEPLAY);
}
void Game::EditText() {
    ZoneScopedNC("Game::EditText", 0x00d0ff);
    g_sceneManager.textUpdate();
}
void Game::SetPrev() {
    g_sceneManager.setPreviousPosition();
}
void Game::DeltaMovement(const float dt) {
    ZoneScopedNC("Game::DeltaMovement", 0x0d00ff);
    g_sceneManager.update(dt);
}
void Game::RetrieveEvent(const std::optional<sf::Event>& event) {
    ZoneScopedNC("Game::RetrieveEvent", 0xa600ff);
    g_sceneManager.handleInput(event);
    if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        switch (keyPressed->code) {
            case sf::Keyboard::Key::P:
                if (levelId == 0) levelId = 1;
                else if (levelId == 1) levelId = 0;
                g_sceneManager.reloadCurrentScene();
                break;
            default: ;
        }
    }
}
void Game::InterpolateMovement(float alpha) {
    ZoneScopedNC("Game::InterpolateMovement", 0x00FF6B);
    g_sceneManager.interpolatePosition(alpha);
}
void Game::Collision() {
    ZoneScopedNC("Game::Collision", 0xff00f7);
    g_sceneManager.postUpdate();
}
void Game::UpdateView() {
    ZoneScopedNC("Game::UpdateView", 0x008C3A);
    g_sceneManager.setView();
}
void Game::MiscUpdate() {
    ZoneScopedNC("Game::MiscUpdate", 0xbbff00);
    Scroll::setWindowView();
    WindowFrame::updateFrame();
    g_sceneManager.setView();
    Scroll::updateView();
    UpdatePositionCharacter();
    g_sceneManager.HUDPositionUpdate();
}
void Game::Draw(float alpha) {
    ZoneScopedNC("Game::Draw", 0xffee00);
    g_sceneManager.draw(WindowFrame::getWindow(), alpha);
}
void Game::Cleanup() {
    ZoneScopedNC("Game::Cleanup", 0xff8400);
    g_sceneManager.objectCleanup();
}
void Game::Deinit() {
    MFCPP::Log::InfoPrint("Game Deinit...");
    g_sceneManager.unload();
}
