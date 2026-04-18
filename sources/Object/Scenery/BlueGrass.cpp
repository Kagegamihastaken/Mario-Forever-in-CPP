#include "Object/Scenery/BlueGrass.hpp"

#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Object/SceneryHelper.hpp"

BlueGrass::BlueGrass(SceneryManager &manager, const sf::Vector2f &position) : Scenery(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_animation.setAnimationSequence(BlueGrassAnimName);
    m_animation.setAnimation(0, 2, 14, true);
    setHitbox(sf::FloatRect({0.f, 0.f}, {64.f, 32.f}));
    setOrigin(sf::Vector2f(0.f, 0.f));
}

void BlueGrass::setPreviousData() {
    setPreviousPosition(getCurrentPosition());
}

void BlueGrass::interpolateData(float alpha) {
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}

void BlueGrass::draw() {
    if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 32.f)) return;
    m_animation.AnimationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.AnimationDraw();
}
