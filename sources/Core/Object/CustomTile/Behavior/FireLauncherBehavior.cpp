#include "Core/Object/CustomTile/Behavior/FireLauncherBehavior.hpp"
#include "Core/WindowFrame.hpp"

FireLauncherBehavior::FireLauncherData FireLauncherBehavior::FireLauncherStatusUpdate(const FireLauncherData &data, bool& launch, bool& sound, float deltaTime) {
    FireLauncherData newData = data;
    launch = false;
    sound = false;
    if (newData.FireIntervalCounting > 0.f) {
        newData.FireIntervalCounting -= deltaTime;
    }
    else if (newData.FireIntervalCounting <= 0.f) {
        newData.FireIntervalCounting = newData.FireInterval;
        newData.ProjectileCounting = newData.ProjectileCount;
    }

    if (newData.ProjectileCounting > 0) {
        if (!newData.PlaySound) {
            sound = true;
            newData.PlaySound = true;
        }
        newData.FireBetweenIntervalCounting += deltaTime;
        if (newData.FireBetweenIntervalCounting >= newData.FireBetweenInterval) {
            newData.FireBetweenIntervalCounting = f_mod(newData.FireBetweenIntervalCounting, newData.FireBetweenInterval);
            launch = true;
            newData.ProjectileCounting -= 1;
        }
    }
    else {
        if (newData.PlaySound) newData.PlaySound = false;
    }
    return newData;
}
