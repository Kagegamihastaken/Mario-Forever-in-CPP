#pragma once

#ifndef GOOMBAAIEFFECT_HPP
#define GOOMBAAIEFFECT_HPP

#include "enum.hpp"
extern void DeleteGoombaAIEffect(float x, float y);
extern void AddGoombaAIEffect(GoombaAIType type, GoombaAIEffectID id, float x, float y);
extern void DeleteAllGoombaAIEffect();
extern void GoombaAIEffectUpdate();
extern void GoombaAIEffectVertYUpdate();
extern void GoombaAIEffectStatusUpdate();

#endif // !GOOMBAAIEFFECT_HPP