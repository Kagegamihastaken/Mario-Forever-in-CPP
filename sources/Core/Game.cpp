#include "Core/ImageManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Sound.hpp"
#include "Core/Music.hpp"
#include "Core/Scroll.hpp"
#include "Text/Text.hpp"
#include "Editor/Editor.hpp"
#include "Core/Game.hpp"
#include "Core/Scene/EditorScene.hpp"
#include "Core/Scene/GameScene.hpp"

SceneManager Game::sceneManager;

void Game::Init() {
    //Essential
    InitTempTex();
    WindowFrame::Init();
    SoundInit();
    loadFontRes();
    MusicInit();
    Scroll::Init();
    //Scene
    sceneManager.addScene<GameScene>("Game");
    sceneManager.addScene<EditorScene>("Editor");
    sceneManager.changeScene("Game");
}
void Game::EditText() {
    sceneManager.textUpdate();
}
void Game::SetPrev() {
    sceneManager.setPreviousPosition();
}
void Game::DeltaMovement(const float dt) {
    sceneManager.update(dt);
}
void Game::RetrieveEvent(const std::optional<sf::Event>& event) {
    WindowFrame::Window::WindowEventUpdate(event);
    sceneManager.handleInput(event);
}
void Game::InterpolateMovement(const float alpha) {
    sceneManager.interpolatePosition(alpha);
}
void Game::Collision() {
    sceneManager.postUpdate();
}
void Game::UpdateView() {
    sceneManager.setView();
}
void Game::MiscUpdate() {
    Scroll::setWindowView();
    sceneManager.setView();
    WindowFrame::updateFrame();
    Scroll::updateView();
    UpdatePositionCharacter();
    sceneManager.HUDPositionUpdate();
}
void Game::Draw() {
    sceneManager.draw(WindowFrame::getWindow());
}
void Game::Cleanup() {
    sceneManager.objectCleanup();
}
