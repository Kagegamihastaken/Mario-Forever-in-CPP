#pragma once

#ifndef WINDOWFRAME_HPP
#define WINDOWFRAME_HPP

extern bool isDebug;
extern sf::RenderWindow window;
extern sf::View view;
extern sf::Clock delta;
extern sf::Clock fpsClock;
extern sf::Clock GameClock;
extern float deltaTime;
extern float fps;
extern float optionSmoothness;
extern void updateFrame();
extern float Width;
extern float Height;
extern float MouseX;
extern float MouseY;

#endif // WINDOWFRAME_HPP