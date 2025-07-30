#pragma once

#ifndef BRICK_HPP
#define BRICK_HPP

#include "Core/Class/BrickClass.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/ExternalHeaders/plf_colony.h"

extern plf::colony<MFCPP::Brick> Bricks;
extern void AddBrick(BrickID ID, BrickAtt att, float x, float y);
extern void SetPrevBricksPos();
extern void InterpolateBricksPos(float alpha);
extern void BrickUpdate(float deltaTime);
extern void BrickDraw();
extern void HitEvent(float x, float y);
extern void DeleteBrick(float x, float y);
extern void BricksInit();
extern void BricksInitTemp();
extern void ForceLoadBricksTexture(BrickID ID);
extern void BrickBreak(float x, float y);
extern void MultiBrickCoin(float x, float y, const plf::colony<MFCPP::Brick>::colony_iterator<false>& it);
extern void DeleteAllBrick();
extern void DeleteBrickIndex(const plf::colony<MFCPP::Brick>::colony_iterator<false>& it);
extern void BrickCleanup();

#endif // BRICK_HPP