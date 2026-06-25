#ifndef MARIO_EFFECT_HPP
#define MARIO_EFFECT_HPP

extern void SetPrevMarioEffectPos();
extern void MarioEffectInit();
extern void MarioEffectDraw(float alpha);
extern void ActiveMarioEffect(float alpha);
extern void MarioEffectStatusUpdate(float deltaTime);
extern void MoveMarioEffect(const sf::Vector2f& pos);

extern bool EffectActive;

#endif