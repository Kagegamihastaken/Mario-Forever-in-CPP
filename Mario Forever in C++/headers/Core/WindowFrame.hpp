#pragma once

#ifndef WINDOWFRAME_HPP
#define WINDOWFRAME_HPP

#include <SFML/Audio.hpp>
#include "MultiThreading/ThreadPool.hpp"

extern bool isDebug;
extern sf::RenderWindow window;
extern sf::Sound Kick2Sound;
extern sf::View view;
extern sf::Clock delta;
extern sf::Clock fpsClock;
extern sf::Clock GameClock;
extern float deltaTime;
extern float fps;
extern int optionSmoothness;
extern void updateFrame();
extern float Width;
extern float Height;
extern float MouseX;
extern float MouseY;
extern void windowInit();
extern void FrameDraw();
extern int hex_to_int(std::string hex);

extern float f_min(float a, float b);
extern float f_max(float a, float b);
extern float f_abs(float a);
extern void OutputToWindow(std::string output);

#endif // WINDOWFRAME_HPP