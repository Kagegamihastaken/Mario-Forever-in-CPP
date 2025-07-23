#include "Core/Common.hpp"

namespace MFCPP {
    sf::Color Color::ColorNormalize() const {
        return sf::Color(static_cast<uint8_t>(r * 255.f), static_cast<uint8_t>(g * 255.f), static_cast<uint8_t>(b * 255.f), static_cast<uint8_t>(a * 255.f));
    }
    constexpr Color Color::LevelDefaultFirst{116.f / 255.f, 149.f / 255.f, 245.f / 255.f, 1.f};
    constexpr Color Color::LevelDefaultSecond{245.f / 255.f, 254.f / 255.f, 253.f / 255.f, 1.f};
}