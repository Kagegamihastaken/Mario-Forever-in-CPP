#pragma once

#ifndef TEXT_HPP
#define TEXT_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include "Core/Loading/enum.hpp"

extern void loadFontRes();
extern void AddText(const std::string &id, const std::string &text, TextMarginID margin, float x, float y);
extern void EditText(const std::string &NewText, const std::string &id);
extern void EditPosition(float NewX, float NewY, const std::string &id);
extern void UpdateText();
extern void UpdatePositionCharacter();
extern int getSizeText(const std::string &id);

#endif // !TEXT_HPP