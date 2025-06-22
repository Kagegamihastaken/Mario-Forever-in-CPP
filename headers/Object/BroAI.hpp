#ifndef BROAI_HPP
#define BROAI_HPP

#include "Core/Loading/enum.hpp"

extern void BroAILoadRes();
extern void BroAIDraw();
extern void AddBroAI(BroAIType type, BroAIMovementType movementType, float x, float y);
extern void SetPrevBroAIPos();
extern void InterpolateBroAIPos(float alpha);
extern void BroAIVertXUpdate(float deltaTime);
extern void BroAIVertYUpdate(float deltaTime);
extern void DeleteBroAI(float x, float y);
extern void DeleteAllBroAI();
extern void BroAIShootUpdate(float deltaTime);
extern void BroAIStatusUpdate();

#endif //BROAI_HPP
