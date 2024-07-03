#pragma once

#ifndef BRICK_HPP
#define BRICK_HPP

#include <vector>
#include "../headers/Obstacles.hpp"
#include "../headers/enum.hpp"

extern std::vector<Obstacles> Bricks;
extern std::vector<BrickID> BrickIDList;
extern sf::Texture BrickTexture;
extern std::vector<std::pair<float, float>> BrickSaveList;
extern void AddBrick(BrickID ID, BrickAtt att, float x, float y);
extern inline void BrickUpdate();
extern void HitEvent(float x, float y);
extern void DeleteBrick(float x, float y);
extern inline void BrickStatusUpdate();
extern void DeleteAllBrick();

#endif // BRICK_HPP