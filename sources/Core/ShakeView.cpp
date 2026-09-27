#include "Core/ShakeView.hpp"
#include "Core/Utility.hpp"

void MFCPP::ShakeView::shake(float val) {
    intensity = val;
}

void MFCPP::ShakeView::update(float deltaTime) {
    if (intensity > 0.f) {
        intensity -= deltaTime * 1.f;
        if (intensity < 0.f) intensity = 0.f;
    }
}
sf::Vector2f MFCPP::ShakeView::getOffset() {
    if (intensity <= 0.f) return sf::Vector2f(0.f, 0.f);
    return sf::Vector2f( Utility::RandomIntNumberGenerator(-1, 1) * intensity, Utility::RandomIntNumberGenerator(-1, 1) * intensity);
}
