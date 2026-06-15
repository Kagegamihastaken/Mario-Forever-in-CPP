#pragma once

#ifndef WINDOWFRAME_HPP
#define WINDOWFRAME_HPP

#include "Animate/SingleAnimationObject.hpp"
#include "Animate/StaticAnimationObject.hpp"
#include "ExternalHeaders/Kairos.hpp"

class WindowFrame {
public:
    class Window {
    public:
        Window() = delete;
        Window(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = delete;

        static uint8_t WindowGetScale();
        static void WindowToggleFullscreen();
        static void WindowToggleSmooth();
        static void WindowSetSmooth(bool val);
        static void WindowSetFullscreen(bool val);
        static void WindowSetScale(uint8_t val);
        static void ChangeScreenMode(uint8_t mode = 0);
        static void WindowEventUpdate(const std::optional<sf::Event>& event);
    private:
        static sf::VideoMode WindowGetVideoMode();
        static sf::VideoMode x1;
        static sf::VideoMode x15;
        static sf::VideoMode x2;
        const static std::vector<sf::VideoMode> Resolutions;

        static bool OPTION_SMOOTH;
        static bool OPTION_VSYNC;
        static bool OPTION_FULLSCREEN;

        static bool PREV_OPTION_FULLSCREEN;
        const static std::string PROGRAM_NAME;
        static uint8_t OPTION_SCALE;
    };
    WindowFrame() = delete;
    WindowFrame(const WindowFrame&) = delete;
    WindowFrame(WindowFrame&&) = delete;
    WindowFrame& operator=(const WindowFrame&) = delete;
    WindowFrame& operator=(WindowFrame&&) = delete;

    static void WindowInit();
    static void GameSceneInit();
    static void FrameDraw();
    static void updateFrame();
    [[nodiscard]] static sf::RenderWindow& getWindow();
    [[nodiscard]] static kairos::Timestep& getTimestep();
    [[nodiscard]] static kairos::FpsLite& getFpsLite();
    [[nodiscard]] static bool isDebug();
    [[nodiscard]] static sf::Vector2f getMousePosition();
    [[nodiscard]] static sf::Vector2f getGameSize();
private:
    static bool _Debug;

    static kairos::FpsLite fpsLite;
    static kairos::Timestep timestep;
    static sf::Image icon;
    static sf::RenderWindow window;
    static sf::Vector2f MousePosition;
    static sf::Vector2f GameSize;

    static MFCPP::StaticAnimationObject MarioHUD;
    static MFCPP::SingleAnimationObject CoinHUD;
    static MFCPP::StaticAnimationObject TimeHUD;
};
namespace Window {
}
extern int hex_to_int(const std::string &hex);
extern float f_mod(float a, float b);
extern float f_min(float a, float b);
extern float f_max(float a, float b);
extern float f_abs(float a);
extern float f_round(float val);
extern int RandomIntNumberGenerator(int a, int b);
extern float RandomFloatNumberGenerator(float a, float b);
extern float VectorGetLength(const sf::Vector2f& val);
extern sf::Vector2f VectorNormalized(const sf::Vector2f& val);

#endif // WINDOWFRAME_HPP