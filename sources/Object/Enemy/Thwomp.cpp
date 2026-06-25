#include "Object/Enemy/Thwomp.hpp"

Thwomp::Thwomp(EnemyManager &manager, const sf::Vector2f &position)
    : Enemy(manager),
    m_transform(position, sf::Vector2f(0.f, 0.f), sf::degrees(0.f)){ //later
}

void Thwomp::updatePreviousData() {

}

void Thwomp::EnemyCollision() {

}

void Thwomp::MarioCollision(float MarioYVelocity) {

}

void Thwomp::statusUpdate(float deltaTime) {

}

void Thwomp::XUpdate(float deltaTime) {

}

void Thwomp::YUpdate(float deltaTime) {

}

void Thwomp::BlockHit() {

}

void Thwomp::ShellHit() {

}

void Thwomp::Destroy() {

}

void Thwomp::draw(float alpha) {

}

void Thwomp::Death(unsigned int state) {

}

bool Thwomp::isDeath() {
    return false;
}

void Thwomp::animationUpdate(float deltaTime) {

}

sf::Vector2f Thwomp::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f Thwomp::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect Thwomp::getHitbox() {
    return m_hitbox;
}

bool Thwomp::isDestroyed() {
    return m_transform.isDestroyed();
}
