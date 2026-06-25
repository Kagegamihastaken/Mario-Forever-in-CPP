#include "Object/Scenery/BlueGrass.hpp"

#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Object/SceneryHelper.hpp"

BlueGrass::BlueGrass(SceneryManager &manager, const sf::Vector2f &position)
    : Scenery(manager),
    m_transform(position, sf::Vector2f(0.f, 0.f), sf::degrees(0.f)){
    m_animation.setAnimationSequence("BlueGrassAnimName");
    m_animation.setAnimation(0, 2, 14, true);
    m_hitbox = sf::FloatRect({0.f, 0.f}, {64.f, 32.f});
}

void BlueGrass::updatePreviousData() {
    m_transform.Update();
}

void BlueGrass::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) {
        m_animation.frameUpdate();
        return;
    }
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
}

void BlueGrass::animationUpdate(float deltaTime) {
    m_animation.frameTimeAccumulate(deltaTime);
}

sf::Vector2f BlueGrass::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f BlueGrass::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect BlueGrass::getHitbox() {
    return m_hitbox;
}

bool BlueGrass::isDestroyed() {
    return m_transform.isDestroyed();
}
