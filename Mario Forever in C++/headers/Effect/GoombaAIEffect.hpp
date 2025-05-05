#pragma once

#ifndef GOOMBAAIEFFECT_HPP
#define GOOMBAAIEFFECT_HPP

#include "../Core/Loading/enum.hpp"
extern void GoombaAIEffectInit();
extern void DeleteGoombaAIEffect(float x, float y);
extern void AddGoombaAIEffect(GoombaAIType type, GoombaAIEffectID id, int SkinID, float x, float y);
extern void SetPrevGoombaAIEffectPos();
extern void InterpolateGoombaAIEffectPos(float alpha);
extern void DeleteAllGoombaAIEffect();
extern void GoombaAIEffectUpdate();
extern void GoombaAIEffectVertYUpdate(float deltaTime);
extern void GoombaAIEffectStatusUpdate(float deltaTime);

#endif // !GOOMBAAIEFFECT_HPP