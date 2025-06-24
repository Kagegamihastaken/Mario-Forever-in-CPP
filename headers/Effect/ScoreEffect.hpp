#pragma once

#ifndef SCOREEFFECT_HPP
#define SCOREEFFECT_HPP

#include "../Core/Loading/enum.hpp"

extern std::vector<sf::Sprite> ScoreEffectList;
extern void SetPrevScoreEffectPos();
extern void InterpolateScoreEffectPos(float alpha);
extern void AddScoreEffect(ScoreID id, float x, float y);
extern void DeleteScoreEffect(int i, float deltaTime);
extern void DeleteAllScoreEffect();
extern void ScoreEffectStatusUpdate(float deltaTime);
extern void ScoreEffectDraw();
#endif