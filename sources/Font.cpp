#include <SFML/Graphics.hpp>
#include "../headers/Text/Font.hpp"
#include "../headers/Core/TextureManager.hpp"

#include <string>
#include <algorithm>

int Xrow;
int Yrow;
int FontSizeX;
int FontSizeY;
TextureManager FontTextureList;
std::string FontString = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`{|}";

void SetFontSize(int TextSizeX, int TextSizeY, int sizeX, int sizeY) {
	Xrow = TextSizeX / sizeX;
	Yrow = TextSizeY / sizeY;
	FontSizeX = sizeX;
	FontSizeY = sizeY;
}
sf::Vector2i GetFontTexture(char c) {
	if (c >= 'a' && c <= 'z') c -= 32;
	int index = std::find(FontString.begin(), FontString.end(), c) - FontString.begin();
	int Divided = index / Xrow;
	return { (index % Xrow) * FontSizeX, Divided * FontSizeY };
}
int GetFontIndex(char c) {
	if (c >= 'a' && c <= 'z') c -= 32;
	int index = std::find(FontString.begin(), FontString.end(), c) - FontString.begin();
	return index;
}