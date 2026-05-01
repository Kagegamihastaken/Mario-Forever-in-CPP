#include "Core/AutoScroll.hpp"

sf::Vector2f MFCPP::AutoScroll::m_position = sf::Vector2f(0.f,0.f);
sf::Vector2f MFCPP::AutoScroll::m_origin_pos = sf::Vector2f(0.f, 0.f);
bool MFCPP::AutoScroll::autoScrollMode = true;
float MFCPP::AutoScroll::m_speed = 1.f;

void MFCPP::AutoScroll::UpdateSpeed(float deltaTime) {
    if (!autoScrollMode) return;
    m_position += sf::Vector2f(m_speed * deltaTime, 0.f);
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
    autoScrollMode = val;
}

bool MFCPP::AutoScroll::getAutoScrollMode() {
    return autoScrollMode;
}

float MFCPP::AutoScroll::getSpeed() {
    return m_speed;
}
