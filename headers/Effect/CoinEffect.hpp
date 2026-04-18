#pragma once

#ifndef COINEFFECT_HPP
#define COINEFFECT_HPP

#include "Core/Loading/enum.hpp"
extern void CoinEffectInit();
extern void AddCoinEffect(CoinID ID, CoinAtt att, float x, float y);

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