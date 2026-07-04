#include "Core/Utility.hpp"

std::random_device seed;

int32_t Utility::RandomIntNumberGenerator(const int32_t a, const int32_t b) {
    std::uniform_int_distribution dis(a, b);
    return dis(seed);
}
float Utility::RandomFloatNumberGenerator(const float a, const float b) {
    std::uniform_real_distribution dis(a, b);
    return dis(seed);
}
float Utility::f_mod(const float a, const float b) { return a - std::floor(a / b) * b;}
float Utility::f_abs(const float a) { return a < 0 ? -a : a; }
float Utility::f_round(const float val) {
    if (const float fl = std::trunc(val); val - fl < 0.5f) return fl;
    else return fl+1.0f;
}
float Utility::VectorGetLength(const sf::Vector2f& val) {
    return std::sqrt(val.x * val.x + val.y * val.y);
}
sf::Vector2f Utility::VectorNormalized(const sf::Vector2f& val) {
    if (const float len = VectorGetLength(val); len > 0)
        return sf::Vector2f(val.x / len, val.y / len);
    return sf::Vector2f(0.f, 0.f);
}
int Utility::hexToInt(const std::string &hex) {
    return std::stoi(hex, nullptr, 16);
}