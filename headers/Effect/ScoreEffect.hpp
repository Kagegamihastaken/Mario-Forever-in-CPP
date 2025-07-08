#pragma once

#ifndef SCOREEFFECT_HPP
#define SCOREEFFECT_HPP

#include "Core/Loading/enum.hpp"
#include "Core/ExternalHeaders/plf_colony.h"
#include "Core/Class/ScoreEffectClass.hpp"

extern plf::colony<MFCPP::ScoreEffect> ScoreEffectList;
extern void SetPrevScoreEffectPos();
extern void InterpolateScoreEffectPos(float alpha);
extern void AddScoreEffect(ScoreID id, float x, float y);
extern void DeleteAllScoreEffect();
extern void ScoreEffectStatusUpdate(float deltaTime);
extern void ScoreEffectDraw();
extern void ScoreEffectCleanup();

#endif