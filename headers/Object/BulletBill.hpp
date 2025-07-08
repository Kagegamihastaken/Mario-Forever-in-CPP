#ifndef BULLETBILL_HPP
#define BULLETBILL_HPP
#include "Core/Loading/enum.hpp"

extern void BulletBillInit();
extern void SetPrevBulletBillPos();
extern void InterpolateBulletBillPos(float alpha);
extern void AddBulletBill(BulletType type, float speed, bool direction, float x, float y);
extern void BulletBillPositionUpdate(float deltaTime);
extern void DrawBulletBill();
extern void DeleteAllBulletBill();
extern void BulletBillCleanup();
extern void BulletBillStatusUpdate();
extern void BulletBillCheckCollide();

#endif //BULLETBILL_HPP
