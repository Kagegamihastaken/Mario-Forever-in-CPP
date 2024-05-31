#pragma once

#ifndef Coin_HPP
#define Coin_HPP

#include <SFML/Graphics.hpp>

class Coin {
public:
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
extern int CoinCount;
extern std::vector<Coin> CoinList;
extern void addCoin(float x, float y);
extern void CoinUpdate();
extern void CoinOnTouch();
extern void DeleteCoin(float x, float y);
#endif // Coin_HPP