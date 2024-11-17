#ifndef PIRANHAAI_HPP
#define PIRANHAAI_HPP

#include "../Core/Loading/enum.hpp"

#include <vector>

#include <SFML/Graphics.hpp>

extern std::vector<sf::Sprite> PiranhaAIList;
extern std::vector<PiranhaID> PiranhaAITypeList;
extern std::vector<sf::FloatRect> PiranhaAIHitboxList;

extern void AddPiranha(PiranhaID ID, float x, float y);
extern void PiranhaAIInit();
extern void PiranhaAIStatusUpdate();
extern void PiranhaAIUpdate();
extern void PiranhaAIMovementUpdate();
extern void DeletePiranhaAI(float x, float y);
extern void ClearPiranhaAI();

#endif