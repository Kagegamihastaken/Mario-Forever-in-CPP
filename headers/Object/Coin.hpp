#pragma once

#ifndef Coin_HPP
#define Coin_HPP

#include "../Core/Enumeration/enum.hpp"
#include "Core/Class/CoinClass.hpp"
#include "Core/ExternalHeaders/plf_colony.h"

extern int CoinCount;
extern plf::colony<MFCPP::Coin> CoinList;
extern void CoinInit();
extern void AddCoin(CoinID ID, CoinAtt att, float x, float y);
extern void CoinDraw(float alpha);
extern void CoinOnTouch();
extern void DeleteAllCoin();
extern void DeleteLuckyBlock(float x, float y);
extern void DeleteIndexCoin(const plf::colony<MFCPP::Coin>::colony_iterator<false>& index);
extern void ForceLoadCoinTexture();
extern void CoinCleanup();
extern void CoinAnimationUpdate(float deltaTime);
#endif // Coin_HPP