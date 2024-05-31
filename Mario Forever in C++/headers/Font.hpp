#pragma once

#ifndef FONT_HPP
#define FONT_HPP

#include <vector>
#include <SFML/Graphics.hpp>

extern sf::Texture FontTexture;
extern std::string FontString;
extern void SetFontSize(int sizeX, int sizeY);
extern std::pair<int, int> GetFontTexture(char c);
extern int FontSizeX;
extern int FontSizeY;

#endif // FONT_HPP