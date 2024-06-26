#include <SFML/Graphics.hpp>
#include "../headers/Font.hpp"
#include "../headers/Text.hpp"
#include "../headers/Scroll.hpp"
#include "../headers/WindowFrame.hpp"
#include "../headers/Loading.hpp"
#include "../headers/enum.hpp"

#include "../resource.h"

#include <vector>
#include <iostream>
#include <string>

std::vector<Text> TextList;
std::vector<TextMarginID> TextMarginList;

int loadFontRes() {
	LoadTexture(FontTexture, FONT_TEXTURE, IMAGEFILE);
	SetFontSize(15, 16);
	return 6;
}
int FontInit = loadFontRes();
void AddText(std::string id, std::string text, TextMarginID margin, float x, float y) {
	//search for duplicates ID
	bool isDuplicate = false;
	for (const auto& i : TextList) {
		if (i.id == id) {
			isDuplicate = true;
		}
	}
	if (!isDuplicate) {
		// If not proceed as usual
		Text Init;
		Init.x = x;
		Init.y = y;
		Init.id = id;
		Init.textContent = text;
		std::pair<int, int> Pos;
		int Counter = 0;
		for (const auto& i : text) {
			Pos = GetFontTexture(i);
			Init.text.push_back(sf::Sprite(FontTexture, sf::IntRect(Pos.first, Pos.second, FontSizeX, FontSizeY)));
			Init.text[Counter].setPosition(x + FontSizeX * Counter, y);
			++Counter;
		}
		TextList.push_back(Init);
		TextMarginList.push_back(margin);
	}
	else std::cout << "Cannot add text ID " << id << " (Duplicated ID)" << "\n";
}
inline void EditText(std::string NewText, std::string id) {
	bool isFounded = false;
	int IndexCounter = 0;
	for (const auto& i : TextList) {
		if (i.id == id) {
			isFounded = true;
			break;
		}
		++IndexCounter;
	}
	if (isFounded) {
		std::vector<sf::Sprite> Init;
		std::pair<int, int> Pos;
		int Counter = 0;
		for (const auto& i : NewText) {
			Pos = GetFontTexture(i);
			Init.push_back(sf::Sprite(FontTexture, sf::IntRect(Pos.first, Pos.second, FontSizeX, FontSizeY)));
			Init[Counter].setPosition(TextList[IndexCounter].x + FontSizeX * Counter, TextList[IndexCounter].y);
			++Counter;
		}
		TextList[IndexCounter].text = Init;
		TextList[IndexCounter].textContent = NewText;
	}
	else std::cout << "Cannot edit text ID " << id << " (Not existed ID)" << "\n";
}
inline void EditPosition(float NewX, float NewY, std::string id) {
	bool isFounded = false;
	int IndexCounter = 0;
	for (const auto& i : TextList) {
		if (i.id == id) {
			isFounded = true;
			break;
		}
		++IndexCounter;
	}
	if (isFounded) {
		TextList[IndexCounter].x = NewX;
		TextList[IndexCounter].y = NewY;
	}
	else std::cout << "Cannot edit position ID " << id << " (Not existed ID)" << "\n";
}
inline void UpdatePositionCharacter() {
	int iTextSize;
	for (int i = 0; i < TextList.size(); ++i) {
		iTextSize = TextList[i].text.size();
		for (int j = 0; j < iTextSize; ++j) {
			if (TextMarginList[i] == LEFT_MARGIN) TextList[i].text[j].setPosition(TextList[i].x + FontSizeX * j + view.getCenter().x - (Width / 2.0f), TextList[i].y + view.getCenter().y - (Height / 2.0f));
			else if (TextMarginList[i] == RIGHT_MARGIN) TextList[i].text[j].setPosition(TextList[i].x + view.getCenter().x - (Width / 2.0f) - (TextList[i].text.size() - 1 - j) * FontSizeX, TextList[i].y + view.getCenter().y - (Height / 2.0f));
		}
	}
}
inline void UpdateText() {
	for (const auto& i : TextList) {
		for (const auto& j : i.text) {
			window.draw(j);
		}
	}
}
inline int getSizeText(std::string id) {
	bool isFounded = false;
	int IndexCounter = 0;
	for (const auto& i : TextList) {
		if (i.id == id) {
			isFounded = true;
			break;
		}
		++IndexCounter;
	}
	if (isFounded) {
		return TextList[IndexCounter].textContent.size();
	}
	else {
		std::cout << "Cannot get text ID " << id << " (Not existed ID)" << "\n";
		return -1;
	}
}