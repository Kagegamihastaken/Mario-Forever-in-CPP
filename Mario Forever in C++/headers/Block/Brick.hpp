#pragma once

#ifndef BRICK_HPP
#define BRICK_HPP

#include <vector>
#include "Obstacles.hpp"
#include "../Core/Loading/enum.hpp"

extern std::vector<Obstacles> Bricks;
extern std::vector<BrickID> BrickIDList;
extern TextureManager BrickTextureManager;
extern std::vector<std::pair<float, float>> BrickSaveList;
extern void AddBrick(BrickID ID, BrickAtt att, float x, float y);
extern inline void BrickUpdate();
extern void HitEvent(float x, float y);
extern void DeleteBrick(float x, float y);
extern void LoadBricks();
extern inline void BrickStatusUpdate();
extern void DeleteAllBrick();

#endif // BRICK_HPP