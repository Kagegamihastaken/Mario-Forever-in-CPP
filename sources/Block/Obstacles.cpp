#include "Block/Obstacles.hpp"
#include "Core/WindowFrame.hpp"

//Obstacles define
sf::VertexArray ObstaclesVA;
sf::RenderTexture ObstacleRTexture;
void ObstaclesDraw() {
	WindowFrame::getWindow().draw(ObstaclesVA, &ObstacleRTexture.getTexture());
}