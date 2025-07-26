#ifndef BROAIPROJECTILE_HPP
#define BROAIPROJECTILE_HPP

extern void SetPrevBroAIProjectilePos();
extern void InterpolateBroAIProjectilePos(float alpha);
extern void DeleteBroAIProjectile(float x, float y);
extern void DeleteAllBroAIProjectile();
extern void BroAIProjectileStatusUpdate();
extern void AddBroAIProjectile(bool direction, BroAIProjectileType type, float x, float y);
extern void BroAIProjectileDraw();
extern void BroAIProjectileMovementUpdate(float deltaTime);
extern void BroAIProjectileSpin(float deltaTime);
extern void BroAIProjectileCollision();

#endif //BROAIPROJECTILE_HPP
