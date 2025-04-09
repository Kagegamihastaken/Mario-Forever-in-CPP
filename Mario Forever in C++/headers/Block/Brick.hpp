#pragma once

#ifndef BRICK_HPP
#define BRICK_HPP

#include <vector>
#include "Obstacles.hpp"
#include "../Core/Loading/enum.hpp"

extern std::vector<Obstacles> Bricks;
extern std::vector<std::pair<sf::FloatRect, sf::Vector2f>> BricksVertPosList;
extern std::vector<BrickID> BrickIDList;
extern TextureManager BrickTextureManager;
extern std::vector<std::pair<float, float>> BrickSaveList;
extern void AddBrick(BrickID ID, BrickAtt att, float x, float y);
extern void BricksSort();
extern inline void BrickUpdate();
extern void HitEvent(float x, float y);
extern void DeleteBrick(float x, float y);
extern void LoadBricks();
extern inline void BrickStatusUpdate();
extern inline BrickID GetIDBrick(float x, float y);
extern inline BrickAtt GetBrickAtt(float x, float y);
extern int getBrickIndex(float x, float y);
extern void MultiBrickCoin(float x, float y, int i);
extern void DeleteAllBrick();

#endif // BRICK_HPP