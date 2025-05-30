#pragma once

#ifndef WINDOWFRAME_HPP
#define WINDOWFRAME_HPP

#include <SFML/Audio.hpp>
#include "ExternalHeaders/Kairos.hpp"

extern bool isDebug;
extern sf::RenderWindow window;
extern sf::RenderTexture rObject;
extern sf::Sound Kick2Sound;
extern sf::View view;
extern int optionSmoothness;
extern kairos::Stopwatch Gclock;
extern kairos::Timestep timestep;
extern kairos::FpsLite fpsLite;
extern void updateFrame();
extern void HUDUpdate();
extern float Width;
extern float Height;
extern float MouseX;
extern float MouseY;
extern void windowInit();
extern void FrameDraw();
extern int hex_to_int(const std::string &hex);

extern float f_min(float a, float b);
extern float f_max(float a, float b);
extern float f_abs(float a);
extern float f_round(float val);

#endif // WINDOWFRAME_HPP