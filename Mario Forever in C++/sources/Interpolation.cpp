#include <SFML/Graphics.hpp>

#include "../headers/Core/ExternalHeaders/Kairos.hpp"
#include "../headers/Core/Interpolation.hpp"

bool isInterpolation = false;
sf::Vector2f linearInterpolation(sf::Vector2f start, sf::Vector2f end, float alpha)
{
	return (start * (1 - alpha) + end * alpha);
}