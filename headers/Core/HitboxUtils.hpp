#ifndef MFCPP_HITBOXUTILS_HPP
#define MFCPP_HITBOXUTILS_HPP

#include <queue>
#include "SFML/Graphics/Rect.hpp"

class HitboxUtils final {
public:
    struct HitboxDetail {
        sf::FloatRect hitbox;
        sf::Vector2f position;
        sf::Color color;
    };
    static void drawHitbox();
    static void addHitboxDebug(const HitboxDetail& obj);
private:
    static std::queue<HitboxDetail> m_hitboxDebug;
};

#endif //MFCPP_HITBOXUTILS_HPP