#pragma once

#ifndef WINDOWFRAME_HPP
#define WINDOWFRAME_HPP

#include "ExternalHeaders/Kairos.hpp"

namespace Window {
    extern int WindowGetScale();
    extern void WindowToggleFullscreen();
    extern void WindowToggleSmooth();
    extern void WindowSetSmooth(bool val);
    extern void WindowSetFullscreen(bool val);
    extern void WindowSetScale(int val);
    extern void ChangeScreenMode(unsigned int mode = 0);
    extern void WindowEventUpdate(const std::optional<sf::Event>& event);
}
extern bool isDebug;
extern sf::VideoMode videoMode;
extern sf::RenderWindow window;
extern sf::RenderTexture rObject;
extern sf::Sound Kick2Sound;
extern sf::View view;
extern const std::vector<sf::VideoMode> Resolutions;
extern kairos::Stopwatch Gclock;
extern kairos::Timestep timestep;
extern kairos::FpsLite fpsLite;
extern void updateFrame();
extern void HUDUpdate();
extern std::random_device seed;
extern float Width;
extern float Height;
extern float MouseX;
extern float MouseY;
extern void windowInit();
extern void GameSceneInit();
extern void FrameDraw();
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