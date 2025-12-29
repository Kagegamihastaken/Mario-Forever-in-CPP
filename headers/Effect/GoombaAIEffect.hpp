#pragma once

#ifndef GOOMBAAIEFFECT_HPP
#define GOOMBAAIEFFECT_HPP

#include "Core/Loading/enum.hpp"

const std::vector<std::string> GoombaDeathEffectFirst = {"DEAD_Goomba_1"};
const std::vector<std::string> GoombaDeathEffectSecond = {"DEAD_Goomba_2"};
const std::vector<std::string> GreenKoopaDeathEffect = {"DEAD_GreenKoopa"};
const std::vector<std::string> RedKoopaDeathEffect = {"DEAD_RedKoopa"};

extern void GoombaAIEffectInit();
extern void DeleteGoombaAIEffect(float x, float y);
extern void AddGoombaAIEffect(GoombaAIType type, GoombaAIEffectID id, int SkinID, float x, float y);
extern void SetPrevGoombaAIEffectPos();
extern void InterpolateGoombaAIEffectPos(float alpha);
extern void DeleteAllGoombaAIEffect();
extern void GoombaAIEffectDraw();
extern void GoombaAIEffectVertYUpdate(float deltaTime);
extern void GoombaAIEffectStatusUpdate(float deltaTime);
extern void GoombaAIEffectCleanup();

#endif // !GOOMBAAIEFFECT_HPP