#pragma once

#ifndef Coin_HPP
#define Coin_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "../Core/Loading/enum.hpp"
#include "../Core/ImageManager.hpp"

class Coin {
public:
	sf::Sprite property = sf::Sprite(tempTex);
	sf::FloatRect hitbox;
};
extern std::vector<CoinID> CoinIDList;
extern std::vector<CoinAtt> CoinAttList;
extern int CoinCount;
extern std::vector<Coin> CoinList;
extern void CoinInit();
extern void AddCoin(CoinID ID, CoinAtt att, float x, float y);
extern void CoinUpdate();
extern void CoinOnTouch();
extern void DeleteAllCoin();
extern void DeleteLuckyBlock(float x, float y);
extern void DeleteCoin(float x, float y);
#endif // Coin_HPP