#pragma once

#ifndef BRICK_HPP
#define BRICK_HPP

#include "Core/Loading/enum.hpp"

extern void AddBrick(BrickID ID, BrickAtt att, float x, float y);
extern void BricksInit();
extern void BricksInitTemp();
extern void ForceLoadBricksTexture(BrickID ID);

#endif // BRICK_HPP