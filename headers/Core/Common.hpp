#ifndef COMMON_HPP
#define COMMON_HPP

namespace MFCPP {

    struct Color {
        float r, g, b, a;
        constexpr Color() : r(1.f), g(1.f), b(1.f), a(1.f) {};
        constexpr Color(const float _r, const float _g, const float _b, const float _a) : r(_r), g(_g), b(_b), a(_a) {};
        sf::Color ColorNormalize() const;

        static const Color LevelDefaultFirst;
        static const Color LevelDefaultSecond;
    };
}

#endif //COMMON_HPP
