#include "Core/PauseEngine.hpp"

#include "Core/MusicManager.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Text/Text.hpp"

namespace PauseEngine {
    constexpr float PAUSE_TIME_LIMIT = 3.f; // 3 seconds

    void PauseEvent(const std::optional<sf::Event>& event) {
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            switch (keyPressed->code) {
                case sf::Keyboard::Key::P:
                    if (PauseTimeElapsed < PAUSE_TIME_LIMIT * 50.f) return;
                    MusicManager::PauseAllMusic();
                    SoundManager::PauseAllSound();
                    if (!isPaused)
                        SoundManager::PlaySound(SoundID::GAME_PAUSE, true);
                    else {
                        PauseTimeElapsed = 0.f;
                        SoundManager::PlaySound(SoundID::GAME_RESUME, true);
                    }
                    isPaused = !isPaused;
                    SetTextVisible("_PAUSED_ONE", isPaused);
                    SetTextVisible("_PAUSED_TWO", isPaused);
                    break;
                default: ;
            }
        }
    }
    bool isGamePaused() {
        return isPaused;
    }
    void PauseStatusUpdate(float deltaTime) {
        PauseTimeElapsed += deltaTime * (1.f / WindowFrame::getTimestep().getTimeSpeed());
        if (PauseTimeElapsed > PAUSE_TIME_LIMIT * 50.f) {
            PauseTimeElapsed = PAUSE_TIME_LIMIT * 50.f;
        }
    }
    void InitPauseEngine() {
        PauseBG[0].color = PauseBG[1].color = sf::Color(0, 0, 0, 128);
        PauseBG[2].color = PauseBG[3].color = sf::Color(0, 0, 0, 128);

        AddText("_PAUSED_ONE", "GAME PAUSED", TextMarginID::LEFT_MARGIN, 233.0f, 445.0f, 1, false);
        AddText("_PAUSED_TWO", "PRESS \"P\" TO RESUME GAME", TextMarginID::LEFT_MARGIN, 129.0f, 462.0f, 1, false);

        PauseTimeElapsed = 2.f * 50.f;
    }
    void PauseDraw() {
        if (!isPaused) return;

        PauseBG[0].position = sf::Vector2f(-32.f, -32.f) + Scroll::getViewPosition();
        PauseBG[1].position = sf::Vector2f(WindowFrame::getGameSize().x + 32.f, -32.f) + Scroll::getViewPosition();
        PauseBG[2].position = sf::Vector2f(-32.f, WindowFrame::getGameSize().y + 32.f) + Scroll::getViewPosition();
        PauseBG[3].position = sf::Vector2f(WindowFrame::getGameSize().x + 32.f, WindowFrame::getGameSize().y + 32.f) + Scroll::getViewPosition();

        WindowFrame::getWindow().draw(PauseBG);
    }
}
