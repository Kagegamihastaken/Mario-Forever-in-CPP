#pragma once

#ifndef Coin_HPP
#define Coin_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "../headers/enum.hpp"

class Coin {
public:
	sf::Sprite property;
	sf::FloatRect hitbox;
};
extern std::vector<CoinID> CoinIDList;
extern std::vector<CoinAtt> CoinAttList;
extern int CoinCount;
extern std::vector<Coin> CoinList;
extern sf::Sound CoinSound;
extern void AddCoin(CoinID ID, CoinAtt att, float x, float y);
extern inline void CoinUpdate();
extern inline void CoinOnTouch();
extern void DeleteCoin(float x, float y);
#endif // Coin_HPP