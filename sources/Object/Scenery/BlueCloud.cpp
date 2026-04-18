#include "Object/Scenery/BlueCloud.hpp"

#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Object/SceneryHelper.hpp"

BlueCloud::BlueCloud(SceneryManager &manager, const sf::Vector2f &position) : Scenery(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_animation.setAnimationSequence(BlueCloudAnimName);
    m_animation.setAnimation(0, 2, 8, true);
    setHitbox(sf::FloatRect({0.f, 0.f}, {64.f, 64.f}));
    setOrigin(sf::Vector2f(0.f, 0.f));
}

void BlueCloud::setPreviousData() {
    setPreviousPosition(getCurrentPosition());
}

void BlueCloud::interpolateData(float alpha) {
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}

void BlueCloud::draw() {
    if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 32.f)) return;
    m_animation.AnimationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.AnimationDraw();
}
