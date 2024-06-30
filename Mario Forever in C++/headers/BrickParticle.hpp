#pragma once

#ifndef BRICKPARTICLE_HPP
#define BRICKPARTICLE_HPP

extern std::vector<std::array<sf::Sprite, 4>> BrickParticleList;
extern void AddBrickParticle(BrickID id, float ori_x, float ori_y);
extern void DeleteSubBrickParticle(float x, float y);
extern void BrickParticleStatusUpdate();
extern void BrickParticleUpdate();

#endif // BRICKPARTICLE_HPP