#include <SFML/Graphics.hpp>

#include "Core/Background/BgGradient.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Level.hpp"

auto bgGradient = sf::VertexArray(sf::PrimitiveType::TriangleStrip, 4);
void BgGradientInitPos() {
	//bgGradient[0].position = sf::Vector2f(ViewX - (view.getSize().x - Width), ViewY - (view.getSize().y - Height));
	//bgGradient[1].position = sf::Vector2f(ViewX + view.getSize().x, ViewY - (view.getSize().y - Height));
	//bgGradient[2].position = sf::Vector2f(ViewX - (view.getSize().x - Width), ViewY + view.getSize().y);
	//bgGradient[3].position = sf::Vector2f(ViewX + view.getSize().x, ViewY + view.getSize().y);

	bgGradient[0].position = sf::Vector2f(0, 0);
	bgGradient[1].position = sf::Vector2f(LevelWidth, 0);
	bgGradient[2].position = sf::Vector2f(0, LevelHeight);
	bgGradient[3].position = sf::Vector2f(LevelWidth, LevelHeight);
}
void BgGradientDraw() {
	window.draw(bgGradient);
}