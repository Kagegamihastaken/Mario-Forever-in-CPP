#pragma once

#ifndef GOOMBAAI_HPP
#define GOOMBAAI_HPP

#include <SFML/Graphics.hpp>

#include "enum.hpp"
#include "Mario.hpp"

extern std::vector<MovableObject> GoombaAIList;
extern std::vector<GoombaAIType> GoombaAITypeList;
extern std::vector<std::pair<float, float>> GoombaAIHitboxList;
extern void DeleteGoombaAI(GoombaAIType type, float x, float y);
extern void AddGoombaAI(GoombaAIType type, float x, float y);
extern void GoombaStatusUpdate();
extern void GoombaAIVertYUpdate();
extern void GoombaAIVertXUpdate();
extern void GoombaAICheckCollide();
extern void GoombaAIUpdate();

#endif // GOOMBAAI_HPP