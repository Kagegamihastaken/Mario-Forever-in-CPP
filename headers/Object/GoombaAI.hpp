#pragma once

#ifndef GOOMBAAI_HPP
#define GOOMBAAI_HPP

#include "Core/Loading/enum.hpp"
#include "Core/Class/GoombaAIClass.hpp"
#include "Core/ExternalHeaders/plf_colony.h"

extern plf::colony<MFCPP::GoombaAI> GoombaAIList;
extern void GoombaAILoadRes();
extern void SetPrevGoombaAIPos();
extern void InterpolateGoombaAIPos(float alpha);
extern void DeleteGoombaAI(GoombaAIType type, float x, float y);
extern void AddGoombaAI(GoombaAIType type, int SkinID, float x, float y, GoombaAIDirection Dir);
extern void GoombaStatusUpdate(float deltaTime);
extern void GoombaAIVertYUpdate(float deltaTime);
extern void GoombaAIVertXUpdate(float deltaTime);
extern void GoombaAICheckCollide();
extern void GoombaAIDraw();
extern void DeleteAllGoombaAI();
extern void GoombaAICollisionUpdate();
extern void GoombaAICleanup();
extern void DeleteGoombaAIIndex(const plf::colony<MFCPP::GoombaAI>::colony_iterator<false>& it);

#endif // GOOMBAAI_HPP