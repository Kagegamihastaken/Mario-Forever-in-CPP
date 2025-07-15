#pragma once
#ifndef PLATFORM_HPP
#define PLATFORM_HPP
#include "Core/Class/PlatformClass.hpp"
#include "Core/ExternalHeaders/plf_colony.h"
extern plf::colony<MFCPP::Platform> PlatformList;
extern void PlatformInit();
extern void AddPlatform(const sf::Vector2f& start, const sf::Vector2f& end);
extern void DrawPlatform();
extern void SetPrevPlatformPos();
extern void InterpolatePlatformPos(float alpha);
extern void PlatformPositionUpdate(float deltaTime);
extern void PlatformCleanup();
extern void PlatformStatusUpdate();

#endif //PLATFORM_HPP
