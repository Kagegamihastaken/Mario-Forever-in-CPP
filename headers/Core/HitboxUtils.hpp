#ifndef MFCPP_HITBOXUTILS_HPP
#define MFCPP_HITBOXUTILS_HPP

#include "SFML/Graphics/Rect.hpp"

namespace HitboxUtils {
    struct HitboxDetail {
        sf::FloatRect hitbox;
        sf::Vector2f position;
        sf::Color color;
    };
    void drawHitbox();
    void addHitboxDebug(const HitboxDetail& obj);
};

#endif //MFCPP_HITBOXUTILS_HPP