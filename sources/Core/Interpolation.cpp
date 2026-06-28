#include "Core/Interpolation.hpp"

bool isInterpolation = true;
sf::Vector2f MFCPP::Interpolation::linearInterpolation(sf::Vector2f start, sf::Vector2f end, float alpha) {
	return {start.x * (1 - alpha) + end.x * alpha, start.y * (1 - alpha) + end.y * alpha};
}
sf::Angle MFCPP::Interpolation::linearInterpolation(sf::Angle start, sf::Angle end, float alpha) {
	return start * (1 - alpha) + end * alpha;
}