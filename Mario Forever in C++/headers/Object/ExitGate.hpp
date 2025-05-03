#pragma once
#ifndef EXITGATE_HPP
#define EXITGATE_HPP

extern bool LevelCompleteEffect;
extern bool ExitGateForeActive;
extern sf::Clock ExitGateClock;
extern sf::Sprite ExitGateBack;
extern sf::Sprite ExitGateIndicator;
extern sf::Sprite ExitGateFore;
extern void ExitGateInit();
extern void ExitGateDraw();
extern void ExitGateStatusUpdate(float deltaTime);
extern void ExitGateEffectDraw();
extern void ExitGateEffectReset();
#endif