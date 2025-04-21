#include <SFML/Graphics.hpp>
#include "../headers/Block/Obstacles.hpp"
#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Core/Scroll.hpp"
#include "../headers/Core/Loading/Loading.hpp"
#include "../headers/Core/TextureManager.hpp"

#include "../resource.h"

#include <string>
#include <vector>
#include <iostream>
#include <array>
#include <thread>

//Obstacles define
sf::VertexArray ObstaclesVA;
std::vector<Obstacles> ObstaclesList;
std::vector<std::pair<sf::FloatRect, sf::Vector2f>> ObstaclesVertPosList;
//set ID for each texture obstacle
std::vector<std::array<int, 3>> ID_list{
	{0, 0, 0},
	{1, 32, 0},
	{2, 64, 0},
	{3, 0, 32},
	{4, 32, 32},
	{5, 64, 32},
	{6, 96, 0},
	{7, 128, 0},
	{8, 160, 0},
	{9, 128, 32},
	{10, 160, 32},
};
TextureManager ObstaclesTextureManager;
//texture loading

void loadObstacleRes() {
	ObstaclesTextureManager.Loadingtexture(TILESET_TEXTURE, "Tileset", 0, 0, 192, 64);
	for (int i = 0; i < ID_list.size(); ++i) {
		ObstaclesTextureManager.Loadingtexture(TILESET_TEXTURE, "Obstacles_" + std::to_string(i), ID_list[i][1], ID_list[i][2], 32, 32);
	}
}
void ObstaclesUpdate() {
	//sf::RenderStates states;
	//sf::Transformable trans;
	//states.transform *= trans.getTransform();
	//states.texture = ObstaclesTextureManager.GetTexture("Tileset");
	window.draw(ObstaclesVA, ObstaclesTextureManager.GetTexture("Tileset"));
	//for (int i = 0; i < ObstaclesList.size(); ++i) {
	//	if (!isOutScreen(ObstaclesList[i].property.getPosition().x, ObstaclesList[i].property.getPosition().y, 32, 32)) {
	//		window.draw(ObstaclesList[i].property);
	//	}
	//}
	//std::cout << ObstaclesList.size() % thread_count << std::endl;
}