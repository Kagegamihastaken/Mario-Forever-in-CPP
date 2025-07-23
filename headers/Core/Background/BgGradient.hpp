#pragma once

#ifndef BGGRADIENT_HPP
#define BGGRADIENT_HPP
#include <imgui.h>

extern sf::VertexArray bgGradient;

extern void BgGradientInitPos(float Width, float Height);
extern void BgGradientDraw();
extern void BgGradientSetColor(const sf::Color& firstC, const sf::Color& secondC);
#endif