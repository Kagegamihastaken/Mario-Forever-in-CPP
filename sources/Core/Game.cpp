#include "Core/WindowFrame.hpp"
#include "Core/Sound.hpp"
#include "Core/Music.hpp"
#include "Core/Scroll.hpp"
#include "Text/Text.hpp"
#include "Editor/Editor.hpp"
#include "Core/Game.hpp"
#include "Core/Scene/EditorScene.hpp"
#include "Core/Scene/GameScene.hpp"

SceneManager g_sceneManager;

void Game::Init() {
    //Essential
    WindowFrame::Init();
    SoundInit();
    loadFontRes();
    MusicInit();
    Scroll::Init();
    //Scene
    g_sceneManager.addScene<GameScene>(SceneManager::SceneState::GAMEPLAY);
    g_sceneManager.addScene<EditorScene>(SceneManager::SceneState::EDITOR);
    g_sceneManager.changeScene(SceneManager::SceneState::GAMEPLAY);
}
void Game::EditText() {
    g_sceneManager.textUpdate();
}
void Game::SetPrev() {
    g_sceneManager.setPreviousPosition();
}
void Game::DeltaMovement(const float dt) {
    g_sceneManager.update(dt);
}
void Game::RetrieveEvent(const std::optional<sf::Event>& event) {
    WindowFrame::Window::WindowEventUpdate(event);
    g_sceneManager.handleInput(event);
}
void Game::InterpolateMovement(float alpha) {
    g_sceneManager.interpolatePosition(alpha);
}
void Game::Collision() {
    g_sceneManager.postUpdate();
}
void Game::UpdateView() {
    g_sceneManager.setView();
}
void Game::MiscUpdate() {
    Scroll::setWindowView();
    WindowFrame::updateFrame();
    g_sceneManager.setView();
    Scroll::updateView();
    UpdatePositionCharacter();
    g_sceneManager.HUDPositionUpdate();
}
void Game::Draw(float alpha) {
    g_sceneManager.draw(WindowFrame::getWindow(), alpha);
}
void Game::Cleanup() {
    g_sceneManager.objectCleanup();
}
void Game::Deinit() {
    MFCPP::Log::InfoPrint("Game Deinit...");
    g_sceneManager.unload();
}
