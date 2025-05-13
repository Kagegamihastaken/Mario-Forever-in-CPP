#include <SFML/Graphics.hpp>

#include "Core/Interpolation.hpp"

bool isInterpolation = true;
sf::Vector2f linearInterpolation(const sf::Vector2f start, const sf::Vector2f end, const float alpha)
{
	return (start * (1 - alpha) + end * alpha);
}