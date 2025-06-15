#include <SFML/Graphics.hpp>
#include "Text/Font.hpp"

#include <string>
#include <algorithm>

const std::string FontString = R"( !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`{|})";

void Font::SetFontSize(const int TextSizeX, const int TextSizeY, const int sizeX, const int sizeY) {
	m_FontSizeX = sizeX;
	m_FontSizeY = sizeY;
	m_Xrow = TextSizeX / sizeX;
	m_Yrow = TextSizeY / sizeY;
}
sf::Vector2i Font::GetFontTexture(const char c) const {
	char character = c;
	if (character >= 'a' && character <= 'z') character -= 32;
	const long long index = std::ranges::find(FontString, character) - FontString.begin();
	const long long Divided = index / m_Xrow;
	return { static_cast<int>(index % m_Xrow) * m_FontSizeX, static_cast<int>(Divided) * m_FontSizeY };
}
int Font::GetFontIndex(const char c) {
	char character = c;
	if (character >= 'a' && character <= 'z') character -= 32;
	const int index = std::ranges::find(FontString, character) - FontString.begin();
	return index;
}
sf::Vector2i Font::GetFontSize() const {
	return {m_FontSizeX, m_FontSizeY};
}