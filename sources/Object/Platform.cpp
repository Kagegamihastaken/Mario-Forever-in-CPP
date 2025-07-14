#include "Object/Platform.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Interpolation.hpp"
#include "Core/WindowFrame.hpp"

plf::colony<MFCPP::Platform> PlatformList;

void PlatformInit() {
    ImageManager::AddTexture("RedPlatform", "data/resources/Platform/RedPlatform.png");
}
void SetPrevPlatformPos() {
    for (auto & i : PlatformList) {
        if (i.willBeDestroyed()) continue;

        i.setPreviousPosition(i.getCurrentPosition());
    }
}
void InterpolatePlatformPos(const float alpha) {
    for (auto & i : PlatformList) {
        if (i.willBeDestroyed()) continue;

        i.setInterpolatedPosition(linearInterpolation(i.getPreviousPosition(), i.getCurrentPosition(), alpha));
    }
}
void AddPlatform(const sf::Vector2f& start, const sf::Vector2f& end) {
    const auto it = PlatformList.emplace(start, end, 2.f, sf::FloatRect({0.f, 0.f}, {95.f, 8.f}));
    it->AddAnimationSequence("RedPlatform");
}
void PlatformPositionUpdate(const float deltaTime) {
    for (auto it = PlatformList.begin(); it != PlatformList.end(); ++it) {
        const sf::Vector2f targetPoint = (it->isMovingToEnd() ? it->getEndPoint() : it->getStartPoint());
        const sf::Vector2f direction = VectorNormalized(targetPoint - it->getCurrentPosition());

        const float distanceToTarget = VectorGetLength(targetPoint - it->getCurrentPosition());
        if (const float moveDistance = it->getSpeed() * deltaTime; moveDistance >= distanceToTarget) {
            it->setCurrentPosition(targetPoint);
            it->setMovingToEnd(!it->isMovingToEnd());
        }
        else {
            it->setCurrentPosition(it->getCurrentPosition() + direction * moveDistance);
        }
    }
}
void DrawPlatform() {
    for (auto &i : PlatformList) {
        i.AnimationUpdate(i.getInterpolatedPosition(), i.getOrigin());
        i.AnimationDraw(window);
    }
}