#include "Core/HitboxUtils.hpp"
#include <config.h>

#include "Core/Logging.hpp"
#include "Core/Scroll.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "Core/WindowFrame.hpp"

std::queue<HitboxUtils::HitboxDetail> HitboxUtils::m_hitboxDebug;
void HitboxUtils::addHitboxDebug(const HitboxDetail &obj) {
#if HITBOX_DEBUG
    m_hitboxDebug.push(obj);
#endif
}
void HitboxUtils::drawHitbox() {
#if HITBOX_DEBUG
    sf::RectangleShape hitbox;
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineThickness(1.f);
    const sf::Vector2f outlineOffset(hitbox.getOutlineThickness(), hitbox.getOutlineThickness());
    while (!m_hitboxDebug.empty()) {
        if (isOutOfScreen(MFCPP::CollisionObject(m_hitboxDebug.front().position, sf::Vector2f(0.f, 0.f), m_hitboxDebug.front().hitbox), 32.f)) {
            m_hitboxDebug.pop();
            continue;
        }
        hitbox.setSize(m_hitboxDebug.front().hitbox.size - 2.f * outlineOffset);
        hitbox.setPosition(m_hitboxDebug.front().position + m_hitboxDebug.front().hitbox.position + outlineOffset);
        hitbox.setOutlineColor(m_hitboxDebug.front().color);
        m_hitboxDebug.pop();
        window.draw(hitbox);
    }
#endif
}
