#include "Object/Scenery/TankTileBack.hpp"

#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"

TankTileBack::TankTileBack(SceneryManager &manager, const sf::Vector2f &position)
    : Scenery(manager),
    m_transform(position, sf::Vector2f(0.f, 0.f), sf::degrees(0.f)){
    m_animation.setTexture("TankTileBack");
    m_hitbox = sf::FloatRect({0.f, 0.f}, {64.f, 32.f});
}

void TankTileBack::updatePreviousData() {
    m_transform.Update();
}

void TankTileBack::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) return;
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
}

void TankTileBack::animationUpdate(float deltaTime) {}

sf::Vector2f TankTileBack::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f TankTileBack::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect TankTileBack::getHitbox() {
    return m_hitbox;
}

bool TankTileBack::isDestroyed() {
    return m_transform.isDestroyed();
}
