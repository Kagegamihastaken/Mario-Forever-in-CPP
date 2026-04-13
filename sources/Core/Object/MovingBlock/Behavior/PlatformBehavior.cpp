#include "Core/Object/MovingBlock/Behavior/PlatformBehavior.hpp"
#include "Core/WindowFrame.hpp"

PlatformBehavior::PlatformData PlatformBehavior::PlatformStatusUpdate(const PlatformData &data, float deltaTime) {
    PlatformData newData = data;
    if (newData.start == newData.end) return newData;
    //Wait
    if (newData.wait && newData.waitState != 1) return newData;

    const sf::Vector2f targetPoint = (newData.movingToEnd ? newData.end : newData.start);
    const sf::Vector2f direction = VectorNormalized(targetPoint - newData.position);

    if (const float distanceToTarget = VectorGetLength(targetPoint - newData.position); newData.smoothStore >= distanceToTarget) {
        if (newData.speed > 0.f) {
            const float maxValue = (newData.smooth ? std::max(newData.speed - 0.1f * deltaTime, 0.f) : 0.f);
            newData.speed = maxValue;
            newData.position += direction * newData.speed * deltaTime;
        }
        else {
            //Smooth
            newData.smoothStore = 0.f;
            newData.speed = 0.f;
            //Wait
            if (!newData.fall)
                newData.waitState = 2;
            //General
            newData.position = targetPoint;
            newData.movingToEnd = !newData.movingToEnd;
        }
    }
    else {
        if (newData.speed < newData.maxSpeed) {
            const float minValue = (newData.smooth ? std::min(newData.speed + 0.1f * deltaTime, newData.maxSpeed) : newData.maxSpeed);
            newData.smoothStore = newData.smoothStore + minValue * deltaTime;
            newData.speed = minValue;
        }
        else newData.speed = newData.maxSpeed;
        newData.position += direction * newData.speed * deltaTime;
    }
    //Falling
    if (newData.willFall && newData.fall) {
        newData.y_velocity += 0.15f * deltaTime;

        newData.start += sf::Vector2f(0.f, newData.y_velocity) * deltaTime;
        newData.end += sf::Vector2f(0.f, newData.y_velocity) * deltaTime;
        newData.position += sf::Vector2f(0.f, newData.y_velocity) * deltaTime;
    }
    return newData;
}
