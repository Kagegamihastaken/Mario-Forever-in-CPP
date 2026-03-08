#pragma once
#ifndef LUCKYBLOCK_HPP
#define LUCKYBLOCK_HPP

#include "Core/Loading/enum.hpp"

extern void LuckyBLockTemp();
extern void ForceLoadLuckyBlockTexture(LuckyBlockID ID);
extern void AddLuckyBlock(LuckyBlockID ID, LuckyBlockAtt Att, float x, float y);
extern void LoadLuckyBlock();

extern std::vector<std::string> NormLuckyBlockAnimName;

#endif // LUCKYBLOCK_HPP