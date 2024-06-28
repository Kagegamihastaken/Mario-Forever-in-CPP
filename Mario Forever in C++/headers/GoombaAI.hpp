#ifndef GOOMBAAI_HPP
#define GOOMBAAI_HPP

#include <SFML/Graphics.hpp>

#include "enum.hpp"
#include "Mario.hpp"

class GoombaAI : public Mario {};

extern void AddGoombaAI(GoombaAIType type, float x, float y);
extern void GoombaAIVertYUpdate();
extern void GoombaAIVertXUpdate();
extern void GoombaAICheckCollide();
extern void GoombaAIUpdate();

#endif // GOOMBAAI_HPP