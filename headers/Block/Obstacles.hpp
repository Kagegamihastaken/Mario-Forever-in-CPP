#pragma once

#ifndef OBSTACLES_HPP
#define OBSTACLES_HPP

#include "Core/ImageManager.hpp"

class Obstacles {
public:
	int id = 0;
	sf::Sprite property;
	sf::FloatRect hitbox;
	sf::Vector2f curr = sf::Vector2f(0, 0);
	sf::Vector2f prev = sf::Vector2f(0, 0);
};
extern sf::VertexArray ObstaclesVA;
extern sf::RenderTexture ObstacleRTexture;
extern std::vector<std::array<int, 3>> ID_list;
extern ImageManager ObstaclesTextureManager;
extern void loadObstacleRes();
extern void ObstaclesDraw();
#endif // OBSTACLES_HPP