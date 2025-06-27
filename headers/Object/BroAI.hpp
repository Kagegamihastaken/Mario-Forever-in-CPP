#ifndef BROAI_HPP
#define BROAI_HPP

#include "Class/BroAIClass.hpp"
#include "Core/Loading/enum.hpp"

extern std::vector<MFCPP::BroAI> BroAIList;
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
extern void BroAICheckCollide();
extern void BroAICleanup();
extern void DeleteBroAIIndex(int i);

#endif //BROAI_HPP
