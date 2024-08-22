#pragma once

#ifndef OBSTACLES_HPP
#define OBSTACLES_HPP

#include <SFML/Graphics.hpp>

class Obstacles {
public:
	int id = 0;
	sf::Sprite property;
	sf::FloatRect hitbox;
};
extern std::vector<std::array<int, 3>> ID_list;
extern std::vector<Obstacles> ObstaclesList;
extern sf::Texture ObstaclesTexture;
extern void ObstaclesUpdate();
#endif // OBSTACLES_HPP