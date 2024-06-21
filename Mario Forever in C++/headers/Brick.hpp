#ifndef BRICK_HPP
#define BRICK_HPP

#include <vector>
#include "../headers/Obstacles.hpp"

extern std::vector<Obstacles> Bricks;
extern sf::Texture BrickTexture;
extern void AddBrick(float x, float y);
extern void BrickUpdate();
void HitEvent(float x, float y);

#endif // BRICK_HPP