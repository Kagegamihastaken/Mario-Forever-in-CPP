#include <SFML/Graphics.hpp>
#include "../headers/Text/Font.hpp"
#include "../headers/Text/Text.hpp"
#include "../headers/Core/Scroll.hpp"
#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Core/Loading/Loading.hpp"
#include "../headers/Core/Loading/enum.hpp"
#include "../headers/Core/TextureManager.hpp"

#include "../resource.h"

#include <vector>
#include <iostream>
#include <string>

std::vector<Text*> TextList;
std::vector<TextMarginID> TextMarginList;

void loadFontRes() {
	FontTextureList.Loadingtexture(FONT_TEXTURE, "Font", 0, 0, 645, 32);
	SetFontSize(645, 32, 15, 16);
}
void AddText(std::string id, std::string text, TextMarginID margin, float x, float y) {
	//search for duplicates ID
	bool isDuplicate = false;
	for (const auto& i : TextList) {
		if (i->id == id) {
			isDuplicate = true;
		}
	}
	if (!isDuplicate) {
		// If not proceed as usual
		Text* Init = new Text();
		Init->x = x;
		Init->y = y;
		Init->id = id;
		Init->textContent = text;
		int Counter = 0;
		sf::Vector2i lp;
		for (const auto& i : text) {
			lp = GetFontTexture(i);
			Init->text.push_back(sf::Sprite(*FontTextureList.GetTexture("Font"), sf::IntRect({ lp.x, lp.y }, { 15, 16 })));
			Init->text[Counter].setPosition({ x + FontSizeX * Counter + Counter, y });
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
		if (i->id == id) {
			isFounded = true;
			break;
		}
		++IndexCounter;
	}
	if (isFounded) {
		std::vector<sf::Sprite> Init;
		sf::Vector2i lp;
		int Counter = 0;
		for (const auto& i : NewText) {
			lp = GetFontTexture(i);
			Init.push_back(sf::Sprite(*FontTextureList.GetTexture("Font"), sf::IntRect({ lp.x, lp.y }, { 15, 16 })));
			Init[Counter].setPosition({ TextList[IndexCounter]->x + FontSizeX * Counter, TextList[IndexCounter]->y });
			++Counter;
		}
		TextList[IndexCounter]->text = Init;
		TextList[IndexCounter]->textContent = NewText;
	}
	else std::cout << "Cannot edit text ID " << id << " (Not existed ID)" << "\n";
}
inline void EditPosition(float NewX, float NewY, std::string id) {
	bool isFounded = false;
	int IndexCounter = 0;
	for (const auto& i : TextList) {
		if (i->id == id) {
			isFounded = true;
			break;
		}
		++IndexCounter;
	}
	if (isFounded) {
		TextList[IndexCounter]->x = NewX;
		TextList[IndexCounter]->y = NewY;
	}
	else std::cout << "Cannot edit position ID " << id << " (Not existed ID)" << "\n";
}
inline void UpdatePositionCharacter() {
	int iTextSize;
	for (int i = 0; i < TextList.size(); ++i) {
		iTextSize = static_cast<int>(TextList[i]->text.size());
		for (int j = 0; j < iTextSize; ++j) {
			if (TextMarginList[i] == LEFT_MARGIN) TextList[i]->text[j].setPosition({ TextList[i]->x + j + FontSizeX * j + view.getCenter().x - (Width / 2.0f), TextList[i]->y + view.getCenter().y - (Height / 2.0f) });
			else if (TextMarginList[i] == RIGHT_MARGIN) TextList[i]->text[j].setPosition({ TextList[i]->x + view.getCenter().x - (Width / 2.0f) - (TextList[i]->text.size() - 1 - j) * FontSizeX - (iTextSize - 1 - j), TextList[i]->y + view.getCenter().y - (Height / 2.0f) });
		}
	}
}
inline void UpdateText() {
	for (const auto& i : TextList) {
		for (const auto& j : i->text) {
			rTexture.draw(j);
		}
	}
}
inline int getSizeText(std::string id) {
	bool isFounded = false;
	int IndexCounter = 0;
	for (const auto& i : TextList) {
		if (i->id == id) {
			isFounded = true;
			break;
		}
		++IndexCounter;
	}
	if (isFounded) {
		return static_cast<int>(TextList[IndexCounter]->textContent.size());
	}
	else {
		std::cout << "Cannot get text ID " << id << " (Not existed ID)" << "\n";
		return -1;
	}
}