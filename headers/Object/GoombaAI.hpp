#pragma once

#ifndef GOOMBAAI_HPP
#define GOOMBAAI_HPP

#include "Core/Loading/enum.hpp"

extern void GoombaAIInit();
extern void AddGoombaAI(GoombaAIType type, int SkinID, float x, float y, GoombaAIDirection Dir);

#endif // GOOMBAAI_HPP