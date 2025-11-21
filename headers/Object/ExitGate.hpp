#pragma once
#ifndef EXITGATE_HPP
#define EXITGATE_HPP

extern bool LevelCompleteEffect;
extern bool ExitGateForeActive;
extern float ExitGateClock;
extern sf::Sprite ExitGateBack;
extern sf::Sprite ExitGateIndicator;
extern sf::Sprite ExitGateFore;
extern sf::Vector2f ExitGateForeCurr;
extern sf::Vector2f ExitGateForePrev;
extern void ExitGateClockUpdate(float deltaTime);
extern void SetPrevExitGatePos();
extern void InterpolateExitGatePos(float alpha);
extern void ExitGateInit();
extern void ExitGateDraw();
extern void ExitGateStatusUpdate(float deltaTime);
extern void ExitGateEffectDraw();
extern void ExitGateEffectReset();
extern void resetExitGateClock();
#endif