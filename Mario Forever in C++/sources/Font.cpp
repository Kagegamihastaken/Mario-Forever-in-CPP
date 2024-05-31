#include <SFML/Graphics.hpp>
#include "../headers/Font.hpp"

#include <string>
#include <algorithm>

int Xrow;
int Yrow;
int FontSizeX;
int FontSizeY;

sf::Texture FontTexture;
std::string FontString = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`{|}";

void SetFontSize(int sizeX, int sizeY) {
	Xrow = FontTexture.getSize().x / sizeX;
	Yrow = FontTexture.getSize().y / sizeY;
	FontSizeX = sizeX;
	FontSizeY = sizeY;
}
std::pair<int, int> GetFontTexture(char c) {
	if (c >= 'a' && c <= 'z') c -= 32;
	int index = std::find(FontString.begin(), FontString.end(), c) - FontString.begin();
	int Divided = index / Xrow;
	return { (index % Xrow) * FontSizeX, Divided * FontSizeY };
}