#pragma once

#ifndef COINEFFECT_HPP
#define COINEFFECT_HPP

#include "Core/Loading/enum.hpp"
#include "Core/Class/CoinEffectClass.hpp"
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
extern void CoinEffectCleanup();

//Animation
static std::vector<std::string> CoinEffectAnimName = {
    "CoinEffect_0", "CoinEffect_1", "CoinEffect_2", "CoinEffect_2", "CoinEffect_2", "CoinEffect_1", "CoinEffect_3",
    "CoinEffect_0", "CoinEffect_1", "CoinEffect_2", "CoinEffect_2", "CoinEffect_2", "CoinEffect_1", "CoinEffect_3",
    "CoinEffect_4", "CoinEffect_4", "CoinEffect_5", "CoinEffect_5", "CoinEffect_6", "CoinEffect_6", "CoinEffect_7"
};
static constexpr int COINEFFECT_IMAGE_WIDTH = 296;
static constexpr int COINEFFECT_WIDTH = 37;
static constexpr int COINEFFECT_HEIGHT = 32;

#endif