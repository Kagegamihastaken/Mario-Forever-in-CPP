#include "Object/Enemy/Thwomp.hpp"

Thwomp::Thwomp(EnemyManager &manager, const sf::Vector2f &position) : Enemy(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
}

void Thwomp::setPreviousData() {

}

void Thwomp::interpolateData(float alpha) {

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

void Thwomp::draw() {

}

void Thwomp::Death(unsigned int state) {

}

bool Thwomp::isDeath() {

}
