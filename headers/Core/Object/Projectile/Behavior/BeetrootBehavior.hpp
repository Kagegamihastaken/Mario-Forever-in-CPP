#pragma once
#ifndef MFCPP_BEETROOTBEHAVIOR_HPP
#define MFCPP_BEETROOTBEHAVIOR_HPP

class BeetrootBehavior {
public:
    struct BeetrootData {
        sf::Vector2f position, velocity;
        int count;
    };
    static BeetrootData BeetrootXUpdate(const sf::Vector2f& pos, int hit_count, const sf::Vector2f& velocity, bool direction, float deltaTime);
    static BeetrootData BeetrootYUpdate(const sf::Vector2f& pos, int hit_count, const sf::Vector2f& velocity, float deltaTime);
    static BeetrootData BeetrootXCollision(const sf::Vector2f& pos, int hit_count, int& do_stuff, const sf::Vector2f& velocity, bool direction, float deltaTime, const sf::FloatRect& hitbox, const sf::Vector2f& origin);
    static BeetrootData BeetrootYCollision(const sf::Vector2f& pos, int hit_count, int& do_stuff, const sf::Vector2f& velocity, float deltaTime, const sf::FloatRect& hitbox, const sf::Vector2f& origin);
};

#endif //MFCPP_BEETROOTBEHAVIOR_HPP