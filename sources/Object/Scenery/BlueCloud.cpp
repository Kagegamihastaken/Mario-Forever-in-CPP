#include "Object/Scenery/BlueCloud.hpp"

#include "Core/Scroll.hpp"

BlueCloud::BlueCloud(SceneryManager &manager, const sf::Vector2f &position)
    : Scenery(manager),
    m_transform(position, sf::Vector2f(0.f, 0.f), sf::degrees(0.f)){
    m_animation.setAnimationSequence("BlueCloudAnimName");
    m_animation.setAnimation(0, 2, 8, true);
    m_hitbox = sf::FloatRect({0.f, 0.f}, {64.f, 64.f});
}

void BlueCloud::updatePreviousData() {
    m_transform.Update();
}

void BlueCloud::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) {
        m_animation.frameUpdate();
        return;
    }
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
}

void BlueCloud::animationUpdate(float deltaTime) {
    m_animation.frameTimeAccumulate(deltaTime);
}

sf::Vector2f BlueCloud::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f BlueCloud::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect BlueCloud::getHitbox() {
    return m_hitbox;
}

bool BlueCloud::isDestroyed() {
    return m_transform.isDestroyed();
}
