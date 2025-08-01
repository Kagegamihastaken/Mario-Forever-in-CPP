#include "Core/Interpolation.hpp"

bool isInterpolation = true;
sf::Vector2f linearInterpolation(const sf::Vector2f start, const sf::Vector2f end, const float alpha) {
	const double dsx = static_cast<double>(start.x);
	const double dsy = static_cast<double>(start.y);
	const double dex = static_cast<double>(end.x);
	const double dey = static_cast<double>(end.y);
	const double da = static_cast<double>(alpha);
	return {static_cast<float>(dsx * (1 - da) + dex * alpha), static_cast<float>(dsy * (1 - da) + dey * da)};
	//return {start.x * (1 - alpha) + end.x * alpha, start.y * (1 - alpha) + end.y * alpha};
}
sf::Angle linearInterpolation(const sf::Angle start, const sf::Angle end, const float alpha) {
	const double da = static_cast<double>(alpha);
	return start * (1 - da) + end * da;
	//return {static_cast<float>(dsx * (1 - da) + dex * alpha), static_cast<float>(dsy * (1 - da) + dey * da)};
	//return {start.x * (1 - alpha) + end.x * alpha, start.y * (1 - alpha) + end.y * alpha};
}