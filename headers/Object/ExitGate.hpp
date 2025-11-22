#pragma once
#ifndef EXITGATE_HPP
#define EXITGATE_HPP
#include "Core/Class/ActiveObjectClass.hpp"

extern bool LevelCompleteEffect;
extern bool ExitGateForeActive;
extern float ExitGateClock;
extern MFCPP::ActiveObject<float> ExitGateBack;
extern MFCPP::ActiveObject<float> ExitGateFore;
extern MFCPP::ActiveObject<float> ExitGateIndicator;
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