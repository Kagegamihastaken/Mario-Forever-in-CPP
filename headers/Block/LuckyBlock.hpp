#pragma once
#ifndef LUCKYBLOCK_HPP
#define LUCKYBLOCK_HPP

#include "Obstacles.hpp"
#include "Core/Class/LuckyBlockClass.hpp"
#include "Core/ExternalHeaders/plf_colony.h"
#include "Core/Loading/enum.hpp"

extern plf::colony<MFCPP::LuckyBlock> LuckyBlock;
extern void SetPrevLuckyBlockPos();
extern void InterpolateLuckyBlockPos(float alpha);
extern void LuckyHit(float x, float y);
extern void AddLuckyBlock(LuckyBlockID ID, LuckyBlockAtt Att, float x, float y);
extern void LoadLuckyBlock();
extern void LuckyBlockDraw();
extern void LuckyBlockUpdate(float deltaTime);
extern void DeleteLuckyBlock(float x, float y);
extern void DeleteAllLuckyBlock();
extern void LuckyHitEvent(float x, float y);
extern void LuckyBlockCleanup();

#endif // LUCKYBLOCK_HPP