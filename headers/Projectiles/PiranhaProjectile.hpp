#ifndef PIRANHAPROJECTILE_HPP
#define PIRANHAPROJECTILE_HPP

#include "Core/Loading/enum.hpp"
extern void SetPrevPiranhaAIProjectilePos();
extern void InterpolatePiranhaAIProjectilePos(float alpha);
extern void DeleteAllPiranhaAIProjectile();
extern void PiranhaAIProjectileStatusUpdate();
extern void AddPiranhaAIProjectile(bool direction, PiranhaDirection piranha_direction, PiranhaProjectileType type, float x, float y);
extern void PiranhaAIProjectileDraw();
extern void PiranhaAIProjectileMovementUpdate(float deltaTime);
extern void PiranhaAIProjectileSpin(float deltaTime);
extern void PiranhaAIProjectileCollision();
extern void PiranhaAIProjectileCleanup();

#endif //PIRANHAPROJECTILE_HPP
