#include "Core/AutoScroll.hpp"

#include "Core/Level.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Platform/TankGround.hpp"

sf::Vector2f MFCPP::AutoScroll::m_position = sf::Vector2f(0.f,0.f);
sf::Vector2f MFCPP::AutoScroll::m_origin_pos = sf::Vector2f(0.f, 0.f);
bool MFCPP::AutoScroll::m_autoScrollMode = true;
bool MFCPP::AutoScroll::m_tankMode = true;
float MFCPP::AutoScroll::m_speed = 1.f;
//Dont touch
bool MFCPP::AutoScroll::m_tankAnimationStop = false;

void MFCPP::AutoScroll::AutoScrollBuild() {
    if (!m_autoScrollMode) return;

    if (m_tankMode) {
        GameScene::movingBlockManager.addMovingBlock<TankGround>(sf::Vector2f(m_origin_pos.x - Width / 2.f, Height - 64.f));
    }
}

void MFCPP::AutoScroll::AutoScrollInit() {
    if (m_tankMode) m_origin_pos.x = Width / 2.f;
}

void MFCPP::AutoScroll::UpdateSpeed(float deltaTime) {
    if (!m_autoScrollMode) return;
    m_position += sf::Vector2f(m_speed * deltaTime, 0.f);
    if (m_position.x >= LevelWidth - Width / 2.f) {
        m_position.x = LevelWidth - Width / 2.f;
        if (!m_tankAnimationStop) m_tankAnimationStop = true;
    }
}

void MFCPP::AutoScroll::resetPosition() {
    m_position = m_origin_pos;
}

sf::Vector2f MFCPP::AutoScroll::getPosition() {
    return m_position;
}

void MFCPP::AutoScroll::setOriginPosition(const sf::Vector2f &pos) {
    m_origin_pos = pos;
}

void MFCPP::AutoScroll::setAutoScrollMode(bool val) {
    m_autoScrollMode = val;
}

bool MFCPP::AutoScroll::getAutoScrollMode() {
    return m_autoScrollMode;
}

float MFCPP::AutoScroll::getSpeed() {
    return m_speed;
}

bool MFCPP::AutoScroll::getTankMode() {
    return m_tankMode;
}

bool MFCPP::AutoScroll::getTankAnimationStop() {
    return m_tankAnimationStop;
}
