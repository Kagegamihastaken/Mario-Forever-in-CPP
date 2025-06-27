#ifndef PIRANHAAI_HPP
#define PIRANHAAI_HPP

#include "Class/PiranhaAIClass.hpp"
#include "Core/Loading/enum.hpp"

extern std::vector<MFCPP::PiranhaAI> PiranhaAIList;
extern void SetPrevPiranhaAIPos();
extern void InterpolatePiranhaAIPos(float alpha);
extern void AddPiranha(PiranhaID ID, float x, float y);
extern void PiranhaAIInit();
extern void PiranhaAIStatusUpdate();
extern void PiranhaAIDraw();
extern void PiranhaAIMovementUpdate(float deltaTime);
extern void DeletePiranhaAIIndex(int i);
extern void ClearPiranhaAI();
extern void PiranhaAICleanup();

#endif