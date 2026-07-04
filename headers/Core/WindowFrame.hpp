#pragma once

#ifndef WINDOWFRAME_HPP
#define WINDOWFRAME_HPP

#include <config.h>

#include "Animate/SingleAnimationObject.hpp"
#include "Animate/StaticAnimationObject.hpp"
#include "ExternalHeaders/Kairos.hpp"

namespace WindowFrame {
#if defined _DEBUG
    static bool _Debug = true;
#else
    static bool _Debug = false;
#endif
    //Animation Define
    static constexpr uint16_t COINHUD_IMAGE_WIDTH = 86;
    static constexpr uint16_t COINHUD_WIDTH = 28;
    static constexpr uint16_t COINHUD_HEIGHT = 16;

    static kairos::FpsLite fpsLite;
    static kairos::Timestep timestep;
    static sf::Image icon;
    static sf::RenderWindow window;
    static sf::Vector2f MousePosition;
    static sf::Vector2f GameSize(GAME_WIDTH, GAME_HEIGHT);

    static MFCPP::StaticAnimationObject MarioHUD;
    static MFCPP::SingleAnimationObject CoinHUD;
    static MFCPP::StaticAnimationObject TimeHUD;

    namespace Window {
        uint8_t WindowGetScale();
        void WindowToggleFullscreen();
        void WindowToggleSmooth();
        void WindowSetSmooth(bool val);
        void WindowSetFullscreen(bool val);
        void WindowSetScale(uint8_t val);
        void ChangeScreenMode(uint8_t mode = 0);
        void WindowEventUpdate(const std::optional<sf::Event>& event);

        static sf::VideoMode WindowGetVideoMode();
        static sf::VideoMode x1({ static_cast<uint32_t>(GameSize.x), static_cast<uint32_t>(GameSize.y) });
        static sf::VideoMode x15({ static_cast<uint32_t>(GameSize.x * 1.5f), static_cast<uint32_t>(GameSize.y * 1.5f) });
        static sf::VideoMode x2({ static_cast<uint32_t>(GameSize.x * 2.0f), static_cast<uint32_t>(GameSize.y * 2.0f) });
        const static std::vector Resolutions{sf::VideoMode::getFullscreenModes()};

        static bool OPTION_SMOOTH{true};
        static bool OPTION_VSYNC{false};
        static bool OPTION_FULLSCREEN{true};

        static bool PREV_OPTION_FULLSCREEN{OPTION_FULLSCREEN};
        const static std::string PROGRAM_NAME{"MFCPP::Mario-Forever"};
        static uint8_t OPTION_SCALE{0};
    };
    void Init();
    void GameSceneInit();
    void FrameDraw();
    void updateFrame();
    [[nodiscard]] sf::RenderWindow& getWindow();
    [[nodiscard]] kairos::Timestep& getTimestep();
    [[nodiscard]] kairos::FpsLite& getFpsLite();
    [[nodiscard]] bool isDebug();
    [[nodiscard]] sf::Vector2f getMousePosition();
    [[nodiscard]] sf::Vector2f getGameSize();
};

#endif // WINDOWFRAME_HPP