#ifndef MFCPP_UTILITY_HPP
#define MFCPP_UTILITY_HPP

class Utility final {
public:
    Utility() = delete;
    Utility(const Utility&) = delete;
    Utility(Utility&&) = delete;
    Utility& operator=(const Utility&) = delete;
    Utility& operator=(Utility&&) = delete;

    static int32_t RandomIntNumberGenerator(int32_t a, int32_t b);
    static float RandomFloatNumberGenerator(float a, float b);
    static float f_mod(float a, float b);
    static float f_min(float a, float b);
    static float f_max(float a, float b);
    static float f_abs(float a);
    static float f_round(float val);
    static float VectorGetLength(const sf::Vector2f& val);
    static sf::Vector2f VectorNormalized(const sf::Vector2f& val);
    static int32_t hexToInt(const std::string &hex);
private:
    static std::random_device seed;
};

#endif //MFCPP_UTILITY_HPP
