#include <SFML/Graphics.hpp>
#include "../headers/Obstacles.hpp"
#include "../headers/WindowFrame.hpp"
#include "../headers/Scroll.hpp"
#include "../headers/Loading.hpp"

#include "../resource.h"

#include <string>
#include <vector>
#include <iostream>

//Obstacles define
std::vector<Obstacles> ObstaclesList;
sf::Texture ObstaclesTexture;
//set ID for each texture obstacle
std::vector<std::vector<int>> ID_list{
	{0, 0, 0},
	{1, 32, 0},
	{2, 64, 0},
	{3, 0, 32},
	{4, 32, 32},
	{5, 64, 32},
	{6, 96, 0}
};
//texture loading

int loadObstacleRes() {
	LoadTexture(ObstaclesTexture, TILESET_TEXTURE);
	return 6;
}
const int ObstacleInit = loadObstacleRes();
void ObstaclesUpdate() {
	for (const auto& i : ObstaclesList) {
		if (!isOutScreen(i.property.getPosition().x, i.property.getPosition().y, 32, 32)) {
			window.draw(i.property);
		}
	}
}