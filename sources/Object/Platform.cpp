#include "Object/Platform.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Logging.hpp"
#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"

plf::colony<MFCPP::Platform> PlatformList;
static bool PlatformDeleteGate = false;

void PlatformInit() {
    ImageManager::AddTexture("RedPlatform", "data/resources/Platform/RedPlatform.png");
    ImageManager::AddTexture("RedSmallPlatform", "data/resources/Platform/RedPlatform.png", sf::IntRect({0, 0}, {32, 32}));
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
void DeletePlatform(const plf::colony<MFCPP::Platform>::colony_iterator<false>& it) {
    it->willDestroy(true);
    PlatformDeleteGate = true;
}
void AddPlatform(const sf::Vector2f& start, const sf::Vector2f& end) {
    const auto it = PlatformList.emplace(start, end, 6.f, sf::FloatRect({0.f, 0.f}, {32.f, 8.f}), true, false, false);
    it->AddAnimationSequence("RedSmallPlatform");
}
void PlatformStatusUpdate() {
    for (auto it = PlatformList.begin(); it != PlatformList.end(); ++it) {
        if (isOutScreenYBottom(it->getCurrentPosition().y, 32.f) && it->isFall()) DeletePlatform(it);
    }
}
void PlatformPositionUpdate(const float deltaTime) {
    for (auto it = PlatformList.begin(); it != PlatformList.end(); ++it) {
        if (it->getStartPoint() == it->getEndPoint()) continue;
        //Wait
        if (it->isWait() && it->getWaitState() != 1) continue;


        // Updates the position of a platform based on its movement parameters.
        //
        // This function calculates the new position of a platform, handling acceleration, deceleration,
        // and smooth movement between its start and end points.
        //
        const sf::Vector2f targetPoint = (it->isMovingToEnd() ? it->getEndPoint() : it->getStartPoint());
        const sf::Vector2f direction = VectorNormalized(targetPoint - it->getCurrentPosition());

        if (const float distanceToTarget = VectorGetLength(targetPoint - it->getCurrentPosition()); it->getSmoothStore() >= distanceToTarget) {
            if (it->getCurrentSpeed() > 0.f) {
                const float maxValue = (it->isSmooth() ? std::max(it->getCurrentSpeed() - 0.1f * deltaTime, 0.f) : 0.f);
                it->setCurrentSpeed(maxValue);
                it->setCurrentPosition(it->getCurrentPosition() + direction * it->getCurrentSpeed() * deltaTime);
            }
            else {
                //Smooth
                it->setSmoothStore(0.f);
                it->setCurrentSpeed(0.f);
                //Wait
                if (!it->isFall())
                    it->setWaitSate(2);
                //General
                it->setCurrentPosition(targetPoint);
                it->setMovingToEnd(!it->isMovingToEnd());
            }
        }
        else {
            if (it->getCurrentSpeed() < it->getSpeed()) {
                const float minValue = (it->isSmooth() ? std::min(it->getCurrentSpeed() + 0.1f * deltaTime, it->getSpeed()) : it->getSpeed());
                it->setSmoothStore(it->getSmoothStore() + minValue * deltaTime);
                it->setCurrentSpeed(minValue);
            }
            else it->setCurrentSpeed(it->getSpeed());
            it->setCurrentPosition(it->getCurrentPosition() + direction * it->getCurrentSpeed() * deltaTime);
        }

        //Falling
        if (it->willFall() && it->isFall()) {
            it->setYVelo(it->getYVelo() + 0.15f * deltaTime);

            it->setStartPoint(it->getStartPoint() + sf::Vector2f(0.f, it->getYVelo()) * deltaTime);
            it->setEndPoint(it->getEndPoint() + sf::Vector2f(0.f, it->getYVelo()) * deltaTime);
            it->move(sf::Vector2f(0.f, it->getYVelo()) * deltaTime);
        }

    }
}
void DrawPlatform() {
    for (auto &i : PlatformList) {
        i.AnimationUpdate(i.getInterpolatedPosition(), i.getOrigin());
        i.AnimationDraw(window);
    }
}
void PlatformCleanup() {
    if (!PlatformDeleteGate) return;
    auto it = PlatformList.begin();
    while (it != PlatformList.end()) {
        if (!it->willBeDestroyed()) ++it;
        else it = PlatformList.erase(it);
    }
    PlatformDeleteGate = false;
}