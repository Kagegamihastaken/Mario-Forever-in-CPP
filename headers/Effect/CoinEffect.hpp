#pragma once

#ifndef COINEFFECT_HPP
#define COINEFFECT_HPP

#include <vector>
#include "Core/Loading/enum.hpp"
#include "Class/CoinEffectClass.hpp"
extern std::vector<MFCPP::CoinEffect> CoinEffectList;
extern void SetPrevCoinEffectPos();
extern void InterpolateCoinEffectPos(float alpha);
extern void ScoreEffectInit();
extern void CoinEffectInit();
extern void DeleteAllCoinEffect();
extern void AddCoinEffect(CoinID ID, CoinAtt att, float x, float y);
extern void CoinEffectDraw();
extern void DeleteCoinEffect(float x, float y);
extern void CoinEffectStatusUpdate(float deltaTime);

static std::vector<std::string> CoinEffectAnimName;
static constexpr int COINEFFECT_IMAGE_WIDTH = 777;
static constexpr int COINEFFECT_WIDTH = 37;
static constexpr int COINEFFECT_HEIGHT = 32;

#endif