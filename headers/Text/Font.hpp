#pragma once

#ifndef FONT_HPP
#define FONT_HPP

#include <SFML/Graphics.hpp>

class Font {
public:
    void SetFontSize(int TextSizeX, int TextSizeY, int sizeX, int sizeY);
    [[nodiscard]] sf::Vector2i GetFontTexture(char c) const;
    int GetFontIndex(char c);
    [[nodiscard]] sf::Vector2i GetFontSize() const;
private:
    int m_Xrow{};
    int m_Yrow{};
    int m_FontSizeX{};
    int m_FontSizeY{};
};

extern const std::string FontString;
//extern void SetFontSize(int TextSizeX, int TextSizeY, int sizeX, int sizeY);
//extern int GetFontIndex(char c);
//extern sf::Vector2i GetFontTexture(char c);
//extern int FontSizeX;
//extern int FontSizeY;

#endif // FONT_HPP