#ifndef MFCPP_UTILITY_HPP
#define MFCPP_UTILITY_HPP
#include "Class/CollisionObjectClass.hpp"

namespace Utility {
    int32_t RandomIntNumberGenerator(int32_t a, int32_t b);
    float RandomFloatNumberGenerator(float a, float b);
    float f_mod(float a, float b);
    float f_abs(float a);
    float f_round(float val);
    float VectorGetLength(const sf::Vector2f& val);
    sf::Vector2f VectorNormalized(const sf::Vector2f& val);
    int32_t hexToInt(const std::string &hex);

    float speedNerf(const MFCPP::CollisionObject& collision, const sf::Vector2f& velocity, float offset, bool direction);
};

#endif //MFCPP_UTILITY_HPP
