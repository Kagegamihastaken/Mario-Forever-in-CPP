#pragma once

#ifndef TEXT_HPP
#define TEXT_HPP

#include "Core/Enumeration/enum.hpp"

extern void loadFontRes();
extern void AddText(const std::string &id, const std::string &text, TextMarginID margin, float x, float y, int32_t order, bool visible = true);
extern void SetTextVisible(const std::string &id, bool visible);
extern void EditText(const std::string &NewText, const std::string &id);
extern void EditPosition(float NewX, float NewY, const std::string &id);
extern void TextDraw(int32_t order);
extern void UpdatePositionCharacter();
extern int getSizeText(const std::string &id);
extern void DeleteAllText();

#endif // !TEXT_HPP