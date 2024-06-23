#pragma once

#ifndef COINEFFECT_HPP
#define COINEFFECT_HPP

#include <vector>
#include <SFML/Graphics.hpp>

extern sf::Texture CoinEffectTexture;
extern std::vector<sf::Sprite> CoinEffectList;
extern void AddCoinEffect(float x, float y);
extern void CoinEffectUpdate();

#endif