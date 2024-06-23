#pragma once

#ifndef BRICK_HPP
#define BRICK_HPP

#include <vector>
#include "../headers/Obstacles.hpp"
#include "../headers/enum.hpp"

extern std::vector<Obstacles> Bricks;
extern std::vector<BrickID> BrickIDList;
extern sf::Texture BrickTexture;
extern void AddBrick(BrickID ID, float x, float y);
extern void BrickUpdate();
void HitEvent(float x, float y);

#endif // BRICK_HPP