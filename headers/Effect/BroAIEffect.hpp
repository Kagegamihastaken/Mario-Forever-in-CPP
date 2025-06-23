#ifndef BROAIEFFECT_HPP
#define BROAIEFFECT_HPP
#include "Core/Loading/enum.hpp"

extern void BroAIEffectInit();
extern void SetPrevBroAIEffectPos();
extern void InterpolateBroAIEffectPos(float alpha);
extern void AddBroAIEffect(BroAIType type, bool dir, float x, float y);
extern void DeleteBroAIEffect(float x, float y);
extern void DeleteAllBroAIEffect();
extern void BroAIEffectStatusUpdate(float deltaTime);
extern void BroAIEffectUpdate();
extern void BroAIEffectVertYUpdate(float deltaTime);

#endif //BROAIEFFECT_HPP
