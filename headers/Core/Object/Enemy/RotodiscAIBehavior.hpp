#ifndef MFCPP_ROTODISCAIBEHAVIOR_HPP
#define MFCPP_ROTODISCAIBEHAVIOR_HPP

#include "SFML/System/Vector2.hpp"

class RotodiscAIBehavior {
public:
    static sf::Vector2f RotodiscPositionAdjust(const sf::Vector2f& position, unsigned int arrange_mode);
    static sf::Vector2f RotodiscRoundXUpdate(const sf::Vector2f& position, const sf::Vector2f& pos_origin, float angle, float radius);
    static sf::Vector2f RotodiscRoundYUpdate(const sf::Vector2f& position, const sf::Vector2f& pos_origin, float angle, float radius);
    static float RotodiscAngleUpdate(float angle, float speed, float deltaTime);
    static void RotodiscRadiusChange(float& current_radius, bool& state, float radius, float speed_change, float deltaTime);
};

#endif //MFCPP_ROTODISCAIBEHAVIOR_HPP