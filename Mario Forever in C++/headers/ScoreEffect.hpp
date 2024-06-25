#pragma once

#ifndef SCOREEFFECT_HPP
#define SCOREEFFECT_HPP

extern std::vector<sf::Sprite> ScoreEffectList;
extern void AddScoreEffect(ScoreID id, float x, float y);
extern void DeleteScoreEffect(float x, float y);
extern inline void ScoreEffectStatusUpdate();
extern inline void ScoreEffectUpdate();
#endif