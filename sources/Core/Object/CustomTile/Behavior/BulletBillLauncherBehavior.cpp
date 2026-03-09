#include "Core/Object/CustomTile/Behavior/BulletBillLauncherBehavior.hpp"

#include "Core/WindowFrame.hpp"
#include "Effect/MarioEffect.hpp"
#include "Object/Mario.hpp"

BulletBillLauncherBehavior::BulletBillLauncherData BulletBillLauncherBehavior::BulletBillLauncherUpdate(const BulletBillLauncherData& data, bool& output, float deltaTime) {
    BulletBillLauncherData newData = data;
    output = false;
    if (!EffectActive) {
        if (f_abs(newData.position.x - player.curr.x) > 80.f) newData.disabled = false;
        else newData.disabled = true;
    }
    else newData.disabled = false;

    if (!newData.disabled)
        newData.timing -= deltaTime;

    //state
    if (newData.state) {
        if (newData.timing <= 0.f) {
            output = true;
            newData.timing = newData.launch_interval + RandomIntNumberGenerator(0, newData.random_fire_interval);
        }
    }
    else {
        if (newData.timing <= -newData.first_time_shot) {
            output = true;
            newData.timing = newData.launch_interval + RandomIntNumberGenerator(0, newData.random_fire_interval);
            newData.state = true;
        }
    }
    return newData;
}
