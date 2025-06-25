#pragma once
#ifndef LUCKYBLOCK_HPP
#define LUCKYBLOCK_HPP

#include "Obstacles.hpp"
#include "Core/Loading/enum.hpp"

extern std::vector<Obstacles> LuckyBlock;
extern std::vector<LuckyBlockID> LuckyBlockIDList;
extern std::vector<std::pair<sf::FloatRect, sf::Vector2f>> LuckyVertPosList;
extern std::vector<std::pair<sf::FloatRect, sf::Vector2f>> LuckyHorzPosList;
extern void SetPrevLuckyBlockPos();
extern void InterpolateLuckyBlockPos(float alpha);
extern void LuckyBlockSort();
extern void LuckyHit(float x, float y, int i);
extern int getLuckyIndex(float x, float y);
extern void AddLuckyBlock(LuckyBlockID ID, LuckyBlockAtt Att, float x, float y);
extern void LoadLuckyBlock();
extern void LuckyBlockDraw();
extern void LuckyBlockUpdate(float deltaTime);
extern void DeleteAllLuckyBlock();
void LuckyHitEvent(float x, float y);

#endif // LUCKYBLOCK_HPP