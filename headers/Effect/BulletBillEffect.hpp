#ifndef BULLETBILLEFFECT_HPP
#define BULLETBILLEFFECT_HPP
#include "Core/Loading/enum.hpp"

extern void SetPrevBulletBillEffectPos();
extern void InterpolateBulletBillEffectPos(float alpha);
extern void AddBulletBillEffect(BulletType type, bool dir, float Xvelo, float x, float y);
extern void DeleteBulletBillEffect(float x, float y);
extern void DeleteAllBulletBillEffect();
extern void BulletBillEffectStatusUpdate();
extern void BulletBillEffectDraw();
extern void BulletBillEffectPositionUpdate(float deltaTime);
extern void BulletBillEffectCleanup();

#endif //BULLETBILLEFFECT_HPP
