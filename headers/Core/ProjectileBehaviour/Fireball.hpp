#pragma once
#ifndef MFCPP_FIREBALL_HPP
#define MFCPP_FIREBALL_HPP

struct FireballData {
    float Xvelo, Yvelo, X, Y;
    bool remove = false;
};

extern FireballData FireballX(const sf::Vector2f& pos, float Xvelo, float Yvelo, bool direction, float deltaTime, const sf::FloatRect& hitbox, const sf::Vector2f& origin);
extern FireballData FireballY(const sf::Vector2f& pos, float Xvelo, float Yvelo, float deltaTime, const sf::FloatRect& hitbox, const sf::Vector2f& origin);

#endif //MFCPP_FIREBALL_HPP