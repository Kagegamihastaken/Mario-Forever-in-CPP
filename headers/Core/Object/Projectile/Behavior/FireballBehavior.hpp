#pragma once
#ifndef MFCPP_FIREBALL_HPP
#define MFCPP_FIREBALL_HPP

class FireballBehavior {
public:
    struct FireballData {
        sf::Vector2f position, velocity;
        bool remove{};
    };

    static FireballData FireballX(const sf::Vector2f& pos, const sf::Vector2f& velocity, bool direction, float deltaTime, const sf::FloatRect& hitbox, const sf::Vector2f& origin);
    static FireballData FireballY(const sf::Vector2f& pos, bool& do_stuff, const sf::Vector2f& velocity, float deltaTime, const sf::FloatRect& hitbox, const sf::Vector2f& origin);
};

#endif //MFCPP_FIREBALL_HPP