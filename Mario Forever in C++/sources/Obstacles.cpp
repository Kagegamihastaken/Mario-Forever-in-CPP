#include <SFML/Graphics.hpp>
#include "../headers/Obstacles.hpp"
#include "../headers/WindowFrame.hpp"
#include "../headers/Scroll.hpp"
#include "../headers/Loading.hpp"

#include "../resource.h"

#include <string>
#include <vector>
#include <iostream>
#include <array>
#include <thread>

//Obstacles define
std::vector<Obstacles> ObstaclesList;
//set ID for each texture obstacle
std::vector<std::array<int, 3>> ID_list{
	{0, 0, 0},
	{1, 32, 0},
	{2, 64, 0},
	{3, 0, 32},
	{4, 32, 32},
	{5, 64, 32},
	{6, 96, 0}
};
std::vector<sf::Texture*> ObstaclesTextureList;
//texture loading

void loadObstacleRes() {
	sf::Texture* ObstaclesTexture = new sf::Texture();
	LoadTexture(*ObstaclesTexture, TILESET_TEXTURE);
	for (const auto& i : ID_list) {
		ObstaclesTextureList.push_back(new sf::Texture());
		ObstaclesTextureList.back()->loadFromImage(ObstaclesTexture->copyToImage(), sf::IntRect(i[1], i[2], 32, 32));
	}
	delete ObstaclesTexture;
}
void ObstaclesUpdate() {
	for (int i = 0; i < ObstaclesList.size(); ++i) {
		if (!isOutScreen(ObstaclesList[i].property.getPosition().x, ObstaclesList[i].property.getPosition().y, 32, 32)) {
			window.draw(ObstaclesList[i].property);
		}
	}
	//std::cout << ObstaclesList.size() % thread_count << std::endl;
}