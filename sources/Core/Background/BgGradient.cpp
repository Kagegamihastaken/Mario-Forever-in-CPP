#include "Core/Background/BgGradient.hpp"
#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Level.hpp"

// NOTE: Cannot change to auto, will found way to work
sf::VertexArray bgGradient(sf::PrimitiveType::TriangleStrip, 4);
void BgGradientSetColor(const sf::Color& firstC, const sf::Color& secondC) {
	bgGradient[0].color = bgGradient[1].color = firstC;
	bgGradient[2].color = bgGradient[3].color = secondC;
}
void BgGradientInitPos(const float Width, const float Height) {
	bgGradient[0].position = sf::Vector2f(0, 0);
	bgGradient[1].position = sf::Vector2f(Width, 0);
	bgGradient[2].position = sf::Vector2f(0, Height);
	bgGradient[3].position = sf::Vector2f(Width, Height);
}
void BgGradientDraw() {
	window.draw(bgGradient);
}