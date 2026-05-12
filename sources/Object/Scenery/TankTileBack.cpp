#include "Object/Scenery/TankTileBack.hpp"

#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"

TankTileBack::TankTileBack(SceneryManager &manager, const sf::Vector2f &position) : Scenery(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_animation.setTexture("TankTileBack");
    setHitbox(sf::FloatRect({0.f, 0.f}, {64.f, 32.f}));
    setOrigin(sf::Vector2f(0.f, 0.f));
}

void TankTileBack::setPreviousData() {
    setPreviousPosition(getCurrentPosition());
}

void TankTileBack::interpolateData(float alpha) {
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}

void TankTileBack::draw() {
    if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 32.f)) return;
    m_animation.animationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.animationDraw();
}
