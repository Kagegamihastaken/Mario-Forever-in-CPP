#ifndef BULLETLAUNCHER_HPP
#define BULLETLAUNCHER_HPP
#include "Core/Loading/enum.hpp"

extern void BulletLauncherInit();
extern void AddBulletLauncher(BulletType type, float x, float y);
extern void BulletLauncherDraw();
extern void BulletLauncherClear();
extern void BulletLauncherStatusUpdate(float deltaTime);

#endif //BULLETLAUNCHER_HPP
