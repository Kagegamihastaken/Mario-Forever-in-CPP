#ifndef MARIO_EFFECT_HPP
#define MARIO_EFFECT_HPP
extern void SetPrevMarioEffectPos();
extern void InterpolateMarioEffectPos(float alpha);
extern void MarioEffectInit();
extern void MarioEffectDraw();
extern void ActiveMarioEffect();
extern void MarioEffectStatusUpdate(float deltaTime);

extern bool EffectActive;

#endif