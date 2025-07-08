#pragma once

#ifndef BRICKPARTICLE_HPP
#define BRICKPARTICLE_HPP

extern void SetPrevBrickParticlePos();
extern void InterpolateBrickParticlePos(float alpha);
extern void AddBrickParticle(BrickID id, float ori_x, float ori_y);
extern void BrickParticleStatusUpdate(float deltaTime);
extern void BrickParticleDraw();
extern void DeleteAllBrickParticle();
extern void BrickParticleInit();
extern void BrickParticleCleanup();

#endif // BRICKPARTICLE_HPP