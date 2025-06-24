#pragma once

#ifndef BRICKPARTICLE_HPP
#define BRICKPARTICLE_HPP

extern std::vector<std::array<sf::Sprite, 4>> BrickParticleList;
extern void SetPrevBrickParticlePos();
extern void InterpolateBrickParticlePos(float alpha);
extern void AddBrickParticle(BrickID id, float ori_x, float ori_y);
extern void DeleteSubBrickParticle(float x, float y);
extern void BrickParticleStatusUpdate(float deltaTime);
extern void BrickParticleDraw();
extern void DeleteAllBrickParticle();
extern void BrickParticleInit();

#endif // BRICKPARTICLE_HPP