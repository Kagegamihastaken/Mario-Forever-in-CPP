#ifndef BROAIEFFECT_HPP
#define BROAIEFFECT_HPP
#include "Core/Loading/enum.hpp"

const std::vector<std::string> HammerBroDeath = {"DEAD_HammerBro"};

extern void BroAIEffectInit();
extern void SetPrevBroAIEffectPos();
extern void InterpolateBroAIEffectPos(float alpha);
extern void AddBroAIEffect(BroAIType type, bool dir, float x, float y);
extern void DeleteBroAIEffect(float x, float y);
extern void DeleteAllBroAIEffect();
extern void BroAIEffectStatusUpdate(float deltaTime);
extern void BroAIEffectDraw();
extern void BroAIEffectVertYUpdate(float deltaTime);
extern void BroAIEffectCleanup();

#endif //BROAIEFFECT_HPP
