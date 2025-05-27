#include <SFML/Graphics.hpp>

#include "Core/Interpolation.hpp"
#include "Core/WindowFrame.hpp"

bool isInterpolation = true;
sf::Vector2f linearInterpolation(const sf::Vector2f start, const sf::Vector2f end, const float alpha) {
	return {std::round(start.x) * (1 - alpha) + std::round(end.x) * alpha, std::round(start.y) * (1 - alpha) + std::round(end.y) * alpha};
	//return {start.x * (1 - alpha) + end.x * alpha, start.y * (1 - alpha) + end.y * alpha};
}