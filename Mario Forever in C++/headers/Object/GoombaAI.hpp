#pragma once

#ifndef GOOMBAAI_HPP
#define GOOMBAAI_HPP

#include <SFML/Graphics.hpp>

#include "../Core/Loading/enum.hpp"
#include "Mario.hpp"

extern std::vector<MovableObject> GoombaAIList;
extern std::vector<GoombaAIType> GoombaAITypeList;
extern std::vector<std::pair<float, float>> GoombaAIHitboxList;
extern std::vector<int> GoombaAISkinIDList;
extern void GoombaAILoadRes();
extern void DeleteGoombaAI(GoombaAIType type, float x, float y);
extern void AddGoombaAI(GoombaAIType type, int SkinID, float x, float y, GoombaAIDirection Dir);
extern void GoombaStatusUpdate();
extern void GoombaAIVertYUpdate();
extern void GoombaAIVertXUpdate();
extern void GoombaAICheckCollide();
extern void GoombaAIUpdate();
extern void DeleteAllGoombaAI();
extern void GoombaAICollisionUpdate();

#endif // GOOMBAAI_HPP