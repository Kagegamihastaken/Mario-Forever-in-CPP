#pragma once

#ifndef GOOMBAAI_HPP
#define GOOMBAAI_HPP

#include "Core/Loading/enum.hpp"
#include "Class/GoombaAIClass.hpp"

extern std::vector<MFCPP::GoombaAI> GoombaAIList;
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

#endif // GOOMBAAI_HPP