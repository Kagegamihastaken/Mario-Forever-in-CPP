#ifndef GOOMBAAI_HPP
#define GOOMBAAI_HPP

#include <SFML/Graphics.hpp>

#include "enum.hpp"
#include "Mario.hpp"

class GoombaAI : public Mario {};

extern std::vector<GoombaAI> GoombaAIList;
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