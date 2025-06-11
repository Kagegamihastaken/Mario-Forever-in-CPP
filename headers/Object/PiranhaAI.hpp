#ifndef PIRANHAAI_HPP
#define PIRANHAAI_HPP

#include "Core/Loading/enum.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

#include "Core/Animate/SingleAnimationObject.hpp"

extern void SetPrevPiranhaAIPos();
extern void InterpolatePiranhaAIPos(float alpha);
extern void AddPiranha(PiranhaID ID, float x, float y);
extern void PiranhaAIInit();
extern void PiranhaAIStatusUpdate();
extern void PiranhaAIUpdate();
extern void PiranhaAIMovementUpdate(float deltaTime);
extern void DeletePiranhaAI(float x, float y);
extern void ClearPiranhaAI();

#endif