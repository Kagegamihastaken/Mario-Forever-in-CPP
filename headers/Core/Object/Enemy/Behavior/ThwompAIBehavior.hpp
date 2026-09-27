#ifndef MFCPP_THWOMPAIBEHAVIOR_HPP
#define MFCPP_THWOMPAIBEHAVIOR_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace ThwompAIBehavior {
    struct ThwompData {
        sf::Vector2f position{};
        sf::Vector2f velocity{};
        bool decending{}, hitGround{};
        float countingBeforeRise{}, rise{}, y_velocity_rise{}, yStore{};
    };
    ThwompData ThwompYStatusUpdate(const ThwompData& data, const sf::FloatRect& hitbox, const sf::Vector2f& origin, float deltaTime);
}

#endif //MFCPP_THWOMPAIBEHAVIOR_HPP
