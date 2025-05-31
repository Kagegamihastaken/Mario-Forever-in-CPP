#include <SFML/Graphics.hpp>
#include "Text/Font.hpp"
#include "Text/Text.hpp"
#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Loading/Loading.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/ImageManager.hpp"

#include <vector>
#include <iostream>
#include <string>

std::vector<Text*> TextList;
std::vector<TextMarginID> TextMarginList;

void loadFontRes() {
	SetFontSize(645, 32, 15, 16);
	ImageManager::AddImage("FontImage", "data/resources/Font.png");
	for (const auto &i : FontString) {
		ImageManager::AddTexture("FontImage", sf::IntRect({ GetFontTexture(i).x, GetFontTexture(i).y }, { 15, 16 }), "Font_" + std::to_string(i));
	}
	//FontTextureList.Loadingtexture("data/resources/Font.png", "Font", 0, 0, 645, 32);
}
void AddText(const std::string &id, const std::string &text, const TextMarginID margin, const float x, float y) {
	//search for duplicates ID
	bool isDuplicate = false;
	for (const auto& i : TextList) {
		if (i->id == id) {
			isDuplicate = true;
		}
	}
	if (!isDuplicate) {
		// If not, proceed as usual

		// LEAKED
		const auto Init = new Text();
		Init->x = x;
		Init->y = y;
		Init->id = id;
		Init->textContent = text;
		int Counter = 0;
		for (const auto& i : text) {
			Init->text.push_back(sf::Sprite(ImageManager::GetTexture("Font_" + std::to_string(i))));
			Init->text[Counter].setPosition({ x + FontSizeX * Counter + Counter, y });
			++Counter;
		}
		TextList.push_back(Init);
		TextMarginList.push_back(margin);
	}
	else std::cout << "Cannot add text ID " << id << " (Duplicated ID)" << "\n";
}
void EditText(const std::string &NewText, const std::string &id) {
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
		int Counter = 0;
		for (const auto& i : NewText) {
			Init.push_back(sf::Sprite(ImageManager::GetTexture("Font_" + std::to_string(i))));
			Init[Counter].setPosition({ TextList[IndexCounter]->x + FontSizeX * Counter, TextList[IndexCounter]->y });
			++Counter;
		}
		TextList[IndexCounter]->text = Init;
		TextList[IndexCounter]->textContent = NewText;
	}
	else std::cout << "Cannot edit text ID " << id << " (Not existed ID)" << "\n";
}
inline void EditPosition(const float NewX, const float NewY, const std::string &id) {
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
	for (int i = 0; i < TextList.size(); ++i) {
		const int iTextSize = static_cast<int>(TextList[i]->text.size());
		for (int j = 0; j < iTextSize; ++j) {
			if (TextMarginList[i] == LEFT_MARGIN) TextList[i]->text[j].setPosition({ TextList[i]->x + j + FontSizeX * j + view.getCenter().x - (Width / 2.0f), TextList[i]->y + view.getCenter().y - (Height / 2.0f) });
			else if (TextMarginList[i] == RIGHT_MARGIN) TextList[i]->text[j].setPosition({ TextList[i]->x + view.getCenter().x - (Width / 2.0f) - (TextList[i]->text.size() - 1 - j) * FontSizeX - (iTextSize - 1 - j), TextList[i]->y + view.getCenter().y - (Height / 2.0f) });
		}
	}
}
inline void UpdateText() {
	for (const auto& i : TextList) {
		for (const auto& j : i->text) {
			window.draw(j);
		}
	}
}
inline int getSizeText(const std::string &id) {
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