#pragma once

#ifndef COINEFFECT_HPP
#define COINEFFECT_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include "../Core/Animate/SingleAnimationObject.hpp"
#include "../Core/Loading/enum.hpp"
#include "../Core/ImageManager.hpp"

class CoinEffect {
public:
	sf::Vector2f curr = sf::Vector2f(0, 0);
	sf::Vector2f prev = sf::Vector2f(0, 0);
	sf::Sprite property = sf::Sprite(tempTex);
	SingleAnimationObject coinEffectAnimation;
	float velocity = -3.5f;
};
extern std::vector<CoinEffect> CoinEffectList;
extern std::vector<CoinID> CoinEffectIDList;
extern void SetPrevCoinEffectPos();
extern void InterpolateCoinEffectPos(float alpha);
extern void ScoreEffectInit();
extern void CoinEffectInit();
extern void DeleteAllCoinEffect();
extern void AddCoinEffect(CoinID ID, CoinAtt att, float x, float y);
extern void CoinEffectUpdate();
extern void DeleteCoinEffect(float x, float y);
extern void CoinEffectStatusUpdate(float deltaTime);

#endif