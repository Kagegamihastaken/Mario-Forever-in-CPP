#ifndef PIRANHAAI_HPP
#define PIRANHAAI_HPP

#include "Core/Class/PiranhaAIClass.hpp"
#include "Core/Loading/enum.hpp"

extern std::vector<MFCPP::PiranhaAI> PiranhaAIList;
extern void SetPrevPiranhaAIPos();
extern void InterpolatePiranhaAIPos(float alpha);
extern void AddPiranha(PiranhaID ID, PiranhaDirection dir, float x, float y);
extern void PiranhaAIInit();
extern void PiranhaAIStatusUpdate();
extern void PiranhaAIDraw();
extern void PiranhaAIMovementUpdate(float deltaTime);
extern void DeletePiranhaAIIndex(int i);
extern void ClearPiranhaAI();
extern void PiranhaAICleanup();
extern void PiranhaAIShoot(float deltaTime);

#endif