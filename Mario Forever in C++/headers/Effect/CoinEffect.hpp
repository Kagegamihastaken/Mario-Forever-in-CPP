#pragma once

#ifndef COINEFFECT_HPP
#define COINEFFECT_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include "../Core/Animate/LocalAnimationManager.hpp"
#include "../Core/Loading/enum.hpp"
#include "../Core/TextureManager.hpp"

class CoinEffect {
public:
	sf::Sprite property = sf::Sprite(tempTex);
	LocalAnimationManager coinEffectAnimation;
	float velocity = -3.5f;
};
extern std::vector<CoinEffect> CoinEffectList;
extern std::vector<CoinID> CoinEffectIDList;
extern void CoinEffectInit();
extern void DeleteAllCoinEffect();
extern void AddCoinEffect(CoinID ID, CoinAtt att, float x, float y);
extern inline void CoinEffectUpdate();
extern void DeleteCoinEffect(float x, float y);
extern inline void CoinEffectStatusUpdate();

#endif