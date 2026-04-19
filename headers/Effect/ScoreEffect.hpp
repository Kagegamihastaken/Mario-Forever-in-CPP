#pragma once

#ifndef SCOREEFFECT_HPP
#define SCOREEFFECT_HPP

#include "Core/Loading/enum.hpp"
#include "Core/ExternalHeaders/plf_colony.h"
#include "Core/Class/ScoreEffectClass.hpp"

extern void ScoreEffectInit();
extern void AddScoreEffect(ScoreID id, float x, float y);

#endif