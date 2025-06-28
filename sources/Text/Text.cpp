#include "Text/Font.hpp"
#include "Text/Text.hpp"
#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Class/TextClass.hpp"

MFCPP::TextList MFText;

void loadFontRes() {
	MFText.SetFontSize(645, 32, 15, 16);
	ImageManager::AddImage("FontImage", "data/resources/Font.png");
	for (const auto &i : FontString) {
		ImageManager::AddTexture("FontImage", sf::IntRect(MFText.GetFontTexture(i), { 15, 16 }), "Font_" + std::to_string(i));
	}
	//FontTextureList.Loadingtexture("data/resources/Font.png", "Font", 0, 0, 645, 32);
}
void AddText(const std::string &id, const std::string &text, const TextMarginID margin, const float x, float y) {
	//search for duplicates ID
	bool isDuplicate = false;
	for (const auto& i : MFText.List) {
		if (i.getID() == id) {
			isDuplicate = true;
		}
	}
	if (!isDuplicate) {
		// If not, proceed as usual
		MFText.List.emplace_back();
		MFText.List.back().setPosition(sf::Vector2f(x, y));
		MFText.List.back().setID(id);
		MFText.List.back().setText(text);
		MFText.List.back().setMargin(margin);
		int Counter = 0;
		for (const auto& i : text) {
			MFText.List.back().m_SpriteText.emplace_back(ImageManager::GetTexture("Font_" + std::to_string(i)));
			//TextList.back().m_SpriteText.push_back(sf::Sprite(ImageManager::GetTexture("Font_" + std::to_string(i))));
			MFText.List.back().m_SpriteText[Counter].setPosition({ x + MFText.GetFontSize().x * Counter + Counter, y });
			++Counter;
		}
	}
	else std::cout << "Cannot add text ID " << id << " (Duplicated ID)" << "\n";
}
void EditText(const std::string &NewText, const std::string &id) {
	bool isFounded = false;
	int IndexCounter = 0;
	for (const auto& i : MFText.List) {
		if (i.getID() == id) {
			isFounded = true;
			break;
		}
		++IndexCounter;
	}
	if (isFounded) {
		if (MFText.List[IndexCounter].getText() != NewText) {
			MFText.List[IndexCounter].m_SpriteText.clear();
			MFText.List[IndexCounter].setText(NewText);
			//std::vector<sf::Sprite> Init;
			int Counter = 0;
			for (const auto& i : NewText) {
				MFText.List[IndexCounter].m_SpriteText.emplace_back(ImageManager::GetTexture("Font_" + std::to_string(i)));
				MFText.List[IndexCounter].m_SpriteText[Counter].setPosition({ MFText.List[IndexCounter].getPosition().x + MFText.GetFontSize().x * Counter, MFText.List[IndexCounter].getPosition().y });
				++Counter;
			}
			//TextList[IndexCounter].text = Init;
		}
	}
	else std::cout << "Cannot edit text ID " << id << " (Not existed ID)" << "\n";
}
void EditPosition(const float NewX, const float NewY, const std::string &id) {
	bool isFounded = false;
	int IndexCounter = 0;
	for (const auto& i : MFText.List) {
		if (i.getID() == id) {
			isFounded = true;
			break;
		}
		++IndexCounter;
	}
	if (isFounded) {
		MFText.List[IndexCounter].setPosition(sf::Vector2f(NewX, NewY));
	}
	else std::cout << "Cannot edit position ID " << id << " (Not existed ID)" << "\n";
}
void UpdatePositionCharacter() {
	for (int i = 0; i < MFText.List.size(); ++i) {
		const int iTextSize = static_cast<int>(MFText.List[i].m_SpriteText.size());
		for (int j = 0; j < iTextSize; ++j) {
			if (MFText.List[i].getMargin() == LEFT_MARGIN) MFText.List[i].m_SpriteText[j].setPosition({ MFText.List[i].getPosition().x + static_cast<float>(j) + MFText.GetFontSize().x * j + view.getCenter().x - (Width / 2.0f), MFText.List[i].getPosition().y + view.getCenter().y - (Height / 2.0f) });
			else if (MFText.List[i].getMargin() == RIGHT_MARGIN) MFText.List[i].m_SpriteText[j].setPosition({ MFText.List[i].getPosition().x + view.getCenter().x - (Width / 2.0f) - (MFText.List[i].m_SpriteText.size() - 1 - j) * MFText.GetFontSize().x - (iTextSize - 1 - j), MFText.List[i].getPosition().y + view.getCenter().y - (Height / 2.0f) });
		}
	}
}
void TextDraw() {
	for (const auto& i : MFText.List) {
		for (const auto& j : i.m_SpriteText) {
			window.draw(j);
		}
	}
}
int getSizeText(const std::string &id) {
	bool isFounded = false;
	int IndexCounter = 0;
	for (const auto& i : MFText.List) {
		if (i.getID() == id) {
			isFounded = true;
			break;
		}
		++IndexCounter;
	}
	if (isFounded) {
		return static_cast<int>(MFText.List[IndexCounter].getText().size());
	}
	else {
		std::cout << "Cannot get text ID " << id << " (Not existed ID)" << "\n";
		return -1;
	}
}