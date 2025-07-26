#ifndef FIREBALL_HPP
#define FIREBALL_HPP
#include "Core/Loading/enum.hpp"

extern void SetPrevMarioProjectilePos();
extern void InterpolateMarioProjectilePos(float alpha);
extern void DeleteMarioProjectile(float x, float y);
extern void DeleteAllMarioProjectile();
extern void MarioProjectileCollision();
extern void MarioProjectileStatusUpdate();
extern void MarioProjectileSpin(float deltaTime);
extern void AddMarioProjectile(bool direction, MarioProjectileType type, float x, float y);
extern void MarioProjectileMovementUpdate(float deltaTime);
extern void MarioProjectileDraw();
extern void MarioProjectileCleanup();
extern int getAmountProjectile();
extern void LevelEndMarioProjectileCleanup();

#endif //FIREBALL_HPP
