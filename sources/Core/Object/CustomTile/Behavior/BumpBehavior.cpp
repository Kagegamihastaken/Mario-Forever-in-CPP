#include "Core/Object/CustomTile/Behavior/BumpBehavior.hpp"

BumpBehavior::BumpData BumpBehavior::BumpCoinUpdate(const BumpData &data, float Ypos, float deltaTime) {
    BumpData newData = data;
    if (newData.state) {
        if (!newData.updown) {
            if (newData.state_count < 11.0f) {
                newData.pos.y -= (newData.state_count < 6.0f ? 3.0f : (newData.state_count < 10.0f ? 2.0f : 1.0f)) * deltaTime;
                newData.state_count += (newData.state_count < 6.0f ? 3.0f : (newData.state_count < 10.0f ? 2.0f : 1.0f)) * deltaTime;
            }
            else {
                newData.updown = true;
                newData.state_count = 11.f;
            }
        }
        else {
            if (newData.state_count > 0.0f) {
                newData.pos.y += (newData.state_count > 10.0f ? 1.0f : (newData.state_count > 6.0f ? 2.0f : 3.0f)) * deltaTime;
                newData.state_count -= (newData.state_count > 10.0f ? 1.0f : (newData.state_count > 6.0f ? 2.0f : 3.0f)) * deltaTime;
            }
            else {
                newData.pos.y = Ypos;
                newData.state = false;
                newData.updown = false;
                newData.state_count = 0.f;
            }
        }
    }
    return newData;
}
BumpBehavior::BumpData BumpBehavior::BumpItemUpdate(const BumpData &data, float Ypos, float deltaTime) {
    BumpData newData = data;
    if (newData.state) {
        if (!newData.updown) {
            if (newData.state_count < 4.0f) {
                newData.pos.y -= deltaTime;
                newData.state_count += deltaTime;
            }
            else {
                newData.state_count = 4.f;
                newData.updown = true;
            }
        }
        else {
            if (newData.state_count > 0.0f) {
                newData.pos.y += deltaTime;
                newData.state_count -= deltaTime;
            }
            else {
                newData.pos.y = Ypos;
                newData.state_count = 0.f;
                newData.updown = false;
                newData.state = false;
            }
        }
    }
    return newData;
}
