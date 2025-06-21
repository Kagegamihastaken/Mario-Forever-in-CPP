#ifndef BROAI_HPP
#define BROAI_HPP

extern void BroAILoadRes();
extern void BroAIDraw();
extern void AddBroAI(float x, float y);
extern void SetPrevBroAIPos();
extern void InterpolateBroAIPos(float alpha);
extern void BroAIVertXUpdate(float deltaTime);
extern void BroAIVertYUpdate(float deltaTime);
extern void DeleteBroAI(float x, float y);
extern void DeleteAllBroAI();

#endif //BROAI_HPP
