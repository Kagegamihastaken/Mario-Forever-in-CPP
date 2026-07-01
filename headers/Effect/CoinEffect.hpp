#pragma once

#ifndef COINEFFECT_HPP
#define COINEFFECT_HPP

#include "../Core/Enumeration/enum.hpp"
extern void CoinEffectInit();
extern void AddCoinEffect(CoinID ID, CoinAtt att, float x, float y);

//Animation
static constexpr int COINEFFECT_IMAGE_WIDTH = 296;
static constexpr int COINEFFECT_WIDTH = 37;
static constexpr int COINEFFECT_HEIGHT = 32;

#endif