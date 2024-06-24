#pragma once

#ifndef COINEFFECT_HPP
#define COINEFFECT_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include "../headers/LocalAnimationManager.hpp"
#include "../headers/enum.hpp"

class CoinEffect {
public:
	sf::Sprite property;
	LocalAnimationManager coinEffectAnimation;
	float velocity = -3.5f;
};

extern sf::Texture CoinEffectTexture;
extern std::vector<CoinEffect> CoinEffectList;
extern std::vector<CoinID> CoinEffectIDList;
extern void AddCoinEffect(CoinID ID, CoinAtt att, float x, float y);
extern void CoinEffectUpdate();
extern void DeleteCoinEffect(float x, float y);
extern void CoinEffectStatusUpdate();

#endif