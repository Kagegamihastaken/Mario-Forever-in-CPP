#pragma once

#ifndef OBSTACLES_HPP
#define OBSTACLES_HPP

#include <SFML/Graphics.hpp>

class Obstacles {
public:
	int id = 0;
	sf::Sprite property;
	sf::FloatRect hitbox;
	void setHitbox(const sf::FloatRect& Sethitbox) {
		this->hitbox = Sethitbox;
	}
	sf::FloatRect getGlobalHitbox() const {
		return this->property.getTransform().transformRect(hitbox);
	}
	bool isCollide(sf::FloatRect& other) {
		return getGlobalHitbox().intersects(other);
	}
};
extern std::vector<std::vector<int>> ID_list;
extern std::vector<Obstacles> ObstaclesList;
extern sf::Texture ObstaclesTexture;
extern void ObstaclesUpdate();
#endif // OBSTACLES_HPP