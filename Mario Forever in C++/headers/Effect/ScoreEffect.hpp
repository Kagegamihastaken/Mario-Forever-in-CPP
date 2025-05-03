#pragma once

#ifndef SCOREEFFECT_HPP
#define SCOREEFFECT_HPP

#include "../Core/Loading/enum.hpp"

extern std::vector<sf::Sprite> ScoreEffectList;
extern void AddScoreEffect(ScoreID id, float x, float y);
extern void DeleteScoreEffect(float x, float y, float deltaTime);
extern void DeleteAllScoreEffect();
extern inline void ScoreEffectStatusUpdate(float deltaTime);
extern inline void ScoreEffectUpdate();
#endif