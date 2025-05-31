#include <SFML/Graphics.hpp>
#include "Text/Font.hpp"
#include "Core/ImageManager.hpp"

#include <string>
#include <algorithm>

int Xrow;
int Yrow;
int FontSizeX;
int FontSizeY;
std::string FontString = R"( !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`{|})";

void SetFontSize(const int TextSizeX, const int TextSizeY, const int sizeX, const int sizeY) {
	Xrow = TextSizeX / sizeX;
	Yrow = TextSizeY / sizeY;
	FontSizeX = sizeX;
	FontSizeY = sizeY;
}
sf::Vector2i GetFontTexture(char c) {
	if (c >= 'a' && c <= 'z') c -= 32;
	const int index = std::ranges::find(FontString, c) - FontString.begin();
	const int Divided = index / Xrow;
	return { (index % Xrow) * FontSizeX, Divided * FontSizeY };
}
int GetFontIndex(char c) {
	if (c >= 'a' && c <= 'z') c -= 32;
	const int index = std::ranges::find(FontString, c) - FontString.begin();
	return index;
}