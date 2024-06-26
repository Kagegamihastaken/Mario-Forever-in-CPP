#ifndef TEXT_HPP
#define TEXT_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include "enum.hpp"

class Text {
public:
	float x = 0, y = 0;
	std::string textContent = "";
	std::vector<sf::Sprite> text;
	std::string id;
};

extern void AddText(std::string id, std::string text, TextMarginID margin, float x, float y);
extern inline void EditText(std::string NewText, std::string id);
extern inline void EditPosition(float NewX, float NewY, std::string id);
extern inline void UpdateText();
extern inline void UpdatePositionCharacter();
extern inline int getSizeText(std::string id);

#endif // !TEXT_HPP